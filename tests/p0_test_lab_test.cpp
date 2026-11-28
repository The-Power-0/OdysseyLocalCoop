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
    for (unsigned i = 1; i < 4; ++i) Apply(m, Command::Next);
    assert(m.selected == 4 && Scenario(m) == 0);
    assert(SelectedAction(m) == Action::Clear);
    Apply(m, Command::Confirm);
    assert(Apply(m, Command::Cancel).changed && !m.armed);
    assert(!Apply(m, Command::Confirm).execute);
    result = Apply(m, Command::Confirm);
    assert(result.execute && result.action == Action::Clear);
    for (unsigned i = 0; i < 4; ++i) Apply(m, Command::Next);
    assert(m.selected == 8 && Scenario(m) == 4 && SelectedAction(m) == Action::MarkOrReplay);
    Apply(m, Command::Next);
    assert(Scenario(m) == 4 && SelectedAction(m) == Action::Clear);
    Apply(m, Command::Next);
    assert(SelectedAction(m) == Action::ReloadStage);
    assert(!Apply(m, Command::Confirm).execute);
    assert(Apply(m, Command::Confirm).action == Action::ReloadStage);
    Apply(m, Command::Next);
    assert(m.selected == 0);
    assert(NeedsP2Health(2) && NeedsP2Health(3) && !NeedsP2Health(4));
    const ReloadGuards ok{true,true,true,true,true,true,true};
    assert(ok.Allowed());
    for (unsigned i=0; i<7; ++i) {
        ReloadGuards g=ok;
        bool* flags[]={&g.optedIn,&g.twoPlayers,&g.sameScene,&g.safePlayers,&g.saveIdle,&g.transitionIdle,&g.restartReady};
        *flags[i]=false;
        assert(!g.Allowed());
    }
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
