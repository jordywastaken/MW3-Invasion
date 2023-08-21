
#pragma once

#include "hudelems.hpp"

static constexpr int ClientMaxOptions = 50;
static constexpr int ClientMaxViewableOptions = 10;

enum ClientButtons
{
    CLIENT_BTN_CROSS = (1 << 0),
    CLIENT_BTN_CIRCLE = (1 << 1),
    CLIENT_BTN_SQUARE = (1 << 2),
    CLIENT_BTN_DPAD_UP = (1 << 3),
    CLIENT_BTN_DPAD_DOWN = (1 << 4),
    CLIENT_BTN_DPAD_LEFT = (1 << 5),
    CLIENT_BTN_DPAD_RIGHT = (1 << 6),
    CLIENT_BTN_L1 = (1 << 7),
    CLIENT_BTN_L2 = (1 << 8),
    CLIENT_BTN_L3 = (1 << 9),
    CLIENT_BTN_R1 = (1 << 10),
    CLIENT_BTN_R2 = (1 << 11),
    CLIENT_BTN_R3 = (1 << 12),
};

enum ClientTheme
{
    CLIENT_THEME_MATRIX,
    CLIENT_THEME_CLASSIC,
    CLIENT_THEME_ENSTONE_SMALL,
    CLIENT_THEME_ENSTONE_LARGE,
    CLIENT_THEME_ENSTONE_RIGHT,
    CLIENT_THEME_ENSTONE_SMALL2
};

class ClientOption
{
public:
    ClientOption(const char* text, void(*callback)(int));
    ~ClientOption();

    void AddChild(ClientOption* child);
    void AddChild(const char* text, void(*callback)(int));
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

    void SetTheme(ClientTheme theme);
    void SetColor(vec3_t color);
    void Move(float xMove, float yMove);

    void Run();

    // Menu
    int clientNum;
    bool opened;
    int buttonBits;
    int buttonTick;
    int currentOption;
    int currentOptionOffset;
    ClientOption* rootMenu;
    ClientOption* currentMenu;
    ClientOption* previousMenu[10];
    int previousOption[10];
    int previousOptionOffset[10];
    int submenuLevel;
    ClientTheme currentTheme;
    HudElem* hudBackground;
    HudElem* hudComponents[2];
    HudElem* hudNavBar;
    HudElem* hudTitle;
    HudElem* hudAuthor;
    HudElem* hudCurrentMenu;
    HudElem* hudOptions[ClientMaxViewableOptions];

    // Options
    bool infiniteAmmo;
    bool fullAutoWeapons;
    bool noclip;
    bool ufo;
    bool noclipBind;
    bool rocketRide;
    bool rocketJump;
    float rocketJumpStrength;
    const char* projectile;
    bool positionSaved;
    bool teleportGun;
    float savedPosition[3];
    vec3_t menuColor;
    vec2_t menuPos;
};

extern Client users[2];

void GameMessage(int clientNum, const char* text);
void GameMessageBold(int clientNum, const char* text);
void PrintObjectifUpdate(int clientNum, const char* text);
void PrintObjectifComplete(int clientNum, const char* text);
void PrintObjectifFailed(int clientNum, const char* text);
void SetClientDvar(int clientNum, const char* dvar, const char* value);

void ToggleGodMode(int clientNum);
void ToggleInfiniteAmmo(int clientNum);
void ToggleNoSpread(int clientNum);
void ToggleNoRecoil(int clientNum);
void ToggleFullAutoWeapons(int clientNum);
void ToggleMovementSpeed(int clientNum);
void SelectFov(int clientNum);
void ToggleNoclip(int clientNum);
void ToggleUfoMode(int clientNum);
void ToggleNoclipBind(int clientNum);
void ToggleRocketRide(int clientNum);
void ToggleRocketJump(int clientNum);
void ToggleRocketJumpStrength(int clientNum);
void SetAllPerks(int clientNum);
void ClearAllPerks(int clientNum);
void SelectPerk(int clientNum);
void SelectWeapon(int clientNum);
void ResetProjectile(int clientNum);
void SetProjectile(int clientNum);
void SelectVision(int clientNum);
void SavePosition(int clientNum);
void LoadPosition(int clientNum);
void TeleportToCrosshair(int clientNum);
void ToggleTeleportGun(int clientNum);
void SelectTimeScale(int clientNum);
void SelectPlayerSpeed(int clientNum);
void SelectJumpHeight(int clientNum);
void SetThemeMatrix(int clientNum);
void SetThemeClassic(int clientNum);
void SetThemeEnstoneSmall(int clientNum);
void SetThemeEnstoneLarge(int clientNum);
void SetThemeEnstoneRight(int clientNum);
void SetThemeEnstoneSmall2(int clientNum);
void ThemeColorRed(int clientNum);
void ThemeColorGreen(int clientNum);
void ThemeColorBlue(int clientNum);
void ThemeColorYellow(int clientNum);
void MoveMenuUp(int clientNum);
void MoveMenuDown(int clientNum);
void MoveMenuLeft(int clientNum);
void MoveMenuRight(int clientNum);

namespace clients
{
    void start();
    void stop();
}