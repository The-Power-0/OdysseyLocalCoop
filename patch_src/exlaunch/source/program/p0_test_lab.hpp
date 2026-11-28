#pragma once
// Pure menu model. Native layout, controllers and scenario actions live in main.cpp.
namespace ocoop::p0lab {
constexpr unsigned ScenarioCount = 5;
constexpr unsigned LegacyItemCount = 8;
constexpr unsigned ItemCount = LegacyItemCount + 3;
enum class Command { Next, Confirm, Toggle, Cancel };
enum class Action { None, MarkOrReplay, Clear, ReloadStage };
struct Menu {
    unsigned selected = 0;
    bool visible = true;
    bool armed = false;
};
struct Result { bool changed; bool execute; Action action = Action::None; };
constexpr unsigned Scenario(const Menu& menu) {
    return menu.selected < LegacyItemCount ? menu.selected % 4 : 4;
}
constexpr Action SelectedAction(const Menu& menu) {
    if (menu.selected == 10) return Action::ReloadStage;
    return (menu.selected < 4 || menu.selected == 8) ? Action::MarkOrReplay : Action::Clear;
}
constexpr bool NeedsP2Health(unsigned scenario) { return scenario == 2 || scenario == 3; }
struct ReloadGuards {
    bool optedIn, twoPlayers, sameScene, safePlayers, saveIdle, transitionIdle, restartReady;
    constexpr bool Allowed() const {
        return optedIn && twoPlayers && sameScene && safePlayers && saveIdle &&
               transitionIdle && restartReady;
    }
};
inline Result Apply(Menu& menu, Command command) {
    if (command == Command::Toggle) {
        menu.visible = !menu.visible;
        menu.armed = false;
        return {true, false};
    }
    if (command == Command::Cancel) {
        const bool changed = menu.armed;
        menu.armed = false;
        return {changed, false};
    }
    if (!menu.visible) return {false, false};
    if (command == Command::Next) {
        menu.selected = (menu.selected + 1) % ItemCount;
        menu.armed = false;
        return {true, false};
    }
    if (!menu.armed) {
        menu.armed = true;
        return {true, false};
    }
    menu.armed = false;
    return {true, true, SelectedAction(menu)};
}
} // namespace ocoop::p0lab
