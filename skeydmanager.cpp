#include "skeydmanager.h"
#include "exception.h"

#include<fcntl.h>
#include<iostream>
#include<poll.h>
#include<unistd.h>
#include<cctype>
#include<QMessageBox>
#include<QHash>
SkeydManager::SkeydManager()
{
    udev = udev_new();
    if(!udev)
    {
        throw Exception("Failed to create udev");
    }

    interface.open_restricted = open_restricted;
    interface.close_restricted = close_restricted;

    li = libinput_udev_create_context(&interface, nullptr, udev);

    if(!li)
    {
        throw Exception("Failed to create libinput context");
    }

    if(libinput_udev_assign_seat(li,"seat0") != 0)
    {
        throw Exception("Failed to assign seat");
    }

    xkb_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    keymap = xkb_keymap_new_from_names(xkb_ctx, nullptr, XKB_KEYMAP_COMPILE_NO_FLAGS);
    xkb_state = xkb_state_new(keymap);

    fd = libinput_get_fd(li);
    }

SkeydManager::~SkeydManager()
    {
        xkb_state_unref(xkb_state);
        xkb_keymap_unref(keymap);
        xkb_context_unref(xkb_ctx);
        libinput_unref(li);
        udev_unref(udev);
    }

void    SkeydManager::Run()
    {
        while(true)
        {
            struct pollfd fds = {fd, POLLIN, 0};

            if(poll(&fds, 1, -1) > 0)
            {
                libinput_dispatch(li);
                handleEvents();
            }
        }
    }



    int SkeydManager::open_restricted(const char *path, int flag, void *)
    {
        return open(path, flag);
    }

    void SkeydManager::close_restricted(int fd, void *)
    {
        close(fd);
    }


    void SkeydManager::handleEvents()
    {
        while ((event = libinput_get_event(li)) != nullptr)
        {
            if(libinput_event_get_type(event) == LIBINPUT_EVENT_KEYBOARD_KEY)
            {
                handleKeyboardEvent(libinput_event_get_keyboard_event(event));
            }
            libinput_event_destroy(event);
        }
    }



    QString SkeydManager::handleKeyboardEvent(struct libinput_event_keyboard *event)
    {
        uint32_t key = libinput_event_keyboard_get_key(event);
        uint32_t state = libinput_event_keyboard_get_key_state(event);

        xkb_keycode_t keycode = key + 8;

        const xkb_keysym_t *syms;
        int num_syms = xkb_state_key_get_syms(xkb_state, keycode, &syms);

        if (num_syms > 0)
        {
            char name[64];
            xkb_keysym_get_name(syms[0], name, sizeof(name));

            QString keyd_name = keysym_to_keyd_name(name);
            return keyd_name;
        }

        return {};
    }

    QString SkeydManager::keysym_to_keyd_name(const char *keysym_name)
    {
        static const QHash<QString, QString> mapping =
        {
           {"Shift_L", "shift"},       {"Shift_R", "shift"},
           {"Control_L", "ctrl"},      {"Control_R", "ctrl"},
           {"Alt_L", "alt"},           {"Alt_R", "alt"},
           {"Return", "enter"},        {"Escape", "esc"},
           {"BackSpace", "backspace"}, {"Tab", "tab"},
           {"space", "space"},         {"Super_L", "super"},
           {"Super_R", "super"},       {"Caps_Lock", "capslock"},
           {"Delete", "delete"},       {"Insert", "insert"},
           {"Home", "home"},           {"End", "end"},
           {"Page_Up", "pageup"},      {"Page_Down", "pagedown"},
           {"Menu", "menu"},           {"Num_Lock", "numlock"},
           {"Print", "printscreen"},   {"Scroll_Lock", "scrolllock"},
           {"Pause", "pause"},
        };

        QString key = QString::fromUtf8(keysym_name);
        if (mapping.contains(key)) {
            return mapping.value(key);
        }

        return key.toLower();
    }
