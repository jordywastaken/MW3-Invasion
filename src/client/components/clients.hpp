
#pragma once

#include "hudelems.hpp"

enum ClientButtons
{
    CLIENT_BTN_CROSS = (1 << 0),
    CLIENT_BTN_CIRCLE = (1 << 1),
    CLIENT_BTN_DPAD_UP = (1 << 2),
    CLIENT_BTN_DPAD_DOWN = (1 << 3),
    CLIENT_BTN_DPAD_LEFT = (1 << 4),
    CLIENT_BTN_DPAD_RIGHT = (1 << 5),
    CLIENT_BTN_L1 = (1 << 6),
    CLIENT_BTN_R3 = (1 << 7),

};

class Client
{
public:
    void SetDefaults();
    void Destroy();

    void HandleInput();
    void InputSleep(int ms);
    bool InputReady();

    void CreateHudElems();
    void OnOpen();
    void OnClose();
    void OnCancel();
    void OnScrollUp();
    void OnScrollDown();

    void Run();

    // Menu
    int clientNum;
    bool opened;
    int buttonBits;
    int buttonTick;
    int currentOption;
    HudElem* hudBackground;
    HudElem* hudLeftBorder;
    HudElem* hudNavBar;
    HudElem* hudTitle;
    HudElem* hudAuthor;
    HudElem* hudCurrentMenu;
    HudElem* hudOptions[20];

    // Options
    bool bGodMode;
    vec3_t menuColor;
};

extern Client users[2];

namespace clients
{
    void start();
    void stop();
}