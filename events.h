#ifndef EVENTS_H
#define EVENTS_H

#include <libinput.h>
#include <libudev.h>
#include <xkbcommon/xkbcommon.h>

#include <string>
#include <unordered_map>

class Events {
public:
    Events();
    ~Events();

    void run();

private:
    struct udev *udev;
    struct libinput *li;
    struct libinput_event *event;
    struct xkb_context *xkb_ctx;
    struct xkb_keymap *keymap;
    struct xkb_state *xkb_state;
    int fd;

    static int open_restricted(const char *path, int flags, void *user_data);
    static void close_restricted(int fd, void *user_data);

    std::string keysym_to_keyd_name(const char *keysym_name);
    void setup_libinput();
    void setup_xkb();
    void poll_events();
    void handle_keyboard_event(struct libinput_event_keyboard *k);
    void cleanup();
    void setLogFilePath(const std::string& path);
    std::string logFilePath;
};

#endif // EVENTS_H
