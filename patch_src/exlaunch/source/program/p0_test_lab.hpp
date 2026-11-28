#pragma once
// Pure menu model. Native layout, controllers and scenario actions live in main.cpp.
namespace ocoop::p0lab {
constexpr unsigned ScenarioCount = 4;
constexpr unsigned ItemCount = ScenarioCount * 2;
enum class Command { Next, Confirm, Toggle, Cancel };
enum class Action { None, MarkOrReplay, Clear };
struct Menu {
    unsigned selected = 0;
    bool visible = true;
    bool armed = false;
};
struct Result { bool changed; bool execute; Action action = Action::None; };
constexpr unsigned Scenario(const Menu& menu) { return menu.selected % ScenarioCount; }
constexpr Action SelectedAction(const Menu& menu) {
    return menu.selected < ScenarioCount ? Action::MarkOrReplay : Action::Clear;
}
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
