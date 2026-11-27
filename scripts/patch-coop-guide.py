"""Identify and replace the English co-op guide in a locally extracted Odyssey RomFS."""

import argparse
from collections import defaultdict
from pathlib import Path
import re
import struct


FIELDS = {
    "heading": (r"Play as a Team", "Local Co-op"),
    "controllers": (r"Two people take a Joy[-‑–]Con each\.", "Connect two controllers."),
    "roles": (r"Decide who will be\s+Mario and\s+Cappy\.", "Each player controls a character."),
    "shared screen": (r"Start the game and play together as a team\.", "Play together on one screen."),
    "start action": (r"Start in Two[-‑–]Player Mode", "Start Local Co-op"),
}


def endian(data, offset):
    mark = data[offset:offset + 2]
    if mark not in (b"\xff\xfe", b"\xfe\xff"):
        raise ValueError("Invalid byte-order mark")
    return "<" if mark == b"\xff\xfe" else ">"


def unpack(data, order, layout, offset):
    return struct.unpack_from(order + layout, data, offset)


def decode_archive(data):
    if not data.startswith(b"Yaz0"):
        return bytearray(data)
    size, = unpack(data, ">", "I", 4)
    output = bytearray()
    cursor = 16
    while len(output) < size:
        flags = data[cursor]
        cursor += 1
        for bit in range(7, -1, -1):
            if len(output) == size:
                break
            if flags & (1 << bit):
                output.append(data[cursor])
                cursor += 1
                continue
            first, second = data[cursor:cursor + 2]
            cursor += 2
            distance = ((first & 15) << 8 | second) + 1
            length = (first >> 4) + 2
            if length == 2:
                length = data[cursor] + 18
                cursor += 1
            if distance > len(output) or len(output) + length > size:
                raise ValueError("Invalid Yaz0 back-reference")
            for _ in range(length):
                output.append(output[-distance])
    return output


def encode_archive(data, original):
    if not original.startswith(b"Yaz0"):
        return data
    # Literal-only Yaz0 keeps the original compression wrapper without dependencies.
    output = bytearray(original[:16])
    for offset in range(0, len(data), 8):
        output.append(0xff)
        output.extend(data[offset:offset + 8])
    return output


def sarc_files(data):
    if not data.startswith(b"SARC"):
        raise ValueError("Expected a SARC archive")
    order = endian(data, 6)
    sfat, = unpack(data, order, "H", 4)
    data_start, = unpack(data, order, "I", 12)
    if data[sfat:sfat + 4] != b"SFAT":
        raise ValueError("Missing SARC file table")
    header_size, count = unpack(data, order, "HH", sfat + 4)
    nodes = sfat + header_size
    sfnt = nodes + count * 16
    if data[sfnt:sfnt + 4] != b"SFNT":
        raise ValueError("Missing SARC name table")
    names = sfnt + unpack(data, order, "H", sfnt + 4)[0]
    for index in range(count):
        node = nodes + index * 16
        attributes, start, end = unpack(data, order, "III", node + 4)
        if not attributes >> 24:
            continue
        name_offset = names + (attributes & 0xffffff) * 4
        name_end = data.index(0, name_offset, data_start)
        name = data[name_offset:name_end].decode("utf-8")
        if not 0 <= start <= end <= len(data) - data_start:
            raise ValueError(f"Invalid SARC bounds for {name}")
        yield name, data_start + start, data_start + end


def visible_text(data, order):
    # Preserve the original positions of characters while skipping inline icon
    # and formatting tags. The confirm button's glyph stays outside the match.
    chars = []
    offsets = []
    pos = 0
    while pos + 2 <= len(data):
        code, = unpack(data, order, "H", pos)
        if code == 0:
            break
        if code == 0x0e:
            size, = unpack(data, order, "H", pos + 6)
            pos += 8 + size
            continue
        if code == 0x0f:
            pos += 6
            continue
        chars.append(chr(code))
        offsets.append(pos)
        pos += 2
    return "".join(chars), offsets


