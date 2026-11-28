#include "program/p0_test_lab.hpp"
#include <cassert>
using ocoop::p0lab::Menu;
using ocoop::p0lab::Command;
using ocoop::p0lab::Apply;

int main() {
    Menu m{};
    assert(m.selected == 0 && m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute && m.armed);
    assert(Apply(m, Command::Confirm).execute && !m.armed);
    Apply(m, Command::Next); assert(m.selected == 1 && !m.armed);
    Apply(m, Command::Next); Apply(m, Command::Next); Apply(m, Command::Next);
    assert(m.selected == 0); // wrap
    Apply(m, Command::Confirm); assert(m.armed);
    Apply(m, Command::Toggle); assert(!m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    assert(m.selected == 0);
    Apply(m, Command::Toggle); assert(m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    assert(Apply(m, Command::Confirm).execute);
    return 0;
}
