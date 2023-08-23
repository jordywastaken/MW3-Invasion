
#include "onHost.hpp"
#include "scheduler.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/imports.hpp"

Client users[2];

ClientOption::ClientOption(const char* text, void(*callback)(int))
    : text(text), callback(callback)
{
    for (int i = 0; i < ClientMaxOptions; i++)
        children[i] = 0;
}

ClientOption::~ClientOption()
{
    for (int i = 0; i < ClientMaxOptions; i++)
    {
        if (!children[i])
            continue;

        delete children[i];
        children[i] = 0;
    }
}

void ClientOption::AddChild(ClientOption* child)
{
    for (int i = 0; i < ClientMaxOptions; i++)
    {
        if (children[i])
            continue;

        children[i] = child;
        return;
    }
}

void ClientOption::AddChild(const char* text, void(*callback)(int))
{
    AddChild(MakeOption(text, callback));
}

size_t ClientOption::GetChildCount()
{
    size_t count = 0;
    for (int i = 0; i < ClientMaxOptions; i++)
    {
        if (children[i])
            ++count;
    }
    return count;
}

///
/// Methods
///
void Client::CreateMenu()
{
    auto* mainMenu = MakeSubmenu("Main menu");

    /// 
    /// Main mods menu
    /// 
    auto* mainModsMenu = MakeSubmenu("Main mods menu");
    mainModsMenu->AddChild("God mode", ToggleGodMode_f);
    mainModsMenu->AddChild("Infinite ammo", ToggleInfiniteAmmo_f);
    mainModsMenu->AddChild("No spread", ToggleNoSpread_f);
    mainModsMenu->AddChild("No recoil", ToggleNoRecoil_f);
    mainModsMenu->AddChild("Full auto weapons", ToggleFullAutoWeapons_f);
    mainModsMenu->AddChild("Movement speed x2", ToggleMovementSpeed_f);

    // For now only the host can edit client side dvars
    if (clientNum == 0)
    {
        auto* fovMenu = MakeSubmenu("Fov menu");
        fovMenu->AddChild("65 (Default)", SelectFov_f);
        fovMenu->AddChild("80", SelectFov_f);
        fovMenu->AddChild("90", SelectFov_f);
        fovMenu->AddChild("100", SelectFov_f);
        fovMenu->AddChild("110", SelectFov_f);
        fovMenu->AddChild("120", SelectFov_f);
        mainModsMenu->AddChild(fovMenu);
    }

    auto* movementTypeMenu = MakeSubmenu("Movement type menu");
    movementTypeMenu->AddChild("Noclip", ToggleNoclip_f);
    movementTypeMenu->AddChild("UFO mode", ToggleUfoMode_f);
    movementTypeMenu->AddChild("Bind noclip to \x12", ToggleNoclipBind_f);
    mainModsMenu->AddChild(movementTypeMenu);

    mainMenu->AddChild(mainModsMenu);

    /// 
    /// Fun menu
    /// 
    auto* funMenu = MakeSubmenu("Fun menu");
    funMenu->AddChild("Rocket ride", ToggleRocketRide_f);
    funMenu->AddChild("Rocket jump", ToggleRocketJump_f);
    funMenu->AddChild("Rocket jump strength", ToggleRocketJumpStrength_f);
    mainMenu->AddChild(funMenu);

    /// 
    /// Perks menu
    /// 
    auto* perksMenu = MakeSubmenu("Perks menu");
    perksMenu->AddChild("Set all perks", SetAllPerks_f);
    perksMenu->AddChild("Clear all perks", ClearAllPerks_f);
    mainMenu->AddChild(perksMenu);

    for (int i = 0; i < 21; i++)
        perksMenu->AddChild(bg_perkNames[i], SelectPerk_f);

    /// 
    /// Weapon menu & Projectile menu
    /// 
    auto* weaponMenu = MakeSubmenu("Weapon menu");
    weaponMenu->AddChild(MakeSubmenu("Rifle menu"));
    weaponMenu->AddChild(MakeSubmenu("Submachine gun menu"));
    weaponMenu->AddChild(MakeSubmenu("Machine gun menu"));
    weaponMenu->AddChild(MakeSubmenu("Shotgun menu"));
    weaponMenu->AddChild(MakeSubmenu("Sniper menu"));
    weaponMenu->AddChild(MakeSubmenu("Pistol menu"));
    weaponMenu->AddChild(MakeSubmenu("Rocket launcher menu"));
    weaponMenu->AddChild("Take current weapon", TakeCurrentWeapon_f);
    weaponMenu->AddChild("Refill ammo", RefillAmmo_f);
    mainMenu->AddChild(weaponMenu);

    auto* projectileMenu = MakeSubmenu("Projectile menu");
    projectileMenu->AddChild("None", ResetProjectile_f);
    mainMenu->AddChild(projectileMenu);

    for (int i = 0; i < 240; i++)
    {
        if (!bg_weaponCompleteDefs[i])
            continue;

        debug_print("bg_weaponCompleteDefs[%i]->szInternalName : \"%s\"\n", i, bg_weaponCompleteDefs[i]->szInternalName);

        int childIndex = -1;
        switch (bg_weaponCompleteDefs[i]->weapDef->weapClass)
        {
        case(WEAPCLASS_RIFLE): childIndex = 0; break;
        case(WEAPCLASS_SMG): childIndex = 1; break;
        case(WEAPCLASS_MG): childIndex = 2; break;
        case(WEAPCLASS_SPREAD): childIndex = 3; break;
        case(WEAPCLASS_SNIPER): childIndex = 4; break;
        case(WEAPCLASS_PISTOL): childIndex = 5; break;
        case(WEAPCLASS_ROCKETLAUNCHER): childIndex = 6; break;
        }

        if (childIndex != -1)
        {
            weaponMenu->children[childIndex]->AddChild(bg_weaponCompleteDefs[i]->szInternalName, SelectWeapon_f);
        }

        if (bg_weaponCompleteDefs[i]->weapDef->weapType == WEAPTYPE_PROJECTILE)
        {
            projectileMenu->AddChild(bg_weaponCompleteDefs[i]->szInternalName, SetProjectile_f);
        }
    }

    ///
    /// Vision menu
    /// 
    auto* visionMenu = MakeSubmenu("Vision menu");
    visionMenu->AddChild("default", SelectVision_f);
    visionMenu->AddChild("default_night", SelectVision_f);
    visionMenu->AddChild("ac130", SelectVision_f);
    visionMenu->AddChild("ac130_inverted", SelectVision_f);
    visionMenu->AddChild("cheat_bw", SelectVision_f);
    visionMenu->AddChild("cheat_bw_contrast", SelectVision_f);
    visionMenu->AddChild("cheat_bw_invert", SelectVision_f);
    visionMenu->AddChild("cheat_bw_invert_contrast", SelectVision_f);
    visionMenu->AddChild("cheat_chaplinnight", SelectVision_f);
    visionMenu->AddChild("cheat_contrast", SelectVision_f);
    visionMenu->AddChild("cheat_invert", SelectVision_f);
    visionMenu->AddChild("cheat_invert_contrast", SelectVision_f);
    visionMenu->AddChild("end_game", SelectVision_f);
    visionMenu->AddChild("icbm", SelectVision_f);
    visionMenu->AddChild("invasion", SelectVision_f);
    visionMenu->AddChild("mpnuke", SelectVision_f);
    visionMenu->AddChild("mpnuke_aftermath", SelectVision_f);
    visionMenu->AddChild("near_death", SelectVision_f);
    visionMenu->AddChild("sepia", SelectVision_f);
    mainMenu->AddChild(visionMenu);

    /// 
    /// Teleport menu
    /// 
    auto* teleportMenu = MakeSubmenu("Teleport menu");
    teleportMenu->AddChild("Save position", SavePosition_f);
    teleportMenu->AddChild("Load position", LoadPosition_f);
    teleportMenu->AddChild("Teleport to crosshair", TeleportToCrosshair_f);
    teleportMenu->AddChild("Teleport gun", ToggleTeleportGun_f);
    mainMenu->AddChild(teleportMenu);

    /// 
    /// Lobby menu
    /// 
    auto* lobbyMenu = MakeSubmenu("Lobby menu");
    auto* timeScaleMenu = MakeSubmenu("Time scale menu");
    timeScaleMenu->AddChild("x0.1", SelectTimeScale_f);
    timeScaleMenu->AddChild("x0.25", SelectTimeScale_f);
    timeScaleMenu->AddChild("x0.5", SelectTimeScale_f);
    timeScaleMenu->AddChild("x0.75", SelectTimeScale_f);
    timeScaleMenu->AddChild("x1", SelectTimeScale_f);
    timeScaleMenu->AddChild("x2", SelectTimeScale_f);
    lobbyMenu->AddChild(timeScaleMenu);

    auto* playerSpeedMenu = MakeSubmenu("Player speed menu");
    playerSpeedMenu->AddChild("190 (Default)", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("250", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("400", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("500", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("600", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("700", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("800", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("900", SelectPlayerSpeed_f);
    playerSpeedMenu->AddChild("1000", SelectPlayerSpeed_f);
    lobbyMenu->AddChild(playerSpeedMenu);

    auto* jumpHeightMenu = MakeSubmenu("Jump height menu");
    jumpHeightMenu->AddChild("39 (Default)", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("50", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("100", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("150", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("250", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("500", SelectJumpHeight_f);
    jumpHeightMenu->AddChild("1000", SelectJumpHeight_f);
    lobbyMenu->AddChild(jumpHeightMenu);

    mainMenu->AddChild(lobbyMenu);

    /// 
    /// Settings menu
    /// 
    auto* themeMenu = MakeSubmenu("Theme menu");
    themeMenu->AddChild("Set theme Matrix", SetThemeMatrix_f);
    //themeMenu->AddChild("Set theme Classic", SetThemeClassic_f);
    themeMenu->AddChild("Set theme Enstone small", SetThemeEnstoneSmall_f);
    themeMenu->AddChild("Set theme Enstone large", SetThemeEnstoneLarge_f);
    //themeMenu->AddChild("Set theme Enstone right", SetThemeEnstoneRight_f);
    //themeMenu->AddChild("Set theme Enstone small2", SetThemeEnstoneSmall2_f);
    themeMenu->AddChild("Theme color red", ThemeColorRed_f);
    themeMenu->AddChild("Theme color green", ThemeColorGreen_f);
    themeMenu->AddChild("Theme color blue", ThemeColorBlue_f);
    themeMenu->AddChild("Theme color yellow", ThemeColorYellow_f);
    themeMenu->AddChild("Move menu up", MoveMenuUp_f);
    themeMenu->AddChild("Move menu down", MoveMenuDown_f);
    themeMenu->AddChild("Move menu left", MoveMenuLeft_f);
    themeMenu->AddChild("Move menu right", MoveMenuRight_f);
    mainMenu->AddChild(themeMenu);

    rootMenu = mainMenu;
    currentMenu = mainMenu;
}

void Client::DestroyMenu()
{
    if (!rootMenu)
        return;

    delete rootMenu;
    rootMenu = 0;
}

void Client::CreateHudElems()
{
    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        // Panels
        hudBackground = CreateMaterialHudElem(clientNum, "white", 250, 480, 67.0, 0.0, 0.0, { 0.0, 0.0, 0.0 }, 0.0);
        hudBackground->SetHorzAlign(HE_HORZALIGN_RIGHT);

        hudComponents[0] = CreateMaterialHudElem(clientNum, "white", 2, 480, 65.0, 0.0, 1.0, menuColor, 0.0);
        hudComponents[0]->SetHorzAlign(HE_HORZALIGN_RIGHT);

        hudNavBar = CreateMaterialHudElem(clientNum, "white", 250, 15, 67.0, 114.0 + (15.0 * currentOption), 1.0, menuColor, 0.0);
        hudNavBar->SetHorzAlign(HE_HORZALIGN_RIGHT);

        // Labels
        hudTitle = CreateTextHudElem(clientNum, "Invasion", 2.0, 192.0, 50.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudTitle->SetHorzAlign(HE_HORZALIGN_RIGHT);
        hudTitle->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudTitle->SetTextFont(HE_FONT_OBJECTIVE);
        hudTitle->SetTextGlow(menuColor);

        hudAuthor = CreateTextHudElem(clientNum, "by Jordy", 1.0, 192.0, 70.0, 1.0, menuColor, 0.0);
        hudAuthor->SetHorzAlign(HE_HORZALIGN_RIGHT);
        hudAuthor->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);

        hudCurrentMenu = CreateTextHudElem(clientNum, currentMenu->text, 1.6, 75.0, 85.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudCurrentMenu->SetHorzAlign(HE_HORZALIGN_RIGHT);

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i] = CreateTextHudElem(clientNum, currentMenu->children[i] ? currentMenu->children[i]->text : "", 1.0, 75.0, 115.0 + (15.0 * i), 2.0, { 1.0, 1.0, 1.0 }, 0.0);
            hudOptions[i]->SetHorzAlign(HE_HORZALIGN_RIGHT);
        }
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        // Panels
        hudBackground = CreateMaterialHudElem(clientNum, "white", 180, 70, menuPos.x, menuPos.y, 0.0, { 0.0, 0.0, 0.0 }, 0.0);
        hudBackground->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudBackground->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudComponents[0] = CreateMaterialHudElem(clientNum, "white", 180, 50, menuPos.x, menuPos.y - 10.0, 1.0, menuColor, 0.0);
        hudComponents[0]->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudComponents[0]->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudComponents[1] = CreateMaterialHudElem(clientNum, "white", 180, 20, menuPos.x, menuPos.y + 25.0, 1.0, menuColor, 0.0);
        hudComponents[1]->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudComponents[1]->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudNavBar = CreateMaterialHudElem(clientNum, "white", 180, 15, menuPos.x, menuPos.y - 45.0 + (15.0 * currentOption), 1.0, menuColor, 0.0);
        hudNavBar->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudNavBar->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        // Labels
        hudTitle = CreateTextHudElem(clientNum, "Invasion", 2.0, menuPos.x, menuPos.y - 10.0, 2.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudTitle->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudTitle->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);
        hudTitle->SetTextFont(HE_FONT_OBJECTIVE);

        hudAuthor = CreateTextHudElem(clientNum, "by Jordy", 0.9, menuPos.x - 85.0, menuPos.y + 25.0, 2.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudAuthor->SetAlignOrg(HE_ALIGN_X_LEFT, HE_ALIGN_Y_MIDDLE);
        hudAuthor->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudCurrentMenu = CreateTextHudElem(clientNum, currentMenu->text, 1.2, menuPos.x, menuPos.y - 65.0, 2.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudCurrentMenu->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudCurrentMenu->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i] = CreateTextHudElem(clientNum, currentMenu->children[i] ? currentMenu->children[i]->text : "", 1.0, menuPos.x, menuPos.y - 45.0 + (15.0 * i), 2.0, { 1.0, 1.0, 1.0 }, 0.0);
            hudOptions[i]->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
            hudOptions[i]->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);
        }

        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        hudBackground = CreateMaterialHudElem(clientNum, "white", 0, 0, 0.0, 0.0, 0.0, { 0.0, 0.0, 0.0 }, 0.0);
        hudBackground->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudBackground->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudTitle = CreateTextHudElem(clientNum, "Invasion", 2.0, 0.0, -205.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudTitle->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudTitle->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);
        hudTitle->SetTextFont(HE_FONT_OBJECTIVE);

        hudAuthor = CreateTextHudElem(clientNum, "by Jordy", 0.9, 0.0, -185.0, 1.0, { 1.0, 1.0, 0.0 }, 0.0);
        hudAuthor->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudAuthor->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        hudCurrentMenu = CreateTextHudElem(clientNum, currentMenu->text, 1.2, 0.0, -155.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudCurrentMenu->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
        hudCurrentMenu->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i] = CreateTextHudElem(clientNum, currentMenu->children[i] ? currentMenu->children[i]->text : "", i == currentOption ? 1.25 : 1.0, 0.0, -120.0 + (15.0 * i), 1.0, i == currentOption ? menuColor : vec3_t(1.0, 1.0, 1.0), 0.0);
            hudOptions[i]->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
            hudOptions[i]->SetAlignScreen(HE_HORZALIGN_CENTER, HE_VERTALIGN_MIDDLE);
            hudOptions[i]->SetTextGlow(menuColor, i == currentOption ? 1.0 : 0.0);
        }

        break;
    }
    }
}

void Client::DestroyHudElems()
{
    delete hudBackground, hudBackground = 0;
    delete hudComponents[0], hudComponents[0] = 0;
    delete hudComponents[1], hudComponents[1] = 0;
    delete hudNavBar, hudNavBar = 0;
    delete hudTitle, hudTitle = 0;
    delete hudAuthor, hudAuthor = 0;
    delete hudCurrentMenu, hudCurrentMenu = 0;

    for(auto& option : hudOptions)
        delete option, option = 0;
}

void Client::SetDefaults()
{
    // Menu
    opened = false;
    buttonBits = 0;
    buttonTick = 0;
    currentOption = 0;
    currentOptionOffset = 0;
    rootMenu = 0;
    currentMenu = 0;

    for (int i = 0; i < 10; i++)
    {
        previousMenu[i] = 0;
        previousOption[i] = 0;
        previousOptionOffset[i] = 0;
    }
    submenuLevel = 0;

    hudBackground = 0;
    hudComponents[0] = 0;
    hudComponents[1] = 0;
    hudNavBar = 0;
    hudTitle = 0;
    hudAuthor = 0;
    hudCurrentMenu = 0;

    for (int i = 0; i < ClientMaxViewableOptions; i++)
        hudOptions[i] = 0;

    // Options
    menuColor = { 0.008, 0.5, 0.2 };
    menuPos = { 0.0, 0.0 };
    infiniteAmmo = false;
    fullAutoWeapons = false;
    noclip = false;
    ufo = false;
    noclipBind = false;
    rocketRide = false;
    rocketJump = false;
    rocketJumpStrength = 256.0;
    projectile = 0;
    positionSaved = false;
    teleportGun = false;
    savedPosition[0] = 0;
    savedPosition[1] = 0;
    savedPosition[2] = 0;
}

void Client::ClearAll()
{
    DestroyHudElems();
    DestroyMenu();
    SetDefaults();
}

void Client::HandleInput()
{
    // Handle bind first down
    if (noclipBind && (buttonBits & CLIENT_BTN_L2) && !noclip)
        ToggleNoclip_f(clientNum);

    // Handle bind released
    if (noclipBind && !(buttonBits & CLIENT_BTN_L2) && noclip)
        ToggleNoclip_f(clientNum);

    if (InputReady())
    {
        if (!opened)
        {
            int binds = (CLIENT_BTN_L1 | CLIENT_BTN_R3);
            if ((buttonBits & binds) == binds)
            {
                OnOpen();
            }
        }
        else
        {
            if (buttonBits & CLIENT_BTN_CROSS)
            {
                OnPress();
            }
            else if (buttonBits & CLIENT_BTN_CIRCLE)
            {
                OnCancel();
            }
            else if (buttonBits & CLIENT_BTN_DPAD_UP)
            {
                OnScrollUp();
            }
            else if (buttonBits & CLIENT_BTN_DPAD_DOWN)
            {
                OnScrollDown();
            }
        }
    }
}

void Client::InputSleep(int ms)
{
    buttonTick = Sys_Milliseconds() + ms;
}

bool Client::InputReady()
{
    return Sys_Milliseconds() >= buttonTick;
}

void Client::ChangeSubmenu(ClientOption* submenu)
{
    currentMenu = submenu;

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        hudCurrentMenu->SetText(submenu->text);
        hudCurrentMenu->SetAlpha(0.0);
        hudCurrentMenu->SetAlpha(1.0, 400);

        hudNavBar->SetAlpha(0.0);
        hudNavBar->SetAlpha(0.5, 400);
        hudNavBar->SetY(114.0 + (currentOption - currentOptionOffset) * 15.0);

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i]->SetText(submenu->children[i + currentOptionOffset] ? submenu->children[i + currentOptionOffset]->text : "");
            hudOptions[i]->SetAlpha(0.0);
            hudOptions[i]->SetAlpha(1.0, 400);
        }
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        hudCurrentMenu->SetText(submenu->text);
        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i]->SetText(submenu->children[i + currentOptionOffset] ? submenu->children[i + currentOptionOffset]->text : "");
        }
        hudNavBar->SetY(menuPos.y - 45.0 + (currentOption - currentOptionOffset) * 15.0);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        hudCurrentMenu->SetText(submenu->text);
        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i]->SetText(submenu->children[i + currentOptionOffset] ? submenu->children[i + currentOptionOffset]->text : "");
            hudOptions[i]->SetTextScale(i == currentOption ? 1.25 : 1.0);
            hudOptions[i]->SetTextGlow(menuColor, i == currentOption ? 1.0 : 0.0);
            hudOptions[i]->SetRGB(i == currentOption ? menuColor : vec3_t(1.0, 1.0, 1.0));
        }
        break;
    }
    }
}

