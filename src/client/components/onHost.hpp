
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
void SetClientBlur(int clientNum, int timeMs, float radius);

void ToggleGodMode_f(int clientNum);
void ToggleInfiniteAmmo_f(int clientNum);
void ToggleNoSpread_f(int clientNum);
void ToggleNoRecoil_f(int clientNum);
void ToggleFullAutoWeapons_f(int clientNum);
void ToggleMovementSpeed_f(int clientNum);
void SelectFov_f(int clientNum);
void ToggleNoclip_f(int clientNum);
void ToggleUfoMode_f(int clientNum);
void ToggleNoclipBind_f(int clientNum);
void ToggleRocketRide_f(int clientNum);
void ToggleRocketJump_f(int clientNum);
void ToggleRocketJumpStrength_f(int clientNum);
void SetAllPerks_f(int clientNum);
void ClearAllPerks_f(int clientNum);
void SelectPerk_f(int clientNum);
void SelectWeapon_f(int clientNum);
void RefillAmmo_f(int clientNum);
void TakeCurrentWeapon_f(int clientNum);
void ResetProjectile_f(int clientNum);
void SetProjectile_f(int clientNum);
void SelectVision_f(int clientNum);
void SavePosition_f(int clientNum);
void LoadPosition_f(int clientNum);
void TeleportToCrosshair_f(int clientNum);
void ToggleTeleportGun_f(int clientNum);
void SelectTimeScale_f(int clientNum);
void SelectPlayerSpeed_f(int clientNum);
void SelectJumpHeight_f(int clientNum);
void SetThemeMatrix_f(int clientNum);
void SetThemeClassic_f(int clientNum);
void SetThemeEnstoneSmall_f(int clientNum);
void SetThemeEnstoneLarge_f(int clientNum);
void SetThemeEnstoneRight_f(int clientNum);
void SetThemeEnstoneSmall2_f(int clientNum);
void ThemeColorRed_f(int clientNum);
void ThemeColorGreen_f(int clientNum);
void ThemeColorBlue_f(int clientNum);
void ThemeColorYellow_f(int clientNum);
void MoveMenuUp_f(int clientNum);
void MoveMenuDown_f(int clientNum);
void MoveMenuLeft_f(int clientNum);
void MoveMenuRight_f(int clientNum);

namespace onHost
{
    void start();
    void stop();
}