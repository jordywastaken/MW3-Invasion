
#include "clients.hpp"
#include "scheduler.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/imports.hpp"

Client users[2];

///
/// float scaleVirtualToReal[2]{ 1.5, 2.25 };
/// float scaleVirtualToFull[2]{ 2.0, 2.25 };
/// float scaleRealToVirtual[2]{ 0.6666666666, 0.4444444444 };
/// 

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
    mainModsMenu->AddChild("God mode", ToggleGodMode);
    mainModsMenu->AddChild("Infinite ammo", ToggleInfiniteAmmo);
    mainModsMenu->AddChild("No spread", ToggleNoSpread);
    mainModsMenu->AddChild("No recoil", ToggleNoRecoil);
    mainModsMenu->AddChild("Full auto weapons", ToggleFullAutoWeapons);
    mainModsMenu->AddChild("Movement speed x2", ToggleMovementSpeed);

    // For now only the host can edit client side dvars
    if (clientNum == 0)
    {
        auto* fovMenu = MakeSubmenu("Fov menu");
        fovMenu->AddChild("65 (Default)", SelectFov);
        fovMenu->AddChild("80", SelectFov);
        fovMenu->AddChild("90", SelectFov);
        fovMenu->AddChild("100", SelectFov);
        fovMenu->AddChild("110", SelectFov);
        fovMenu->AddChild("120", SelectFov);
        mainModsMenu->AddChild(fovMenu);
    }

    auto* movementTypeMenu = MakeSubmenu("Movement type menu");
    movementTypeMenu->AddChild("Noclip", ToggleNoclip);
    movementTypeMenu->AddChild("UFO mode", ToggleUfoMode);
    movementTypeMenu->AddChild("Bind noclip to \x12", ToggleNoclipBind);
    mainModsMenu->AddChild(movementTypeMenu);

    mainMenu->AddChild(mainModsMenu);

    /// 
    /// Fun menu
    /// 
    auto* funMenu = MakeSubmenu("Fun menu");
    funMenu->AddChild("Rocket ride", ToggleRocketRide);
    funMenu->AddChild("Rocket jump", ToggleRocketJump);
    funMenu->AddChild("Rocket jump strength", ToggleRocketJumpStrength);
    mainMenu->AddChild(funMenu);

    /// 
    /// Perks menu
    /// 
    auto* perksMenu = MakeSubmenu("Perks menu");
    perksMenu->AddChild("Set all perks", SetAllPerks);
    perksMenu->AddChild("Clear all perks", ClearAllPerks);
    mainMenu->AddChild(perksMenu);

    for (int i = 0; i < 21; i++)
        perksMenu->AddChild(bg_perkNames[i], SelectPerk);

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
    mainMenu->AddChild(weaponMenu);

    auto* projectileMenu = MakeSubmenu("Projectile menu");
    projectileMenu->AddChild("None", ResetProjectile);
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
            weaponMenu->children[childIndex]->AddChild(bg_weaponCompleteDefs[i]->szInternalName, SelectWeapon);
        }

        if (bg_weaponCompleteDefs[i]->weapDef->weapType == WEAPTYPE_PROJECTILE)
        {
            projectileMenu->AddChild(bg_weaponCompleteDefs[i]->szInternalName, SetProjectile);
        }
    }

    ///
    /// Vision menu
    /// 
    auto* visionMenu = MakeSubmenu("Vision menu");
    visionMenu->AddChild("default", SelectVision);
    visionMenu->AddChild("default_night", SelectVision);
    visionMenu->AddChild("ac130", SelectVision);
    visionMenu->AddChild("ac130_inverted", SelectVision);
    visionMenu->AddChild("cheat_bw", SelectVision);
    visionMenu->AddChild("cheat_bw_contrast", SelectVision);
    visionMenu->AddChild("cheat_bw_invert", SelectVision);
    visionMenu->AddChild("cheat_bw_invert_contrast", SelectVision);
    visionMenu->AddChild("cheat_chaplinnight", SelectVision);
    visionMenu->AddChild("cheat_contrast", SelectVision);
    visionMenu->AddChild("cheat_invert", SelectVision);
    visionMenu->AddChild("cheat_invert_contrast", SelectVision);
    visionMenu->AddChild("end_game", SelectVision);
    visionMenu->AddChild("icbm", SelectVision);
    visionMenu->AddChild("invasion", SelectVision);
    visionMenu->AddChild("mpnuke", SelectVision);
    visionMenu->AddChild("mpnuke_aftermath", SelectVision);
    visionMenu->AddChild("near_death", SelectVision);
    visionMenu->AddChild("sepia", SelectVision);
    mainMenu->AddChild(visionMenu);

    /// 
    /// Teleport menu
    /// 
    auto* teleportMenu = MakeSubmenu("Teleport menu");
    teleportMenu->AddChild("Save position", SavePosition);
    teleportMenu->AddChild("Load position", LoadPosition);
    teleportMenu->AddChild("Teleport to crosshair", TeleportToCrosshair);
    teleportMenu->AddChild("Teleport gun", ToggleTeleportGun);
    mainMenu->AddChild(teleportMenu);

    /// 
    /// Lobby menu
    /// 
    auto* lobbyMenu = MakeSubmenu("Lobby menu");
    auto* timeScaleMenu = MakeSubmenu("Time scale menu");
    timeScaleMenu->AddChild("x0.1", SelectTimeScale);
    timeScaleMenu->AddChild("x0.25", SelectTimeScale);
    timeScaleMenu->AddChild("x0.5", SelectTimeScale);
    timeScaleMenu->AddChild("x0.75", SelectTimeScale);
    timeScaleMenu->AddChild("x1", SelectTimeScale);
    timeScaleMenu->AddChild("x2", SelectTimeScale);
    lobbyMenu->AddChild(timeScaleMenu);

    auto* playerSpeedMenu = MakeSubmenu("Player speed menu");
    playerSpeedMenu->AddChild("190 (Default)", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("250", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("400", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("500", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("600", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("700", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("800", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("900", SelectPlayerSpeed);
    playerSpeedMenu->AddChild("1000", SelectPlayerSpeed);
    lobbyMenu->AddChild(playerSpeedMenu);

    auto* jumpHeightMenu = MakeSubmenu("Jump height menu");
    jumpHeightMenu->AddChild("39 (Default)", SelectJumpHeight);
    jumpHeightMenu->AddChild("50", SelectJumpHeight);
    jumpHeightMenu->AddChild("100", SelectJumpHeight);
    jumpHeightMenu->AddChild("150", SelectJumpHeight);
    jumpHeightMenu->AddChild("250", SelectJumpHeight);
    jumpHeightMenu->AddChild("500", SelectJumpHeight);
    jumpHeightMenu->AddChild("1000", SelectJumpHeight);
    lobbyMenu->AddChild(jumpHeightMenu);

    mainMenu->AddChild(lobbyMenu);

    /// 
    /// Settings menu
    /// 
    auto* settingsMenu = MakeSubmenu("Settings menu");
    settingsMenu->AddChild("Theme color red", ThemeColorRed);
    settingsMenu->AddChild("Theme color green", ThemeColorGreen);
    settingsMenu->AddChild("Theme color blue", ThemeColorBlue);
    settingsMenu->AddChild("Theme color yellow", ThemeColorYellow);
    mainMenu->AddChild(settingsMenu);

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

        hudNavBar = CreateMaterialHudElem(clientNum, "white", 250, 15, 67.0, 114.0, 1.0, menuColor, 0.0);
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

        hudCurrentMenu = CreateTextHudElem(clientNum, "Main menu", 1.6, 75.0, 85.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudCurrentMenu->SetHorzAlign(HE_HORZALIGN_RIGHT);

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i] = CreateTextHudElem(clientNum, currentMenu->children[i] ? currentMenu->children[i]->text : "", 1.0, 75.0, 115.0 + (15.0 * i), 2.0, { 1.0, 1.0, 1.0 }, 0.0);
            hudOptions[i]->SetHorzAlign(HE_HORZALIGN_RIGHT);
        }
        break;
    }
    }
}

void Client::DestroyHudElems()
{
    if (!hudBackground)
        return;

    delete hudBackground;
    delete hudComponents[0];
    delete hudComponents[1];
    delete hudNavBar;
    delete hudTitle;
    delete hudAuthor;
    delete hudCurrentMenu;

    for(int i = 0; i < ClientMaxViewableOptions; i++)
        delete hudOptions[i];
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
        ToggleNoclip(clientNum);

    // Handle bind released
    if (noclipBind && !(buttonBits & CLIENT_BTN_L2) && noclip)
        ToggleNoclip(clientNum);

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

        for (int i = 0; i < ClientMaxViewableOptions; i++)
        {
            hudOptions[i]->SetText(submenu->children[i] ? submenu->children[i]->text : "");
            hudOptions[i]->SetAlpha(0.0);
            hudOptions[i]->SetAlpha(1.0, 400);
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

        InputSleep(800);
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
        previousMenu[submenuLevel] = currentMenu;
        previousOption[submenuLevel] = currentOption;
        previousOptionOffset[submenuLevel] = currentOptionOffset;
        currentOption = 0;
        currentOptionOffset = 0;
        submenuLevel++;
        ChangeSubmenu(currentMenu->children[currentOption]);

        switch (currentTheme)
        {
        case(CLIENT_THEME_MATRIX):
        {
            hudNavBar->SetY(114.0 + 15.0 * currentOption);
            InputSleep(400);
            break;
        }
        }
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

        switch (currentTheme)
        {
        case(CLIENT_THEME_MATRIX):
        {
            hudNavBar->SetY(114.0 + 15.0 * (currentOption - currentOptionOffset));
            InputSleep(400);
            break;
        }
        }
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
            firstOption->SetY(115.0 + (15.0 * 9));
            firstOption->SetText(currentMenu->children[currentOptionOffset + ClientMaxViewableOptions - 1]->text);
        }
        hudNavBar->SetY(114.0 + 15.0 * (currentOption - currentOptionOffset), 115);
        break;
    }
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

void ToggleGodMode(int clientNum)
{
    const int bit = (1 << 0);
    int& value = level.clients[clientNum].ps.otherFlags;

    value ^= bit;

    GameMessage(clientNum, va("God mode: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleInfiniteAmmo(int clientNum)
{
    users[clientNum].infiniteAmmo ^= 1;

    GameMessage(clientNum, va("Infinite ammo: %s", users[clientNum].infiniteAmmo ? "^2On" : "^1Off"));
}

void ToggleNoSpread(int clientNum)
{
    const int bit = (1 << 1);
    int& value = level.clients[clientNum].ps.weapCommon.spreadOverrideState;

    value ^= bit;
    level.clients[clientNum].ps.weapCommon.spreadOverride = 0;

    GameMessage(clientNum, va("No spread: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleNoRecoil(int clientNum)
{
    const int bit = (1 << 10);
    int& value = level.clients[clientNum].ps.weapCommon.weapFlags;

    value ^= bit;
    level.clients[clientNum].ps.recoilScale = 0;

    GameMessage(clientNum, va("No recoil: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleFullAutoWeapons(int clientNum)
{
    users[clientNum].fullAutoWeapons ^= 1;

    GameMessage(clientNum, va("Full auto weapons: %s", users[clientNum].fullAutoWeapons ? "^2On" : "^1Off"));
}

void ToggleMovementSpeed(int clientNum)
{
    float& speedScale = level.clients[clientNum].pers.moveSpeedScaleMultiplier;

    speedScale = (speedScale == 2.0) ? 1.0 : 2.0;

    GameMessage(clientNum, va("Movement speed x2: %s", (speedScale == 2.0) ? "^2On" : "^1Off"));
}

void SelectFov(int clientNum)
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

void ToggleNoclip(int clientNum)
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

void ToggleUfoMode(int clientNum)
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

void ToggleNoclipBind(int clientNum)
{
    users[clientNum].noclipBind ^= 1;

    GameMessage(clientNum, va("Noclip: %s", users[clientNum].noclipBind ? "^2bound to \x12" : "^1unbound"));
}

void ToggleRocketRide(int clientNum)
{
    users[clientNum].rocketRide ^= 1;

    GameMessage(clientNum, va("Rocket ride: %s", users[clientNum].rocketRide ? "^2On" : "^1Off"));
}

void ToggleRocketJump(int clientNum)
{
    users[clientNum].rocketJump ^= 1;

    GameMessage(clientNum, va("Rocket jump: %s", users[clientNum].rocketJump ? "^2On" : "^1Off"));
}

void ToggleRocketJumpStrength(int clientNum)
{
    users[clientNum].rocketJumpStrength += 256.0;

    if (users[clientNum].rocketJumpStrength > 1024.0)
        users[clientNum].rocketJumpStrength = 256.0;

    GameMessage(clientNum, va("Rocket jump strength set to: ^2%.0f", users[clientNum].rocketJumpStrength));
}

void SetAllPerks(int clientNum)
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

void ClearAllPerks(int clientNum)
{
    gentity_s* entity = &level.gentities[clientNum];
    gclient_s* client = entity->client;

    client->ps.perks[0] = 0;
    client->ps.perks[1] = 0;
    for (int i = 0; i < 9; i++)
        client->ps.perkSlots[i] = 0;

    GameMessage(clientNum, "All perks: ^2cleared");
}

void SelectPerk(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* perkName = currentOption->text;

    int perkIndex = BG_GetPerkCodeIndexForName(perkName);

    (HasPerk(clientNum, perkIndex) ? UnsetPerk : SetPerk)(clientNum, perkIndex);

    GameMessage(clientNum, va("%s: ^2%s", HasPerk(clientNum, perkIndex) ? "Set perk" : "Removed perk", perkName));
}

void SelectWeapon(int clientNum)
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

void ResetProjectile(int clientNum)
{
    users[clientNum].projectile = 0;
    GameMessage(clientNum, "Projectile set to: ^2None");
}

void SetProjectile(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* projectileName = currentOption->text;

    users[clientNum].projectile = projectileName;
    GameMessage(clientNum, va("Projectile set to: ^2%s", projectileName));
}

void SelectVision(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* visionName = currentOption->text;

    SV_GameSendServerCommand(clientNum, va("visionset_naked \"%s\" 200", visionName));
    GameMessage(clientNum, va("Vision set: ^2%s", visionName));
}

void SavePosition(int clientNum)
{
    users[clientNum].savedPosition[0] = level.clients[clientNum].ps.origin[0];
    users[clientNum].savedPosition[1] = level.clients[clientNum].ps.origin[1];
    users[clientNum].savedPosition[2] = level.clients[clientNum].ps.origin[2];
    users[clientNum].positionSaved = true;

    GameMessage(clientNum, "Position: ^2Saved!");
}

void LoadPosition(int clientNum)
{
    if (!users[clientNum].positionSaved)
        return GameMessage(clientNum, "^1Error^7: Position wasn't saved!");

    SetPosition(&level.gentities[clientNum], users[clientNum].savedPosition);
    GameMessage(clientNum, "Position: ^2Loaded!");
}

void TeleportToCrosshair(int clientNum)
{
    SetPosition(&level.gentities[clientNum], GetCrosshairPos(clientNum));
    GameMessage(clientNum, "Teleported to crosshair!");
}

void ToggleTeleportGun(int clientNum)
{
    users[clientNum].teleportGun ^= 1;

    GameMessage(clientNum, va("Teleport gun: %s", users[clientNum].teleportGun ? "^2On" : "^1Off"));
}

void SelectTimeScale(int clientNum)
{
    auto* currentOption = users[clientNum].currentMenu->children[users[clientNum].currentOption];
    const char* timeScale = currentOption->text;

    Dvar_SetFromStringByName("com_timescale", timeScale + 1); // Skip the 'x'
    GameMessage(clientNum, va("Time scale set to: ^2%s", timeScale));
}

void SelectPlayerSpeed(int clientNum)
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

void SelectJumpHeight(int clientNum)
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

void ThemeColorRed(int clientNum)
{
    vec3_t color{ 0.9, 0.1, 0.01 };
    users[clientNum].hudComponents[0]->SetRGB(color, 200);
    users[clientNum].hudNavBar->SetRGB(color, 200);
    users[clientNum].hudAuthor->SetRGB(color, 200);
    users[clientNum].hudTitle->SetTextGlow(color);

    GameMessage(clientNum, "Theme color: ^1red");
}

void ThemeColorGreen(int clientNum)
{
    vec3_t color{ 0.008, 0.5, 0.2 };
    users[clientNum].hudComponents[0]->SetRGB(color, 200);
    users[clientNum].hudNavBar->SetRGB(color, 200);
    users[clientNum].hudAuthor->SetRGB(color, 200);
    users[clientNum].hudTitle->SetTextGlow(color);

    GameMessage(clientNum, "Theme color: ^2green");
}

void ThemeColorBlue(int clientNum)
{
    vec3_t color{ 0.0, 0.0, 1.0 };
    users[clientNum].hudComponents[0]->SetRGB(color, 200);
    users[clientNum].hudNavBar->SetRGB(color, 200);
    users[clientNum].hudAuthor->SetRGB(color, 200);
    users[clientNum].hudTitle->SetTextGlow(color);

    GameMessage(clientNum, "Theme color: ^4blue");
}

void ThemeColorYellow(int clientNum)
{
    vec3_t color{ 1.0, 1.0, 0.0 };
    users[clientNum].hudComponents[0]->SetRGB(color, 200);
    users[clientNum].hudNavBar->SetRGB(color, 200);
    users[clientNum].hudAuthor->SetRGB(color, 200);
    users[clientNum].hudTitle->SetTextGlow(color);

    GameMessage(clientNum, "Theme color: ^3yellow");
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
                    TeleportToCrosshair(clientNum);
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

namespace clients
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