void Client::OnOpen()
{
    if (!rootMenu)
        CreateMenu();

    if (!hudBackground)
        CreateHudElems();

    opened = true;

    level.clients[clientNum].ps.weapCommon.weapFlags |= (1 << 11); // Disable action slots
    level.clients[clientNum].ps.otherFlags |= (1 << 14); // Disable hud

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        // Slide in
        hudBackground->AddX(-250.0, 400);
        hudComponents[0]->AddX(-250.0, 400);
        hudTitle->AddX(-250.0, 400);
        hudAuthor->AddX(-250.0, 400);

        hudNavBar->AddX(-250.0, 400);
        hudCurrentMenu->AddX(-250.0, 400);
        for (auto option : hudOptions)
            option->AddX(-250.0, 400);

        // Fade in
        hudBackground->SetAlpha(0.6, 800);
        hudComponents[0]->SetAlpha(1.0, 800);
        hudTitle->SetAlpha(1.0, 800);
        hudAuthor->SetAlpha(1.0, 800);

        // Start fading after the slide in animation
        hudNavBar->SetAlpha(0.6, 400, 400);
        hudCurrentMenu->SetAlpha(1.0, 400, 400);
        for (auto option : hudOptions)
            option->SetAlpha(1.0, 400, 400);

        SetClientBlur(clientNum, 600, 0.0);
        InputSleep(800);
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        hudBackground->SetAlpha(0.6);
        hudComponents[0]->SetAlpha(0.6);
        hudComponents[1]->SetAlpha(0.6);
        hudNavBar->SetAlpha(0.6, 750);
        hudTitle->SetAlpha(1.0);
        hudAuthor->SetAlpha(1.0);
        hudCurrentMenu->SetAlpha(1.0, 750);

        for (auto& option : hudOptions)
            option->SetAlpha(1.0, 750);

        hudBackground->SetHeight(260, 400);
        hudComponents[0]->AddY(-95.0, 400);
        hudComponents[1]->AddY(95.0, 400);
        hudTitle->AddY(-95.0, 400);
        hudAuthor->AddY(95.0, 400);

        SetClientBlur(clientNum, 600, 0.0);
        InputSleep(400);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        hudBackground->SetAlpha(0.75, 400);
        hudBackground->ScaleOvertime(854, 480, 400);

        hudTitle->SetAlpha(1.0, 200, 400);
        hudAuthor->SetAlpha(1.0, 200, 400);
        hudCurrentMenu->SetAlpha(1.0, 200, 400);

        for (auto& option : hudOptions)
            option->SetAlpha(1.0, 200, 400);

        SetClientBlur(clientNum, 600, 2.0);
        InputSleep(600);
        break;
    }
    }
}

