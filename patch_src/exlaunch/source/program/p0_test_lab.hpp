#pragma once

// Host-testable input and confirmation model for the experimental P0 Test Lab.
// Game-specific rendering, controllers and actions live in main.cpp.
namespace ocoop::p0lab {

enum class Command { Next, Confirm, Toggle };
struct Menu {
    unsigned selected = 0;
    bool visible = true;
    bool armed = false;
};
struct Result { bool changed; bool execute; };

inline Result Apply(Menu& menu, Command command) {
    if (command == Command::Toggle) {
        menu.visible = !menu.visible;
        menu.armed = false;
        return {true, false};
    }
    if (!menu.visible)
        return {false, false};
    if (command == Command::Next) {
        menu.selected = (menu.selected + 1) % 4;
        menu.armed = false;
        return {true, false};
    }
    if (!menu.armed) {
        menu.armed = true;
        return {true, false};
    }
    menu.armed = false;
    return {true, true};
}

} // namespace ocoop::p0lab
