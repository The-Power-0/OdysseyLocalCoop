#include "program/p0_test_lab.hpp"
#include <cassert>
using namespace ocoop::p0lab;

int main() {
    Menu m{};
    assert(m.selected == 0 && m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute && m.armed);
    auto result = Apply(m, Command::Confirm);
    assert(result.execute && result.action == Action::MarkOrReplay && !m.armed);
    Apply(m, Command::Confirm);
    Apply(m, Command::Next);
    assert(m.selected == 1 && !m.armed);
    for (unsigned i = 1; i < ScenarioCount; ++i) Apply(m, Command::Next);
    assert(m.selected == ScenarioCount && Scenario(m) == 0);
    assert(SelectedAction(m) == Action::Clear);
    Apply(m, Command::Confirm);
    assert(Apply(m, Command::Cancel).changed && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    result = Apply(m, Command::Confirm);
    assert(result.execute && result.action == Action::Clear);
    for (unsigned i = 0; i < ScenarioCount; ++i) Apply(m, Command::Next);
    assert(m.selected == 0);
    Apply(m, Command::Confirm);
    Apply(m, Command::Toggle);
    assert(!m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    assert(!Apply(m, Command::Next).changed && m.selected == 0);
    Apply(m, Command::Toggle);
    assert(m.visible && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    assert(Apply(m, Command::Confirm).execute);
    return 0;
}
