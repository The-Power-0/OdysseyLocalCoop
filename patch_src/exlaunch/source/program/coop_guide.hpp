#pragma once

#include <cstddef>
#include <string_view>

namespace ocoop::guide {

inline constexpr std::size_t kMaxMessageUnits = 256;

struct Field {
    std::u16string_view original;
    std::u16string_view replacement;
};

inline constexpr Field kFields[] = {
    {u"play as a team", u"Local Co-op"},
    {u"two people take a joy-con each.", u"Connect two controllers."},
    {u"decide who will be mario and cappy.", u"Each player controls a character."},
    {u"start the game and play together as a team.", u"Play together on one screen."},
    {u"start in two-player mode", u"Start Local Co-op"},
};

// The message data can contain control tags with zeroes in their payloads.
// Match visible text and preserve raw prefixes/suffixes such as the A icon.
inline unsigned Rewrite(std::u16string_view source,
                        char16_t (&output)[kMaxMessageUnits + 1],
                        std::size_t& outputLength) {
    if (source.empty() || source.size() > kMaxMessageUnits)
        return 0;

    char16_t visible[kMaxMessageUnits];
    std::size_t offsets[kMaxMessageUnits];
    std::size_t visibleSize = 0;
    for (std::size_t i = 0; i < source.size();) {
        char16_t code = source[i];
        if (code == 0)
            break;
        if (code == 0x0e || code == 0x0f) {
            const std::size_t header = code == 0x0e ? 4 : 3;
            if (source.size() - i < header)
                return 0;
            const std::size_t payloadBytes = code == 0x0e ? source[i + 3] : 0;
            if (payloadBytes % 2 || payloadBytes / 2 > source.size() - i - header)
                return 0;
            i += header + payloadBytes / 2;
            continue;
        }

        if (code >= u'A' && code <= u'Z')
            code += u'a' - u'A';
        if (code == u'\u2011' || code == u'\u2013')
            code = u'-';
        if (code == u'\t' || code == u'\n' || code == u'\r' || code == u'\u00a0')
            code = u' ';
        if (code != u' ' || !visibleSize || visible[visibleSize - 1] != u' ') {
            visible[visibleSize] = code;
            offsets[visibleSize++] = i;
        }
        ++i;
    }

    const std::u16string_view text(visible, visibleSize);
    std::size_t head = 0;
    std::size_t written = 0;
    unsigned replaced = 0;
    for (std::size_t i = 0; i < text.size();) {
        bool matched = false;
        for (unsigned field = 0; field < sizeof(kFields) / sizeof(kFields[0]); ++field) {
            const auto& entry = kFields[field];
            if (!text.substr(i).starts_with(entry.original))
                continue;
            const std::size_t first = offsets[i];
            i += entry.original.size();
            const std::size_t end = offsets[i - 1] + 1;
            if (entry.replacement.size() > end - first)
                return 0;
            for (; head < first; ++head)
                output[written++] = source[head];
            for (char16_t code : entry.replacement)
                output[written++] = code;
            head = end;
            replaced |= 1u << field;
            matched = true;
            break;
        }
        if (!matched)
            ++i;
    }

    if (!replaced)
        return 0;
    for (; head < source.size(); ++head)
        output[written++] = source[head];
    output[written] = 0;
    outputLength = written;
    return replaced;
}

} // namespace ocoop::guide
