#include "events.h"
#include <iostream>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <fstream>
#include <QDebug>
#include <unordered_map>
#include <cctype>

Events::Events() : udev(nullptr), li(nullptr), event(nullptr),
    xkb_ctx(nullptr), keymap(nullptr), xkb_state(nullptr), fd(-1) {}

Events::~Events() {
    cleanup();
}

int Events::open_restricted(const char *path, int flags, void *) {
    return open(path, flags);
}

void Events::close_restricted(int fd, void *) {
    close(fd);
}

std::string Events::keysym_to_keyd_name(const char *keysym_name) {
    static const std::unordered_map<std::string, std::string> mapping = {
        {"Shift_L", "shift"}, {"Shift_R", "shift"},
        {"Control_L", "ctrl"}, {"Control_R", "ctrl"},
        {"Alt_L", "alt"}, {"Alt_R", "alt"},
        {"Return", "enter"}, {"Escape", "esc"},
        {"BackSpace", "backspace"}, {"Tab", "tab"},
        {"space", "space"}, {"Super_L", "super"},
        {"Super_R", "super"}, {"Caps_Lock", "capslock"},
        {"Delete", "delete"}, {"Insert", "insert"},
        {"Home", "home"}, {"End", "end"},
        {"Page_Up", "pageup"}, {"Page_Down", "pagedown"},
        {"Menu", "menu"}, {"Num_Lock", "numlock"},
        {"Print", "printscreen"}, {"Scroll_Lock", "scrolllock"},
        {"Pause", "pause"}, {"Meta_L", "leftmeta"},
        {"Shift_L", "leftshift"}, {"F23", "f23"}
    };

    auto it = mapping.find(keysym_name);
    if (it != mapping.end()) {
        return it->second;
    }

    std::string name(keysym_name);
    for (auto &c : name) c = std::tolower(c);
    return name;
}

void Events::setup_libinput() {
    udev = udev_new();
    static struct libinput_interface interface = {
        .open_restricted = open_restricted,
        .close_restricted = close_restricted
    };
    li = libinput_udev_create_context(&interface, NULL, udev);
    if (!li) {
        throw std::runtime_error("Failed to create libinput context");
    }

    if (libinput_udev_assign_seat(li, "seat0") != 0) {
        throw std::runtime_error("Failed to assign seat");
    }

    fd = libinput_get_fd(li);
}

void Events::setup_xkb() {
    xkb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    keymap = xkb_keymap_new_from_names(xkb_ctx, NULL, XKB_KEYMAP_COMPILE_NO_FLAGS);
    xkb_state = xkb_state_new(keymap);
}

void Events::poll_events() {
    while (true) {
        struct pollfd fds = {fd, POLLIN, 0};
        if (poll(&fds, 1, -1) > 0) {
            libinput_dispatch(li);
            while ((event = libinput_get_event(li)) != NULL) {
                if (libinput_event_get_type(event) == LIBINPUT_EVENT_KEYBOARD_KEY) {
                    auto *k = libinput_event_get_keyboard_event(event);
                    handle_keyboard_event(k);
                }
                libinput_event_destroy(event);
            }
        }
    }
}

void Events::handle_keyboard_event(struct libinput_event_keyboard *k) {
    uint32_t key = libinput_event_keyboard_get_key(k);
    uint32_t state = libinput_event_keyboard_get_key_state(k);

    if (state != LIBINPUT_KEY_STATE_PRESSED) return;

    xkb_keycode_t keycode = key + 8;

    const xkb_keysym_t *syms;
    int num_syms = xkb_state_key_get_syms(xkb_state, keycode, &syms);

    if (num_syms > 0) {
        char name[64];
        xkb_keysym_get_name(syms[0], name, sizeof(name));

        std::string keyd_name = keysym_to_keyd_name(name);

        static std::string last_key = "";
        static bool have_last_key = false;

        if (!have_last_key) {
            last_key = keyd_name;
            have_last_key = true;
        } else {
            if (logFilePath.empty()) return;

            std::ofstream outfile(logFilePath, std::ios::app);
            if (outfile.is_open()) {
                outfile << "\n" << last_key << " = " << keyd_name << "\n";
                outfile.flush();
                outfile.close();
            }
            have_last_key = false;
        }
    }
}

void Events::cleanup() {
    if (xkb_state) xkb_state_unref(xkb_state);
    if (keymap) xkb_keymap_unref(keymap);
    if (xkb_ctx) xkb_context_unref(xkb_ctx);
    if (li) libinput_unref(li);
    if (udev) udev_unref(udev);
}

void Events::run() {
    try {
        setup_libinput();
        setup_xkb();
        poll_events();
    } catch (const std::exception &) {
        // Handle exceptions if needed
    }
    cleanup();
}

void Events::setLogFilePath(const std::string& path) {
    logFilePath = path;
}
