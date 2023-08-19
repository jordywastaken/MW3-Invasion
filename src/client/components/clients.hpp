
#pragma once

#include "hudelems.hpp"

static constexpr int ClientMaxOptions = 25;

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

class ClientOption
{
public:
    ClientOption(const char* text, void(*callback)(int));
    ~ClientOption();

    void AddChild(ClientOption* child);
    size_t GetChildCount();

    const char* text;
    void(*callback)(int);
    ClientOption* children[ClientMaxOptions];
};

#define MakeSubmenu(name) new ClientOption(name, 0)
#define MakeOption(name, cb) new ClientOption(name, cb)

class Client
{
public:
    void CreateMenu();
    void DestroyMenu();
    void CreateHudElems();
    void DestroyHudElems();
    void SetDefaults();
    void ClearAll();

    void HandleInput();
    void InputSleep(int ms);
    bool InputReady();

    void ChangeSubmenu(ClientOption* submenu);
    void OnOpen();
    void OnClose();
    void OnPress();
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
    ClientOption* rootMenu;
    ClientOption* currentMenu;
    ClientOption* previousMenu[10];
    int previousOption[10];
    int submenuLevel;
    HudElem* hudBackground;
    HudElem* hudLeftBorder;
    HudElem* hudNavBar;
    HudElem* hudTitle;
    HudElem* hudAuthor;
    HudElem* hudCurrentMenu;
    HudElem* hudOptions[ClientMaxOptions];

    // Options
    vec3_t menuColor;
    bool infiniteAmmo;
    bool rocketRide;
    bool rocketJump;
    float rocketJumpStrength;
    const char* projectile;
    bool positionSaved;
    bool teleportGun;
    float savedPosition[3];
};

extern Client users[2];

void GameMessage(int clientNum, const char* text);
void GameMessageBold(int clientNum, const char* text);
void SetClientDvar(int clientNum, const char* dvar, const char* value);

void ToggleGodMode(int clientNum);
void ToggleInfiniteAmmo(int clientNum);
void ToggleNoSpread(int clientNum);
void ToggleNoRecoil(int clientNum);
void ToggleMovementSpeed(int clientNum);
void ToggleRocketRide(int clientNum);
void ToggleRocketJump(int clientNum);
void ToggleRocketJumpStrength(int clientNum);
void SetAllPerks(int clientNum);
void ClearAllPerks(int clientNum);
void SelectPerk(int clientNum);
void SelectWeapon(int clientNum);
void ResetProjectile(int clientNum);
void SetProjectile(int clientNum);
void SavePosition(int clientNum);
void LoadPosition(int clientNum);
void TeleportToCrosshair(int clientNum);
void ToggleTeleportGun(int clientNum);
void MenuColorRed(int clientNum);
void MenuColorGreen(int clientNum);
void MenuColorBlue(int clientNum);
void MenuColorYellow(int clientNum);

namespace clients
{
    void start();
    void stop();
}