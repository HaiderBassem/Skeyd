#ifndef SKEYDMANAGER_H
#define SKEYDMANAGER_H

#include<libinput.h>
#include<libudev.h>
#include<xkbcommon/xkbcommon.h>

#include<QString>
#include<unordered_map>
using namespace std;

class SkeydManager
{
public:
    SkeydManager();
    ~SkeydManager();


    void Run();

private:

    struct udev *udev = nullptr;
    struct libinput *li = nullptr;
    struct libinput_event *event = nullptr;
    struct libinput_interface interface = {};
    struct xkb_context *xkb_ctx = nullptr;
    struct xkb_keymap *keymap = nullptr;
    struct xkb_state *xkb_state = nullptr;

    int fd = -1;

    static int open_restricted(const char *path, int falg, void *user_data);
    static void close_restricted(int fd, void *user_data);

    void handleEvents();
    QString handleKeyboardEvent(struct libinput_event_keyboard *k);
    static QString keysym_to_keyd_name(const char *keysym_name);

};

#endif // SKEYDMANAGER_H