void Client::OnClose()
{
    opened = false;

    level.clients[clientNum].ps.weapCommon.weapFlags &= ~(1 << 11); // Enable action slots
    level.clients[clientNum].ps.otherFlags &= ~(1 << 14); // Enable hud

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        // Fade out
        hudBackground->SetAlpha(0.0, 800);
        hudComponents[0]->SetAlpha(0.0, 800);
        hudTitle->SetAlpha(0.0, 800);
        hudAuthor->SetAlpha(0.0, 800);

        hudNavBar->SetAlpha(0.0, 400);
        hudCurrentMenu->SetAlpha(0.0, 400);
        for (auto option : hudOptions)
            option->SetAlpha(0.0, 400);

        // Slide out once the fading animation is done
        hudBackground->AddX(250.0, 400, 400);
        hudComponents[0]->AddX(250.0, 400, 400);
        hudTitle->AddX(250.0, 400, 400);
        hudAuthor->AddX(250.0, 400, 400);

        hudNavBar->AddX(250.0, 400, 400);
        hudCurrentMenu->AddX(250.0, 400, 400);
        for (auto option : hudOptions)
            option->AddX(250.0, 400, 400);

        InputSleep(800);
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        hudBackground->SetHeight(70, 400);
        hudComponents[0]->AddY(95.0, 400);
        hudComponents[1]->AddY(-95.0, 400);
        hudTitle->AddY(95.0, 400);
        hudAuthor->AddY(-95.0, 400);

        hudBackground->SetAlpha(0.0, 1, 400);
        hudComponents[0]->SetAlpha(0.0, 1, 400);
        hudComponents[1]->SetAlpha(0.0, 1, 400);
        hudNavBar->SetAlpha(0.0);
        hudTitle->SetAlpha(0.0, 1, 400);
        hudAuthor->SetAlpha(0.0, 1, 400);
        hudCurrentMenu->SetAlpha(0.0);

        for (auto& option : hudOptions)
            option->SetAlpha(0.0);

        InputSleep(400);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        hudBackground->SetAlpha(0.0, 400, 200);
        hudBackground->ScaleOvertime(0, 0, 400, 200);

        hudTitle->SetAlpha(0.0, 200);
        hudAuthor->SetAlpha(0.0, 200);
        hudCurrentMenu->SetAlpha(0.0, 200);

        for (auto& option : hudOptions)
            option->SetAlpha(0.0, 200);

        SetClientBlur(clientNum, 600, 0.0);
        InputSleep(600);
        break;
    }
    }
}

