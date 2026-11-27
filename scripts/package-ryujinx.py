"""Create an installer-free Ryujinx ZIP from the verified 1.4.1 development package."""

import hashlib
import json
from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile, ZipInfo

ROOT = Path(__file__).resolve().parents[1]
INPUT = ROOT / "artifacts/package"
OUTPUT = ROOT / "artifacts/release"
GAME_VERSION = "1.4.1"
PAYLOAD = "payload/contents/0100000000010000/"
FILES = {
    "exefs/main.npdm",
    "exefs/subsdk9",
    "romfs/LayoutData/OCoopScoreBoard.szs",
    "romfs/OCoop/settings.ini",
}


def digest(data):
    return hashlib.sha256(data).hexdigest().upper()


def main():
    version = (ROOT / "VERSION").read_text().strip()
    name = f"OdysseyTogether-{version}-smo-{GAME_VERSION}-Ryujinx.zip"
    verification = json.loads((ROOT / "artifacts/verify/verification-report.json").read_text())
    report = json.loads((INPUT / "package-report.json").read_text())
    archive = Path(report["zip"]["path"])
    if (verification["status"] != "PASS" or verification["version"] != version
            or verification["game_version"] != GAME_VERSION
            or verification["zip_sha256"].upper() != digest(archive.read_bytes())):
        raise SystemExit("Expected a verified Odyssey 1.4.1 package")

    records = report["payload_files"]
    if (len(records) != len(FILES)
            or {record["path"] for record in records} != {PAYLOAD + path for path in FILES}):
        raise SystemExit("Unexpected mod payload")

    payload = {}
    with ZipFile(archive) as source:
        for record in records:
            relative = record["path"]
            data = source.read(f"{archive.stem}/{relative}")
            if digest(data) != record["sha256"].upper():
                raise SystemExit(f"Payload hash mismatch for {relative}")
            payload[f"OdysseyTogether/{relative.removeprefix(PAYLOAD)}"] = data

    OUTPUT.mkdir(parents=True, exist_ok=True)
    result = OUTPUT / name
    with ZipFile(result, "w", compression=ZIP_DEFLATED) as target:
        for path, data in sorted(payload.items()):
            entry = ZipInfo(path, (2020, 1, 1, 0, 0, 0))
            target.writestr(entry, data, compress_type=ZIP_DEFLATED)
    with ZipFile(result) as target:
        if set(target.namelist()) != set(payload) or any(
                target.read(path) != data for path, data in payload.items()):
            raise SystemExit("Ryujinx ZIP verification failed")

    (OUTPUT / f"{name}.sha256").write_text(f"{digest(result.read_bytes())} *{name}\n")
    print(f"Verified drag-and-drop package: {result}")


if __name__ == "__main__":
    main()