def message_records(data):
    order = endian(data, 8)
    if data[12] != 1:
        raise ValueError("Expected UTF-16 MSBT messages")
    section_count, = unpack(data, order, "H", 14)
    offset = 32
    sections = {}
    for _ in range(section_count):
        kind = bytes(data[offset:offset + 4])
        size, = unpack(data, order, "I", offset + 4)
        start = offset + 16
        end = start + size
        if end > len(data):
            raise ValueError("Truncated MSBT section")
        sections[kind] = (start, end)
        offset = (end + 15) & ~15
    if b"LBL1" not in sections or b"TXT2" not in sections:
        return

    labels = defaultdict(list)
    base, end = sections[b"LBL1"]
    groups, = unpack(data, order, "I", base)
    for group in range(groups):
        count, relative = unpack(data, order, "II", base + 4 + group * 8)
        cursor = base + relative
        for _ in range(count):
            size = data[cursor]
            stop = cursor + 1 + size
            if stop + 4 > end:
                raise ValueError("Truncated MSBT label")
            label = data[cursor + 1:stop].decode("utf-8")
            index, = unpack(data, order, "I", stop)
            labels[index].append(label)
            cursor = stop + 4

    base, end = sections[b"TXT2"]
    count, = unpack(data, order, "I", base)
    offsets = unpack(data, order, f"{count}I", base + 4)
    ordered = sorted(set(offsets) | {end - base})
    limits = dict(zip(ordered, ordered[1:]))
    for index, relative in enumerate(offsets):
        if relative < 4 + count * 4 or relative not in limits:
            raise ValueError("Invalid MSBT message offset")
        yield labels[index], order, base + relative, base + limits[relative]


def inspect(archives):
    matches = defaultdict(set)
    for path, (_, archive) in archives.items():
        for name, file_start, file_end in sarc_files(archive):
            data = archive[file_start:file_end]
            if not data.startswith(b"MsgStdBn"):
                continue
            for labels, order, start, end in message_records(data):
                text, positions = visible_text(data[start:end], order)
                if not labels:
                    continue
                for field, (pattern, _) in FIELDS.items():
                    for match in re.finditer(pattern, text, re.IGNORECASE):
                        first = start + positions[match.start()]
                        last = start + positions[match.end() - 1] + 2
                        matches[field].add((path, name, tuple(labels), order,
                                            file_start + start, file_start + end,
                                            first - start, last - start))
    return matches


def patch(archives, matches):
    edits = defaultdict(list)
    for field, entries in matches.items():
        path, _, _, order, start, end, first, last = next(iter(entries))
        encoding = "utf-16-le" if order == "<" else "utf-16-be"
        edits[path, start, end].append((first, last, FIELDS[field][1].encode(encoding)))
    for (path, start, end), ranges in edits.items():
        _, archive = archives[path]
        message = archive[start:end]
        for first, last, replacement in sorted(ranges, reverse=True):
            message[first:last] = replacement
        if len(message) > end - start:
            raise ValueError("Replacement would overflow the original message slot")
        archive[start:end] = message.ljust(end - start, b"\0")
    return {path for path, _, _ in edits}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("romfs", type=Path, help="Root of your unmodified Odyssey RomFS extract")
    parser.add_argument("--install-to", type=Path,
                        help="Ryujinx's Open Mods Directory for Odyssey. Omit to identify fields only.")
    args = parser.parse_args()

    archives = {}
    localized = Path("LocalizedData/USen/MessageData")
    candidates = [localized / name for name in ("LayoutMessage.szs", "SystemMessage.szs")]
    candidates.append(Path("LayoutData/ControllerGuideMulti.szs"))
    for relative in candidates:
        source = args.romfs / relative
        if source.is_file():
            original = source.read_bytes()
            archives[relative] = (original, decode_archive(original))
    if not any(path.parent == localized for path in archives):
        parser.error("US English LayoutMessage.szs or SystemMessage.szs was not found under the RomFS root")

    matches = inspect(archives)
    for field in FIELDS:
        entries = sorted(matches[field])
        if not entries:
            print(f"{field}: no matching message")
        for path, name, labels, *_ in entries:
            print(f"{field}: {path} / {name} / {', '.join(labels)}")
    if any(len(matches[field]) != 1 for field in FIELDS):
        parser.exit(1, "Expected exactly one match per field. No files were written.\n")
    # Keep the core guide edits in one message table. The shared footer/button
    # caption is allowed to live in its own table.
    guide = {next(iter(matches[field]))[:2] for field in FIELDS if field != "start action"}
    if len(guide) != 1:
        parser.exit(1, "Guide fields were split across message tables. No files were written.\n")
    if not args.install_to:
        return
    if args.install_to.name.lower() != "0100000000010000" or not args.install_to.is_dir():
        parser.error("--install-to must be Odyssey's existing Ryujinx title mod directory")

    changed = patch(archives, matches)
    output = args.install_to / "OCoop Guide" / "romfs"
    for path in sorted(changed):
        original, archive = archives[path]
        destination = output / path
        destination.parent.mkdir(parents=True, exist_ok=True)
        temporary = destination.with_suffix(".tmp")
        temporary.write_bytes(encode_archive(archive, original))
        temporary.replace(destination)
        print(f"Installed {destination}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, IndexError, struct.error) as error:
        raise SystemExit(f"Guide patch failed: {error}") from None