void Client::OnPress()
{
    if (!currentMenu->children[currentOption])
        return;

    auto callback = currentMenu->children[currentOption]->callback;
    if (callback)
    {
        callback(clientNum);
        InputSleep(200);
    }
    else if (currentMenu->children[currentOption]->GetChildCount() != 0)
    {
        auto* selectedOption = currentMenu->children[currentOption];

        previousMenu[submenuLevel] = currentMenu;
        previousOption[submenuLevel] = currentOption;
        previousOptionOffset[submenuLevel] = currentOptionOffset;
        currentOption = 0;
        currentOptionOffset = 0;
        submenuLevel++;

        ChangeSubmenu(selectedOption);
        InputSleep(200);
    }
}

void Client::OnCancel()
{
    if (currentMenu == rootMenu)
    {
        OnClose();
    }
    else
    {
        submenuLevel--;
        currentOption = previousOption[submenuLevel];
        currentOptionOffset = previousOptionOffset[submenuLevel];
        ChangeSubmenu(previousMenu[submenuLevel]);
        InputSleep(200);
    }
}

void Client::OnScrollUp()
{
    InputSleep(115);
    if (currentOption == 0)
        return;

    bool shouldMoveOptions = false;

    if (currentOptionOffset > 0 && currentOptionOffset == currentOption)
    {
        shouldMoveOptions = true;
        currentOptionOffset--;
    }
    currentOption--;

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        if (shouldMoveOptions)
        {
            auto* lastOption = hudOptions[ClientMaxViewableOptions - 1];

            for (int i = ClientMaxViewableOptions - 1; i > 0; i--)
            {
                hudOptions[i] = hudOptions[i - 1];
                hudOptions[i]->SetY(115.0 + (15.0 * i), 115);
            }

            hudOptions[0] = lastOption;
            lastOption->SetY(115.0);
            lastOption->SetText(currentMenu->children[currentOptionOffset]->text);
        }
        hudNavBar->SetY(114.0 + 15.0 * (currentOption - currentOptionOffset), 115);
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        if (shouldMoveOptions)
        {
            auto* lastOption = hudOptions[ClientMaxViewableOptions - 1];

            for (int i = ClientMaxViewableOptions - 1; i > 0; i--)
            {
                hudOptions[i] = hudOptions[i - 1];
                hudOptions[i]->SetY(menuPos.y - 45.0 + (15.0 * i), 115);
            }

            hudOptions[0] = lastOption;
            lastOption->SetY(menuPos.y - 45.0);
            lastOption->SetText(currentMenu->children[currentOptionOffset]->text);
        }
        hudNavBar->SetY(menuPos.y - 45.0 + (currentOption - currentOptionOffset) * 15.0, 115);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        if (shouldMoveOptions)
        {
            auto* lastOption = hudOptions[ClientMaxViewableOptions - 1];

            for (int i = ClientMaxViewableOptions - 1; i > 0; i--)
            {
                hudOptions[i] = hudOptions[i - 1];
                hudOptions[i]->SetY(-120.0 + (15.0 * i), 115);
            }

            hudOptions[0] = lastOption;
            lastOption->SetY(-120.0);
            lastOption->SetText(currentMenu->children[currentOptionOffset]->text);
        }

        auto* previousOption = hudOptions[currentOption - currentOptionOffset + 1];
        auto* newOption = hudOptions[currentOption - currentOptionOffset];

        previousOption->SetRGB({ 1.0, 1.0, 1.0 }, 200);
        previousOption->SetTextGlowAlpha(0.0);
        previousOption->SetTextScale(1.0, 200);

        newOption->SetRGB(menuColor, 200);
        newOption->SetTextGlowAlpha(1.0);
        newOption->SetTextScale(1.25, 200);
        break;
    }
    }
}

void Client::OnScrollDown()
{
    InputSleep(115);
    if (currentOption == currentMenu->GetChildCount() - 1)
        return;

    bool shouldMoveOptions = false;

    if (currentOption - currentOptionOffset == ClientMaxViewableOptions - 1)
    {
        shouldMoveOptions = true;
        currentOptionOffset++;
    }
    currentOption++;

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        if (shouldMoveOptions)
        {
            auto* firstOption = hudOptions[0];

            for (int i = 0; i < ClientMaxViewableOptions - 1; i++)
            {
                hudOptions[i] = hudOptions[i + 1];
                hudOptions[i]->SetY(115.0 + (15.0 * i), 115);
            }

            hudOptions[ClientMaxViewableOptions - 1] = firstOption;
            firstOption->SetY(115.0 + (15.0 * 9.0));
            firstOption->SetText(currentMenu->children[currentOptionOffset + ClientMaxViewableOptions - 1]->text);
        }
        hudNavBar->SetY(114.0 + 15.0 * (currentOption - currentOptionOffset), 115);
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        if (shouldMoveOptions)
        {
            auto* firstOption = hudOptions[0];

            for (int i = 0; i < ClientMaxViewableOptions - 1; i++)
            {
                hudOptions[i] = hudOptions[i + 1];
                hudOptions[i]->SetY(menuPos.y - 45.0 + (15.0 * i), 115);
            }

            hudOptions[ClientMaxViewableOptions - 1] = firstOption;
            firstOption->SetY(menuPos.y - 45.0 + (15.0 * 9.0));
            firstOption->SetText(currentMenu->children[currentOptionOffset + ClientMaxViewableOptions - 1]->text);
        }
        hudNavBar->SetY(menuPos.y - 45.0 + (currentOption - currentOptionOffset) * 15.0, 115);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        if (shouldMoveOptions)
        {
            auto* firstOption = hudOptions[0];

            for (int i = 0; i < ClientMaxViewableOptions - 1; i++)
            {
                hudOptions[i] = hudOptions[i + 1];
                hudOptions[i]->SetY(-120.0 + (15.0 * i), 115);
            }

            hudOptions[ClientMaxViewableOptions - 1] = firstOption;
            firstOption->SetY(-120.0 + (15.0 * 9.0));
            firstOption->SetText(currentMenu->children[currentOptionOffset + ClientMaxViewableOptions - 1]->text);
        }

        auto* previousOption = hudOptions[currentOption - currentOptionOffset - 1];
        auto* newOption = hudOptions[currentOption - currentOptionOffset];

        previousOption->SetRGB({ 1.0, 1.0, 1.0 }, 200);
        previousOption->SetTextGlowAlpha(0.0);
        previousOption->SetTextScale(1.0, 200);

        newOption->SetRGB(menuColor, 200);
        newOption->SetTextGlowAlpha(1.0);
        newOption->SetTextScale(1.25, 200);
        break;
    }
    }
}

void Client::SetTheme(ClientTheme theme)
{
    DestroyHudElems();
    currentTheme = theme;
    OnOpen();
}

void Client::SetColor(vec3_t color)
{
    menuColor = color;

    switch (currentTheme)
    {
    case(CLIENT_THEME_MATRIX):
    {
        hudComponents[0]->SetRGB(color, 150);
        hudNavBar->SetRGB(color, 150);
        hudAuthor->SetRGB(color, 150);
        hudTitle->SetTextGlow(color);
        break;
    }
    case(CLIENT_THEME_ENSTONE_SMALL):
    {
        hudComponents[0]->SetRGB(color, 150);
        hudComponents[1]->SetRGB(color, 150);
        hudNavBar->SetRGB(color, 150);
        break;
    }
    case(CLIENT_THEME_ENSTONE_LARGE):
    {
        for (auto& option : hudOptions)
            option->SetTextGlow(color, 0.0);

        hudOptions[currentOption - currentOptionOffset]->SetRGB(color, 150);
        hudOptions[currentOption - currentOptionOffset]->SetTextGlowAlpha(1.0);
        break;
    }
    }
}

