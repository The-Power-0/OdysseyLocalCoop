#include "program/coop_guide.hpp"

#include <cassert>

int main() {
    using namespace std::literals::string_view_literals;
    using ocoop::guide::Rewrite;
    char16_t output[ocoop::guide::kMaxMessageUnits + 1];
    std::size_t length = 0;
    assert(Rewrite(u"Play as a Team", output, length) == 0x01);
    assert(std::u16string_view(output, length) == u"Local Co-op");
    assert(Rewrite(u"1. Two people take a Joy\u2011Con each.", output, length) == 0x02);
    assert(std::u16string_view(output, length) == u"1. Connect two controllers.");
    assert(Rewrite(u"2. Decide who will be \x0e\x01\x02\x02\x00"
                   u"Mario and \x0e\x01\x02\x02\x00" u"Cappy."sv, output, length) == 0x04);
    assert(std::u16string_view(output, length) == u"2. Each player controls a character.");
    assert(Rewrite(u"3. Start the game and play together as a team.", output, length) == 0x08);
    assert(std::u16string_view(output, length) == u"3. Play together on one screen.");

    auto confirm = u"\x0e\x01\x02\x02\x00 Start in Two-Player Mode\0\0"sv;
    auto expected = u"\x0e\x01\x02\x02\x00 Start Local Co-op\0\0"sv;
    assert(Rewrite(confirm, output, length) == 0x10);
    assert(std::u16string_view(output, length) == expected);
    assert(output[length] == 0);

    assert(Rewrite(u"Controller Settings", output, length) == 0);
    assert(Rewrite(u"Start Local Co-op", output, length) == 0);
    assert(Rewrite(u"\x0e\x01"sv, output, length) == 0);
}