void Client::Move(float xMove, float yMove)
{
    switch (currentTheme)
    {
    case(CLIENT_THEME_ENSTONE_SMALL):
    case(CLIENT_THEME_ENSTONE_SMALL2):
    {
        menuPos.x += xMove;
        menuPos.y += yMove;

        hudBackground->AddPosOvertime(xMove, yMove, 150);
        hudComponents[0]->AddPosOvertime(xMove, yMove, 150);
        hudComponents[1]->AddPosOvertime(xMove, yMove, 150);
        hudNavBar->AddPosOvertime(xMove, yMove, 150);
        hudTitle->AddPosOvertime(xMove, yMove, 150);
        hudAuthor->AddPosOvertime(xMove, yMove, 150);
        hudCurrentMenu->AddPosOvertime(xMove, yMove, 150);
        for (auto& option : hudOptions)
            option->AddPosOvertime(xMove, yMove, 150);

        GameMessage(clientNum, va("Menu is at: X=^2%.0f^7 Y=^2%.0f", menuPos.x, menuPos.y));
        break;
    }
    default:
        GameMessage(clientNum, "Not supported with the current theme");
        break;
    }
}

void Client::Run()
{
    if (!level.clients)
        return;

    HandleInput();
}

///
/// Game script functions
/// 
float* GetPlayerAngles(int clientNum)
{
    return level.clients[clientNum].ps.viewangles;
}

float* GetPlayerViewPos(int clientNum)
{
    static float s_viewPos[3];

    s_viewPos[0] = level.clients[clientNum].ps.origin[0];
    s_viewPos[1] = level.clients[clientNum].ps.origin[1];
    s_viewPos[2] = level.clients[clientNum].ps.origin[2] + level.clients[clientNum].ps.viewHeightCurrent;
    return s_viewPos;
}

float* AnglesToForward(float* angles, float scale)
{
    static float s_posForward[3];

    AngleVectors(angles, s_posForward, 0, 0);
    s_posForward[0] *= scale;
    s_posForward[1] *= scale;
    s_posForward[2] *= scale;
    return s_posForward;
}

float* BulletTrace(int entityNum, float* start, float* end)
{
    static float s_position[3];

    trace_t trace;
    G_LocationalTrace(&trace, start, end, entityNum, 0x802831, 0);
    s_position[0] = start[0] + (end[0] - start[0]) * trace.fraction;
    s_position[1] = start[1] + (end[1] - start[1]) * trace.fraction;
    s_position[2] = start[2] + (end[2] - start[2]) * trace.fraction;
    return s_position;
}

float* GetCrosshairPos(int clientNum)
{
    float* viewPos = GetPlayerViewPos(clientNum);

    float startPos[3];
    float* startForward = AnglesToForward(GetPlayerAngles(clientNum), 30.0);
    startPos[0] = viewPos[0] + startForward[0];
    startPos[1] = viewPos[1] + startForward[1];
    startPos[2] = viewPos[2] + startForward[2];

    float endPos[3];
    float* endForward = AnglesToForward(GetPlayerAngles(clientNum), 9999999.0);
    endPos[0] = viewPos[0] + endForward[0];
    endPos[1] = viewPos[1] + endForward[1];
    endPos[2] = viewPos[2] + endForward[2];

    return BulletTrace(level.gentities[clientNum].s.number, startPos, endPos);
}

void SetPosition(gentity_s* entity, float* position)
{
    if (entity->client)
        return SetClientOrigin(entity, position);

    entity->r.currentOrigin[0] = position[0];
    entity->r.currentOrigin[1] = position[1];
    entity->r.currentOrigin[2] = position[2];
    entity->s.lerp.pos.trBase[0] = position[0];
    entity->s.lerp.pos.trBase[1] = position[1];
    entity->s.lerp.pos.trBase[2] = position[2];
    entity->s.lerp.pos.trType = TR_STATIONARY;
    entity->s.lerp.pos.trTime = 0;
    entity->s.lerp.pos.trDuration = 0;
    entity->s.lerp.pos.trDelta[0] = 0.0;
    entity->s.lerp.pos.trDelta[1] = 0.0;
    entity->s.lerp.pos.trDelta[2] = 0.0;
}

void GameMessage(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("gm \"%s\"", text));
}

void GameMessageBold(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("gmb \"%s\"", text));
}

void PrintObjectifUpdate(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("obj_update \"%s\"", text));
}

void PrintObjectifComplete(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("obj_complete \"%s\"", text));
}

void PrintObjectifFailed(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("obj_failed \"%s\"", text));
}

void SetClientDvar(int clientNum, const char* dvar, const char* value)
{
    SV_GameSendServerCommand(clientNum, va("setclientdvar \"%s\" \"%s\"", dvar, value));
}

void SetClientBlur(int clientNum, int timeMs, float radius)
{
    SV_GameSendServerCommand(clientNum, va("scr_blur %i %f %i %i", timeMs, radius, BLUR_TIME_RELATIVE, BLUR_PRIORITY_SCRIPT));
}

void Scr_SetNumParams(int paramCount)
{
    scrVmPub.outparamcount = paramCount;
}

void Scr_ClearOutParams()
{
    while (scrVmPub.outparamcount)
    {
        RemoveRefToValue(scrVmPub.top->type, &scrVmPub.top->u);
        --scrVmPub.top;
        --scrVmPub.outparamcount;
    }
}

void Scr_MagicBullet(const char* weapon, float* start, float* end)
{
    Scr_AddVector(end);
    Scr_AddVector(start);
    Scr_AddString(weapon);
    Scr_SetNumParams(3);
    hook::invoke<void>(0x162A38);
    Scr_ClearOutParams();
}

bool HasPerk(int clientNum, int perkIndex)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    if (perkIndex == 21)
        return false;

    int perkBit = (1 << (perkIndex & 31));
    return (client->ps.perks[perkIndex >> 5] & perkBit) == perkBit;
}

void SetPerk(int clientNum, int perkIndex)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    if (perkIndex == 21)
        return;

    client->ps.perks[perkIndex >> 5] |= (1 << (perkIndex & 31));
}

void UnsetPerk(int clientNum, int perkIndex)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    if (perkIndex == 21)
        return;

    client->ps.perks[perkIndex >> 5] &= ~(1 << (perkIndex & 31));
}

Weapon GetCurrentWeapon(int clientNum)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    Weapon currentWeapon = client->ps.weapCommon.weapon.data ? client->ps.weapCommon.weapon : client->pers.cmd.weapon;

    if (client->ps.weapCommon.weapFlags & (1 << 7) || !currentWeapon.data)
        return Weapon();

    return currentWeapon;
}

int GetEquippedWeaponIndex(int clientNum, const Weapon weapon)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    for (int i = 0; i < 15; i++)
    {
        if (client->ps.weaponsEquipped[i].data != weapon.data)
            continue;

        return i;
    }
    return -1;
}

bool HasWeapon(int clientNum, const Weapon weapon)
{
    return GetEquippedWeaponIndex(clientNum, weapon) != -1;
}

void TakeWeapon(int clientNum, const Weapon weapon)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    BG_TakePlayerWeapon(&client->ps, weapon);
}

void GiveWeapon(int clientNum, const Weapon weapon)
{
    if (HasWeapon(clientNum, weapon))
        return;

    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    if (G_GivePlayerWeapon(&client->ps, weapon, 0))
    {
        Add_Ammo(entity, weapon, false, 999, 1);

        if (BG_HasUnderbarrelAmmo(weapon))
        {
            Add_Ammo(entity, weapon, true, 999, 1);
        }
    }
}

void SwitchToWeapon(int clientNum, const Weapon weapon, bool immediate)
{
    if (!HasWeapon(clientNum, weapon))
        return;

    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    if (immediate)
    {
        client->ps.weapCommon.weapon.data = 0;
        client->ps.weapCommon.weapFlags |= (1 << 19);
    }

    G_SelectWeapon(clientNum, weapon);
}

void ResetDvar(const char* name)
{
    dvar_t* dvar = Dvar_FindVar(name);

    if (dvar)
    {
        dvar->current = dvar->reset;
    }
}

void ToggleGodMode_f(int clientNum)
{
    const int bit = (1 << 0);
    int& value = level.clients[clientNum].ps.otherFlags;

    value ^= bit;

    GameMessage(clientNum, va("God mode: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleInfiniteAmmo_f(int clientNum)
{
    users[clientNum].infiniteAmmo ^= 1;

    GameMessage(clientNum, va("Infinite ammo: %s", users[clientNum].infiniteAmmo ? "^2On" : "^1Off"));
}

void ToggleNoSpread_f(int clientNum)
{
    const int bit = (1 << 1);
    int& value = level.clients[clientNum].ps.weapCommon.spreadOverrideState;

    value ^= bit;
    level.clients[clientNum].ps.weapCommon.spreadOverride = 0;

    GameMessage(clientNum, va("No spread: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleNoRecoil_f(int clientNum)
{
    const int bit = (1 << 10);
    int& value = level.clients[clientNum].ps.weapCommon.weapFlags;

    value ^= bit;
    level.clients[clientNum].ps.recoilScale = 0;

    GameMessage(clientNum, va("No recoil: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleFullAutoWeapons_f(int clientNum)
{
    users[clientNum].fullAutoWeapons ^= 1;

    GameMessage(clientNum, va("Full auto weapons: %s", users[clientNum].fullAutoWeapons ? "^2On" : "^1Off"));
}

void ToggleMovementSpeed_f(int clientNum)
{
    float& speedScale = level.clients[clientNum].pers.moveSpeedScaleMultiplier;

    speedScale = (speedScale == 2.0) ? 1.0 : 2.0;

    GameMessage(clientNum, va("Movement speed x2: %s", (speedScale == 2.0) ? "^2On" : "^1Off"));
}

void SelectFov_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* fovValue = currentOption->text;

    if (users[clientNum].currentOption == 0)
    {
        ResetDvar("cg_fov");
    }
    else
    {
        Dvar_SetFromStringByName("cg_fov", fovValue);
    }

    GameMessage(clientNum, va("Fov set to: ^2%s", fovValue));
}

void ToggleNoclip_f(int clientNum)
{
    users[clientNum].noclip ^= 1;

    level.clients[clientNum].ps.weapCommon.weapFlags &= ~(1 << 7);

    if (users[clientNum].noclip)
    {
        users[clientNum].ufo = false;

        if (!users[clientNum].noclipBind)
            level.clients[clientNum].ps.weapCommon.weapFlags |= (1 << 7);
    }

    GameMessage(clientNum, va("Noclip: %s", users[clientNum].noclip ? "^2On" : "^1Off"));
}

void ToggleUfoMode_f(int clientNum)
{
    users[clientNum].ufo ^= 1;

    level.clients[clientNum].ps.weapCommon.weapFlags &= ~(1 << 7);

    if (users[clientNum].ufo)
    {
        users[clientNum].noclip = false;
        level.clients[clientNum].ps.weapCommon.weapFlags |= (1 << 7);
    }

    GameMessage(clientNum, va("UFO mode: %s", users[clientNum].ufo ? "^2On" : "^1Off"));
}

void ToggleNoclipBind_f(int clientNum)
{
    users[clientNum].noclipBind ^= 1;

    GameMessage(clientNum, va("Noclip: %s", users[clientNum].noclipBind ? "^2bound to \x12" : "^1unbound"));
}

void ToggleRocketRide_f(int clientNum)
{
    users[clientNum].rocketRide ^= 1;

    GameMessage(clientNum, va("Rocket ride: %s", users[clientNum].rocketRide ? "^2On" : "^1Off"));
}

void ToggleRocketJump_f(int clientNum)
{
    users[clientNum].rocketJump ^= 1;

    GameMessage(clientNum, va("Rocket jump: %s", users[clientNum].rocketJump ? "^2On" : "^1Off"));
}

void ToggleRocketJumpStrength_f(int clientNum)
{
    users[clientNum].rocketJumpStrength += 256.0;

    if (users[clientNum].rocketJumpStrength > 1024.0)
        users[clientNum].rocketJumpStrength = 256.0;

    GameMessage(clientNum, va("Rocket jump strength set to: ^2%.0f", users[clientNum].rocketJumpStrength));
}

void SetAllPerks_f(int clientNum)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    client->ps.perks[0] = 0;
    client->ps.perks[1] = 0;
    for (int slot = 0; slot < 9; slot++)
        client->ps.perkSlots[slot] = 0;

    for (int perk = 0; perk < 21; perk++)
        SetPerk(clientNum, perk);

    GameMessage(clientNum, "All perks: ^2set");
}

void ClearAllPerks_f(int clientNum)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    client->ps.perks[0] = 0;
    client->ps.perks[1] = 0;
    for (int i = 0; i < 9; i++)
        client->ps.perkSlots[i] = 0;

    GameMessage(clientNum, "All perks: ^2cleared");
}

void SelectPerk_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* perkName = currentOption->text;

    int perkIndex = BG_GetPerkCodeIndexForName(perkName);

    (HasPerk(clientNum, perkIndex) ? UnsetPerk : SetPerk)(clientNum, perkIndex);

    GameMessage(clientNum, va("%s: ^2%s", HasPerk(clientNum, perkIndex) ? "Set perk" : "Removed perk", perkName));
}

void SelectWeapon_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* weaponName = currentOption->text;

    Weapon weapon;
    G_GetWeaponForName(&weapon, weaponName);

    if (HasWeapon(clientNum, weapon))
        return GameMessage(clientNum, "^1Error^7: You already have this weapon");

    TakeWeapon(clientNum, GetCurrentWeapon(clientNum));
    GiveWeapon(clientNum, weapon);
    SwitchToWeapon(clientNum, weapon, false);
    GameMessage(clientNum, va("Weapon given: ^2%s", weaponName));
}

void RefillAmmo_f(int clientNum)
{
    gentity_s* entity = &level.gentities[clientNum];
    Weapon weapon = GetCurrentWeapon(clientNum);

    if (weapon.data)
    {
        Add_Ammo(entity, weapon, false, 999, 1);

        if (BG_HasUnderbarrelAmmo(weapon))
        {
            Add_Ammo(entity, weapon, true, 999, 1);
        }
    }
    GameMessage(clientNum, "Ammo: ^2refilled");
}

void TakeCurrentWeapon_f(int clientNum)
{
    TakeWeapon(clientNum, GetCurrentWeapon(clientNum));
    GameMessage(clientNum, "Current weapon: ^2taken");
}

void ResetProjectile_f(int clientNum)
{
    users[clientNum].projectile = 0;
    GameMessage(clientNum, "Projectile set to: ^2None");
}

void SetProjectile_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* projectileName = currentOption->text;

    users[clientNum].projectile = projectileName;
    GameMessage(clientNum, va("Projectile set to: ^2%s", projectileName));
}

void SelectVision_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* visionName = currentOption->text;

    SV_GameSendServerCommand(clientNum, va("visionset_naked \"%s\" 200", visionName));
    GameMessage(clientNum, va("Vision set: ^2%s", visionName));
}

void SavePosition_f(int clientNum)
{
    users[clientNum].savedPosition[0] = level.clients[clientNum].ps.origin[0];
    users[clientNum].savedPosition[1] = level.clients[clientNum].ps.origin[1];
    users[clientNum].savedPosition[2] = level.clients[clientNum].ps.origin[2];
    users[clientNum].positionSaved = true;

    GameMessage(clientNum, "Position: ^2Saved!");
}

void LoadPosition_f(int clientNum)
{
    if (!users[clientNum].positionSaved)
        return GameMessage(clientNum, "^1Error^7: Position wasn't saved!");

    SetPosition(&level.gentities[clientNum], users[clientNum].savedPosition);
    GameMessage(clientNum, "Position: ^2Loaded!");
}

void TeleportToCrosshair_f(int clientNum)
{
    SetPosition(&level.gentities[clientNum], GetCrosshairPos(clientNum));
    GameMessage(clientNum, "Teleported to crosshair!");
}

void ToggleTeleportGun_f(int clientNum)
{
    users[clientNum].teleportGun ^= 1;

    GameMessage(clientNum, va("Teleport gun: %s", users[clientNum].teleportGun ? "^2On" : "^1Off"));
}

void SelectTimeScale_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* timeScale = currentOption->text;

    Dvar_SetFromStringByName("com_timescale", timeScale + 1); // Skip the 'x'
    GameMessage(clientNum, va("Time scale set to: ^2%s", timeScale));
}

void SelectPlayerSpeed_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* playerSpeedValue = currentOption->text;

    if (users[clientNum].currentOption == 0)
    {
        ResetDvar("g_speed");
    }
    else
    {
        Dvar_SetFromStringByName("g_speed", playerSpeedValue);
    }

    GameMessage(clientNum, va("Player speed set to: ^2%s", playerSpeedValue));
}

void SelectJumpHeight_f(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* jumpHeightValue = currentOption->text;

    if (users[clientNum].currentOption == 0)
    {
        ResetDvar("jump_height");
    }
    else
    {
        Dvar_SetFromStringByName("jump_height", jumpHeightValue);
    }

    GameMessage(clientNum, va("Jump height set to: ^2%s", jumpHeightValue));
}

void SetThemeMatrix_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_MATRIX);
    GameMessage(clientNum, va("Theme set: ^2Matrix"));
}

void SetThemeClassic_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_CLASSIC);
    GameMessage(clientNum, va("Theme set: ^2Classic"));
}

void SetThemeEnstoneSmall_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_ENSTONE_SMALL);
    GameMessage(clientNum, va("Theme set: ^2Enstone small"));
}

void SetThemeEnstoneLarge_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_ENSTONE_LARGE);
    GameMessage(clientNum, va("Theme set: ^2Enstone large"));
}

void SetThemeEnstoneRight_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_ENSTONE_RIGHT);
    GameMessage(clientNum, va("Theme set: ^2Enstone right"));
}

void SetThemeEnstoneSmall2_f(int clientNum)
{
    users[clientNum].SetTheme(CLIENT_THEME_ENSTONE_SMALL2);
    GameMessage(clientNum, va("Theme set: ^2Enstone small2"));
}

void ThemeColorRed_f(int clientNum)
{
    users[clientNum].SetColor({ 0.9, 0.1, 0.01 });
    GameMessage(clientNum, "Theme color: ^1red");
}

void ThemeColorGreen_f(int clientNum)
{
    users[clientNum].SetColor({ 0.008, 0.5, 0.2 });
    GameMessage(clientNum, "Theme color: ^2green");
}

void ThemeColorBlue_f(int clientNum)
{
    users[clientNum].SetColor({ 0.0, 0.0, 1.0 });
    GameMessage(clientNum, "Theme color: ^4blue");
}

void ThemeColorYellow_f(int clientNum)
{
    users[clientNum].SetColor({ 1.0, 1.0, 0.0 });
    GameMessage(clientNum, "Theme color: ^3yellow");
}

void MoveMenuUp_f(int clientNum)
{
    users[clientNum].Move(0.0, -10.0);
}

void MoveMenuDown_f(int clientNum)
{
    users[clientNum].Move(0.0, 10.0);
}

void MoveMenuLeft_f(int clientNum)
{
    users[clientNum].Move(-10.0, 0.0);
}

void MoveMenuRight_f(int clientNum)
{
    users[clientNum].Move(10.0, 0.0);
}

///
/// Hooks
///
__attribute__((naked, noinline)) void ClientCommand_Original(int clientNum, const char* s)
{
    asm
    (
        "stdu      %r1, -0x590(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x5A0(%r1);"
        "std       %r31, 0x588(%r1);"
        "lis       %r11, 0x13;"
        "ori       %r11, %r11, 0xEC94;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void ClientCommand_Hook(int clientNum, const char* s)
{
    Client& user = users[clientNum & 1];

    if (_sys_strncmp(s, "n ", 2) == 0)
    {
        int key = atoi(s + 2);
        // debug_print("Key: %i\n", key);

        switch (key)
        {
        case(1): user.buttonBits |= CLIENT_BTN_R1; break;
        case(2): user.buttonBits &= ~CLIENT_BTN_R1; break;

        case(5): user.buttonBits |= CLIENT_BTN_R2; break;
        case(6): user.buttonBits &= ~CLIENT_BTN_R2; break;

        case(7): user.buttonBits |= CLIENT_BTN_L2; break;
        case(8): user.buttonBits &= ~CLIENT_BTN_L2; break;

        case(9): user.buttonBits |= CLIENT_BTN_L3; break;
        case(10): user.buttonBits &= ~CLIENT_BTN_L3; break;

        case(11): user.buttonBits |= CLIENT_BTN_SQUARE; break;
        case(12): user.buttonBits &= ~CLIENT_BTN_SQUARE; break;

        case(13): user.buttonBits |= CLIENT_BTN_L1; break;
        case(14): user.buttonBits &= ~CLIENT_BTN_L1; break;

        case(15): user.buttonBits |= CLIENT_BTN_DPAD_UP; break;
        case(16): user.buttonBits &= ~CLIENT_BTN_DPAD_UP; break;

        case(17): user.buttonBits |= CLIENT_BTN_DPAD_DOWN; break;
        case(18): user.buttonBits &= ~CLIENT_BTN_DPAD_DOWN; break;

        case(19): user.buttonBits |= CLIENT_BTN_DPAD_LEFT; break;
        case(20): user.buttonBits &= ~CLIENT_BTN_DPAD_LEFT; break;

        case(21): user.buttonBits |= CLIENT_BTN_DPAD_RIGHT; break;
        case(22): user.buttonBits &= ~CLIENT_BTN_DPAD_RIGHT; break;

        case(23): user.buttonBits |= CLIENT_BTN_CIRCLE; break;
        case(24): user.buttonBits &= ~CLIENT_BTN_CIRCLE; break;

        case(25): user.buttonBits |= CLIENT_BTN_CROSS; break;
        case(26): user.buttonBits &= ~CLIENT_BTN_CROSS; break;

        case(27): user.buttonBits |= CLIENT_BTN_R3; break;
        case(28): user.buttonBits &= ~CLIENT_BTN_R3; break;
        }
    }

    ClientCommand_Original(clientNum, s);
}

__attribute__((naked, noinline)) void ClientThink_real_Original(gentity_s* ent)
{
    asm
    (
        "stdu      %r1, -0x230(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x240(%r1);"
        "std       %r25, 0x1F8(%r1);"
        "lis       %r11, 0x12;"
        "ori       %r11, %r11, 0xF718;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void ClientThink_real_Hook(gentity_s* ent)
{
    if (!CL_IsLocalClientInGame(0))
    {
        debug_print("Destroying all menus\n");
        for (auto& user : users)
            user.ClearAll();
    }
    else if (ent && ent->client)
    {
        gclient_s* client = ent->client;
        Client& user = users[client->ps.clientNum & 1];

        user.Run();
    }

    ClientThink_real_Original(ent);
}

__attribute__((naked, noinline)) void VM_Notify_Original(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue* top)
{
    asm
    (
        "stdu      %r1, -0x120(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x130(%r1);"
        "std       %r17, 0xA8(%r1);"
        "lis       %r11, 0x23;"
        "ori       %r11, %r11, 0x14C8;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void VM_Notify_Hook(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue* top)
{
    int entityNum = Scr_GetSelf(notifyListOwnerId);

    if (level.gentities && entityNum < 2)
    {
        gentity_s* entity = &level.gentities[entityNum];
        gclient_s* client = entity->client;

        if (client)
        {
            int clientNum = client->ps.clientNum;
            const char* notify = SL_ConvertToString(stringValue);

            debug_print("VM_Notify: client %i '%s'\n", clientNum, notify);

            if (_sys_strcmp(notify, "weapon_fired") == 0)
            {
                if (users[clientNum].projectile)
                {
                    float* viewPos = GetPlayerViewPos(clientNum);

                    float startPos[3];
                    float* startForward = AnglesToForward(GetPlayerAngles(clientNum), 30.0);
                    startPos[0] = viewPos[0] + startForward[0];
                    startPos[1] = viewPos[1] + startForward[1];
                    startPos[2] = viewPos[2] + startForward[2];

                    float endPos[3];
                    float* endForward = AnglesToForward(GetPlayerAngles(clientNum), 9999999.0);
                    endPos[0] = viewPos[0] + endForward[0];
                    endPos[1] = viewPos[1] + endForward[1];
                    endPos[2] = viewPos[2] + endForward[2];

                    float* tracePos = BulletTrace(entity->s.number, startPos, endPos);
                    Scr_MagicBullet(users[clientNum].projectile, startPos, tracePos);
                }

                if (users[clientNum].teleportGun)
                {
                    TeleportToCrosshair_f(clientNum);
                }
            }

            if (_sys_strcmp(notify, "missile_fire") == 0)
            {
                if (users[clientNum].rocketRide)
                {
                    gentity_s* rocket = &level.gentities[Scr_GetSelf(top->u.intValue)];

                    if (G_EntLinkTo(entity, rocket, 0))
                        G_InitPlayerLinkAngles(entity);
                }
            }
        }
    }
    else
    {
        //printf("VM_Notify: notifyListOwnerId=%i '%s'\n", notifyListOwnerId, SL_ConvertToString(stringValue));
    }

    VM_Notify_Original(notifyListOwnerId, stringValue, top);
}

__attribute__((naked, noinline)) void PM_WeaponUseAmmo_Original(playerState_s* ps, const Weapon* weapon, bool isAlternate, int amount, PlayerHandIndex hand)
{
    asm
    (
        "stdu      %r1, -0x90(%r1);"
        "mflr      %r0;"
        "std       %r0, 0xA0(%r1);"
        "std       %r29, 0x78(%r1);"
        "lis       %r11, 0x2B;"
        "ori       %r11, %r11, 0xADB4;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void PM_WeaponUseAmmo_Hook(playerState_s* ps, const Weapon* weapon, bool isAlternate, int amount, PlayerHandIndex hand)
{
    if (users[ps->clientNum].infiniteAmmo)
        return;

    PM_WeaponUseAmmo_Original(ps, weapon, isAlternate, amount, hand);
}

__attribute__((naked, noinline)) gentity_s* Weapon_RocketLauncher_Fire_Original(gentity_s* ent, const Weapon weapon, float spread, weaponParms* wp, const float* gunVel, missileFireParms* fireParms, missileFireParms* magicBullet)
{
    asm
    (
        "stdu      %r1, -0xF0(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x100(%r1);"
        "stfd      %f27, 0xC8(%r1);"
        "lis       %r11, 0x18;"
        "ori       %r11, %r11, 0xD940;"
        "mtctr     %r11;"
        "bctr;"
    );
    return 0;
}

gentity_s* Weapon_RocketLauncher_Fire_Hook(gentity_s* ent, const Weapon weapon, float spread, weaponParms* wp, const float* gunVel, missileFireParms* fireParms, missileFireParms* magicBullet)
{
    gentity_s* result = Weapon_RocketLauncher_Fire_Original(ent, weapon, spread, wp, gunVel, fireParms, magicBullet);

    if (!ent->client)
        return result;

    int clientNum = ent->client->ps.clientNum;

    if (users[clientNum].rocketJump)
    {
        ent->client->ps.velocity[0] -= wp->forward[0] * users[clientNum].rocketJumpStrength;
        ent->client->ps.velocity[1] -= wp->forward[1] * users[clientNum].rocketJumpStrength;
        ent->client->ps.velocity[2] -= wp->forward[2] * users[clientNum].rocketJumpStrength;
    }

    return result;
}

__attribute__((naked, noinline)) void PmoveSingle_Original(pmove_t* pm)
{
    asm
    (
        "stdu      %r1, -0x220(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x230(%r1);"
        "stfd      %f26, 0x1F0(%r1);"
        "lis       %r11, 0x2A;"
        "ori       %r11, %r11, 0xB650;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void PmoveSingle_Hook(pmove_t* pm)
{
    if (users[pm->ps->clientNum].noclip || users[pm->ps->clientNum].ufo)
    {
        float forwardMove = static_cast<float>(pm->cmd.forwardmove) / 128.0;
        float rightMove = static_cast<float>(pm->cmd.rightmove) / 128.0;
        float upMove = 0.0;

        if (pm->cmd.buttons & (1 << 14))
            upMove += 1.0;
        if (pm->cmd.buttons & (1 << 15))
            upMove -= 1.0;

        float speed = pm->cmd.buttons & (1 << 1) ? 20.0 : 10.0;
        forwardMove *= speed;
        rightMove *= speed;
        upMove *= speed;

        if (users[pm->ps->clientNum].noclip)
        {
            float forward[3], right[3];
            AngleVectors(pm->ps->viewangles, forward, right, 0);

            pm->ps->velocity[0] = forward[0] * forwardMove + right[0] * rightMove;
            pm->ps->velocity[1] = forward[1] * forwardMove + right[1] * rightMove;
            pm->ps->velocity[2] = forward[2] * forwardMove + right[2] * rightMove;
            pm->ps->origin[0] += pm->ps->velocity[0];
            pm->ps->origin[1] += pm->ps->velocity[1];
            pm->ps->origin[2] += pm->ps->velocity[2];
        }
        else if (users[pm->ps->clientNum].ufo)
        {
            float forward[3], right[3], up[3];
            AngleVectors(pm->ps->viewangles, forward, right, up);

            pm->ps->velocity[0] = forward[0] * forwardMove + right[0] * rightMove;
            pm->ps->velocity[1] = forward[1] * forwardMove + right[1] * rightMove;
            pm->ps->velocity[2] = up[2] * upMove;
            pm->ps->origin[0] += pm->ps->velocity[0];
            pm->ps->origin[1] += pm->ps->velocity[1];
            pm->ps->origin[2] += pm->ps->velocity[2];
        }
    }
    

    PmoveSingle_Original(pm);
}

__attribute__((naked, noinline)) void PM_WalkMove_Original(pmove_t* pm, pml_t* pml)
{
    asm
    (
        "stdu      %r1, -0x140(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x150(%r1);"
        "stfd      %f26, 0x110(%r1);"
        "lis       %r11, 0x2A;"
        "ori       %r11, %r11, 0xAE38;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void PM_WalkMove_Hook(pmove_t* pm, pml_t* pml)
{
    if (users[pm->ps->clientNum].noclip || users[pm->ps->clientNum].ufo)
        return;

    PM_WalkMove_Original(pm, pml);
}

__attribute__((naked, noinline)) void PM_AirMove_Original(pmove_t* pm, pml_t* pml)
{
    asm
    (
        "stdu      %r1, -0x180(%r1);"
        "mflr      %r0;"
        "std       %r0, 0x190(%r1);"
        "stfd      %f30, 0x170(%r1);"
        "lis       %r11, 0x2A;"
        "ori       %r11, %r11, 0xA4D8;"
        "mtctr     %r11;"
        "bctr;"
    );
}

void PM_AirMove_Hook(pmove_t* pm, pml_t* pml)
{
    if (users[pm->ps->clientNum].noclip || users[pm->ps->clientNum].ufo)
        return;

    PM_AirMove_Original(pm, pml);
}

void PM_Weapon_StartFiring_Hook(HookContext_t& ctx)
{
    playerState_s* ps = reinterpret_cast<playerState_s*>(static_cast<uintptr_t>(ctx.r31));

    if (ctx.r3 == WEAPON_FIRETYPE_FULLAUTO || users[ps->clientNum].fullAutoWeapons)
    {
        ctx.lr = 0x2B7480;
    }
}

namespace onHost
{
    void start()
    {
        for (int i = 0; i < 2; i++)
        {
            users[i].ClearAll();
            users[i].clientNum = i;
            users[i].currentTheme = CLIENT_THEME_MATRIX;
        }

        hook::jump(0x13EC84, *reinterpret_cast<uintptr_t*>(ClientCommand_Hook));
        hook::jump(0x12F708, *reinterpret_cast<uintptr_t*>(ClientThink_real_Hook));
        hook::jump(0x2314B8, *reinterpret_cast<uintptr_t*>(VM_Notify_Hook));
        hook::jump(0x2BADA4, *reinterpret_cast<uintptr_t*>(PM_WeaponUseAmmo_Hook));
        hook::jump(0x18D930, *reinterpret_cast<uintptr_t*>(Weapon_RocketLauncher_Fire_Hook));
        hook::jump(0x2AB640, *reinterpret_cast<uintptr_t*>(PmoveSingle_Hook));
        hook::jump(0x2AAE28, *reinterpret_cast<uintptr_t*>(PM_WalkMove_Hook));
        hook::jump(0x2AA4C8, *reinterpret_cast<uintptr_t*>(PM_AirMove_Hook));

        hook::inject(0x2B7438, PM_Weapon_StartFiring_Hook);
    }

    void stop()
    {
        hook::copy(0x13EC84, *reinterpret_cast<uint32_t**>(ClientCommand_Original), 4);
        hook::copy(0x12F708, *reinterpret_cast<uint32_t**>(ClientThink_real_Original), 4);
        hook::copy(0x2314B8, *reinterpret_cast<uint32_t**>(VM_Notify_Original), 4);
        hook::copy(0x2BADA4, *reinterpret_cast<uint32_t**>(PM_WeaponUseAmmo_Original), 4);
        hook::copy(0x18D930, *reinterpret_cast<uint32_t**>(Weapon_RocketLauncher_Fire_Original), 4);
        hook::copy(0x2AB640, *reinterpret_cast<uint32_t**>(PmoveSingle_Original), 4);
        hook::copy(0x2AAE28, *reinterpret_cast<uint32_t**>(PM_WalkMove_Original), 4);
        hook::copy(0x2AA4C8, *reinterpret_cast<uint32_t**>(PM_AirMove_Original), 4);

        for (auto& user : users)
            user.ClearAll();
    }
}