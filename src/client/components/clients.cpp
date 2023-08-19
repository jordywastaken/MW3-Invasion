
#include "clients.hpp"
#include "scheduler.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/imports.hpp"

Client users[2];

// Menu design values
static constexpr int ClientMenuWidth = 250;
static constexpr int ClientMenuHeight = 480;
static constexpr int ClientMenuBorderWidth = 2;
static constexpr int ClientMenuNavBarHeight = 15;

static constexpr int ClientMenuSlideInDuration = 400;
static constexpr int ClientMenuFadeInDuration = 400;
static constexpr int ClientMenuSlideOutDuration = 400;
static constexpr int ClientMenuFadeOutDuration = 400;

static constexpr int ClientMenuScrollDuration = 175;

static constexpr float ClientMenuX = 67.0;
static constexpr float ClientMenuTitleY = 50.0;
static constexpr float ClientMenuAuthorY = 70.0;
static constexpr float ClientMenuCurrentMenuY = 85.0;
static constexpr float ClientMenuOptionY = 115.0;
static constexpr float ClientMenuOptionX = ClientMenuX + 7.5;

ClientOption::ClientOption(const char* text, void(*callback)(int))
    : text(text), callback(callback)
{
    for (int i = 0; i < 20; i++)
        children[i] = 0;
}

ClientOption::~ClientOption()
{
    for (int i = 0; i < 20; i++)
    {
        if (!children[i])
            continue;

        delete children[i];
        children[i] = 0;
    }
}

void ClientOption::AddChild(ClientOption* child)
{
    for (int i = 0; i < 20; i++)
    {
        if (children[i])
            continue;

        children[i] = child;
        return;
    }
}

size_t ClientOption::GetChildCount()
{
    size_t count = 0;
    for (int i = 0; i < 20; i++)
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

    auto* mainModsMenu = MakeSubmenu("Main mods menu");
    mainModsMenu->AddChild(MakeOption("God mode", ToggleGodMode));
    mainModsMenu->AddChild(MakeOption("Infinite ammo", ToggleInfiniteAmmo));
    mainModsMenu->AddChild(MakeOption("No spread", ToggleNoSpread));
    mainModsMenu->AddChild(MakeOption("No recoil", ToggleNoRecoil));
    mainModsMenu->AddChild(MakeOption("Movement speed x2", ToggleMovementSpeed));
    mainMenu->AddChild(mainModsMenu);

    auto* funMenu = MakeSubmenu("Fun menu");
    funMenu->AddChild(MakeOption("Rocket ride", ToggleRocketRide));
    funMenu->AddChild(MakeOption("Rocket jump", ToggleRocketJump));
    funMenu->AddChild(MakeOption("Rocket jump strength", ToggleRocketJumpStrength));
    mainMenu->AddChild(funMenu);

    //auto* perksMenu = MakeSubmenu("Perks menu");
    //perksMenu->AddChild(MakeOption("Set all perks", 0));
    //perksMenu->AddChild(MakeOption("Remove all perks", 0));
    //mainMenu->AddChild(perksMenu);

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
    projectileMenu->AddChild(MakeOption("None", ResetProjectile));
    mainMenu->AddChild(projectileMenu);

    for (int i = 0; i < 240; i++)
    {
        if (!bg_weaponCompleteDefs[i])
            continue;

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
            weaponMenu->children[childIndex]->AddChild(MakeOption(bg_weaponCompleteDefs[i]->szInternalName, SelectWeapon));
        }

        if (bg_weaponCompleteDefs[i]->weapDef->weapType == WEAPTYPE_PROJECTILE)
        {
            projectileMenu->AddChild(MakeOption(bg_weaponCompleteDefs[i]->szInternalName, SetProjectile));
        }
    }

    auto* teleportMenu = MakeSubmenu("Teleport menu");
    teleportMenu->AddChild(MakeOption("Save position", SavePosition));
    teleportMenu->AddChild(MakeOption("Load position", LoadPosition));
    teleportMenu->AddChild(MakeOption("Teleport to crosshair", TeleportToCrosshair));
    teleportMenu->AddChild(MakeOption("Teleport gun", ToggleTeleportGun));
    mainMenu->AddChild(teleportMenu);

    //auto* lobbyMenu = MakeSubmenu("Lobby menu");
    //mainMenu->AddChild(lobbyMenu);

    auto* settingsMenu = MakeSubmenu("Settings menu");
    settingsMenu->AddChild(MakeOption("Theme red", MenuColorRed));
    settingsMenu->AddChild(MakeOption("Theme green", MenuColorGreen));
    settingsMenu->AddChild(MakeOption("Theme blue", MenuColorBlue));
    settingsMenu->AddChild(MakeOption("Theme yellow", MenuColorYellow));
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
    // Panels
    hudBackground = CreateMaterialHudElem(clientNum, "white", ClientMenuWidth, ClientMenuHeight, ClientMenuX, 0.0, 0.0, { 0.0, 0.0, 0.0 }, 0.0);
    hudBackground->SetHorzAlign(HE_HORZALIGN_RIGHT);

    hudLeftBorder = CreateMaterialHudElem(clientNum, "white", ClientMenuBorderWidth, ClientMenuHeight, ClientMenuX - ClientMenuBorderWidth, 0.0, 1.0, menuColor, 0.0);
    hudLeftBorder->SetHorzAlign(HE_HORZALIGN_RIGHT);

    hudNavBar = CreateMaterialHudElem(clientNum, "white", ClientMenuWidth, ClientMenuNavBarHeight, ClientMenuX, ClientMenuOptionY - 1.0, 1.0, menuColor, 0.0);
    hudNavBar->SetHorzAlign(HE_HORZALIGN_RIGHT);

    // Labels
    hudTitle = CreateTextHudElem(clientNum, "Invasion", 2.0, ClientMenuX + ClientMenuWidth / 2, ClientMenuTitleY, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
    hudTitle->SetHorzAlign(HE_HORZALIGN_RIGHT);
    hudTitle->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
    hudTitle->SetTextGlow(menuColor);
    hudTitle->SetTextFont(HE_FONT_OBJECTIVE);

    hudAuthor = CreateTextHudElem(clientNum, "by Jordy", 1.0, ClientMenuX + ClientMenuWidth / 2, ClientMenuAuthorY, 1.0, menuColor, 0.0);
    hudAuthor->SetHorzAlign(HE_HORZALIGN_RIGHT);
    hudAuthor->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);

    hudCurrentMenu = CreateTextHudElem(clientNum, "Main menu", 1.6, ClientMenuOptionX, ClientMenuCurrentMenuY, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
    hudCurrentMenu->SetHorzAlign(HE_HORZALIGN_RIGHT);

    for (int i = 0; i < 20; i++)
    {
        hudOptions[i] = CreateTextHudElem(clientNum, currentMenu->children[i] ? currentMenu->children[i]->text : "", 1.0, ClientMenuOptionX, ClientMenuOptionY + (ClientMenuNavBarHeight * i), 2.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudOptions[i]->SetHorzAlign(HE_HORZALIGN_RIGHT);
    }
}

void Client::DestroyHudElems()
{
    if (!hudBackground)
        return;

    delete hudBackground;
    delete hudLeftBorder;
    delete hudNavBar;
    delete hudTitle;
    delete hudAuthor;
    delete hudCurrentMenu;

    for(int i = 0; i < 20; i++)
        delete hudOptions[i];
}

void Client::SetDefaults()
{
    opened = false;
    buttonBits = 0;
    buttonTick = 0;
    currentOption = 0;
    rootMenu = 0;
    currentMenu = 0;

    for (int i = 0; i < 10; i++)
    {
        previousMenu[i] = 0;
        previousOption[i] = 0;
    }
    submenuLevel = 0;

    hudBackground = 0;
    hudLeftBorder = 0;
    hudNavBar = 0;
    hudTitle = 0;
    hudAuthor = 0;
    hudCurrentMenu = 0;

    for (int i = 0; i < 20; i++)
        hudOptions[i] = 0;

    menuColor = { 0.008, 0.5, 0.2 };
    infiniteAmmo = false;
    rocketRide = false;
    rocketJump = false;
    rocketJumpStrength = 128.0;
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
    level.clients[clientNum].ps.weapCommon.weapFlags &= ~(1 << 11); // Enable action slots
    level.clients[clientNum].ps.pm_flags &= ~(1 << 18); // Enable jump
    level.clients[clientNum].ps.pm_flags &= ~(1 << 24); // Enable crouch
    level.clients[clientNum].ps.pm_flags &= ~(1 << 25); // Enable prone
    level.clients[clientNum].ps.otherFlags &= ~(1 << 14); // Enable hud

    if (opened || !InputReady())
    {
        level.clients[clientNum].ps.weapCommon.weapFlags |= (1 << 11); // Disable action slots
        level.clients[clientNum].ps.pm_flags |= (1 << 18); // Disable jump
        level.clients[clientNum].ps.pm_flags |= (1 << 24); // Disable crouch
        level.clients[clientNum].ps.pm_flags |= (1 << 25); // Disable prone
        level.clients[clientNum].ps.otherFlags |= (1 << 14); // Disable hud
    }

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
    buttonTick = level.time + ms;
}

bool Client::InputReady()
{
    return level.time > buttonTick;
}

void Client::ChangeSubmenu(ClientOption* submenu)
{
    currentOption = 0;
    currentMenu = submenu;

    hudCurrentMenu->SetText(submenu->text);
    hudCurrentMenu->SetAlpha(0.0);
    hudCurrentMenu->SetAlpha(1.0, ClientMenuFadeInDuration);

    hudNavBar->SetAlpha(0.0);
    hudNavBar->SetAlpha(0.5, ClientMenuFadeInDuration);

    for (int i = 0; i < 20; i++)
    {
        hudOptions[i]->SetText(submenu->children[i] ? submenu->children[i]->text : "");
        hudOptions[i]->SetAlpha(0.0);
        hudOptions[i]->SetAlpha(1.0, ClientMenuFadeInDuration);
    }
}

void Client::OnOpen()
{
    if (!rootMenu)
        CreateMenu();

    if (!hudBackground)
        CreateHudElems();

    opened = true;

    // Slide in
    hudBackground->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);
    hudLeftBorder->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);
    hudTitle->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);
    hudAuthor->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);

    hudNavBar->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);
    hudCurrentMenu->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);
    for(auto option : hudOptions) 
        option->AddX(-ClientMenuWidth, ClientMenuSlideInDuration);

    // Fade in
    hudBackground->SetAlpha(0.6, ClientMenuSlideInDuration + ClientMenuFadeInDuration);
    hudLeftBorder->SetAlpha(1.0, ClientMenuSlideInDuration + ClientMenuFadeInDuration);
    hudTitle->SetAlpha(1.0, ClientMenuSlideInDuration + ClientMenuFadeInDuration);
    hudAuthor->SetAlpha(1.0, ClientMenuSlideInDuration + ClientMenuFadeInDuration);
    
    // Start fading after the slide in animation
    hudNavBar->SetAlpha(0.6, ClientMenuFadeInDuration, ClientMenuSlideInDuration);
    hudCurrentMenu->SetAlpha(1.0, ClientMenuFadeInDuration, ClientMenuSlideInDuration);
    for (auto option : hudOptions)
        option->SetAlpha(1.0, ClientMenuFadeInDuration, ClientMenuSlideInDuration);

    InputSleep(ClientMenuSlideInDuration + ClientMenuFadeInDuration);
}

void Client::OnClose()
{
    opened = false;

    // Fade out
    hudBackground->SetAlpha(0.0, ClientMenuSlideOutDuration + ClientMenuFadeOutDuration);
    hudLeftBorder->SetAlpha(0.0, ClientMenuSlideOutDuration + ClientMenuFadeOutDuration);
    hudTitle->SetAlpha(0.0, ClientMenuSlideOutDuration + ClientMenuFadeOutDuration);
    hudAuthor->SetAlpha(0.0, ClientMenuSlideOutDuration + ClientMenuFadeOutDuration);

    hudNavBar->SetAlpha(0.0, ClientMenuFadeOutDuration);
    hudCurrentMenu->SetAlpha(0.0, ClientMenuFadeOutDuration);
    for (auto option : hudOptions)
        option->SetAlpha(0.0, ClientMenuFadeOutDuration);

    // Slide out once the fading animation is done
    hudBackground->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);
    hudLeftBorder->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);
    hudTitle->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);
    hudAuthor->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);

    hudNavBar->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);
    hudCurrentMenu->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);
    for (auto option : hudOptions)
        option->AddX(ClientMenuWidth, ClientMenuSlideOutDuration, ClientMenuFadeOutDuration);

    InputSleep(ClientMenuSlideOutDuration + ClientMenuFadeOutDuration);
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
        submenuLevel++;
        ChangeSubmenu(currentMenu->children[currentOption]);
        hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption);
        InputSleep(ClientMenuFadeInDuration);
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
        ChangeSubmenu(previousMenu[submenuLevel]);
        currentOption = previousOption[submenuLevel];
        hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption);
        InputSleep(ClientMenuFadeInDuration);
    }
}

void Client::OnScrollUp()
{
    currentOption = currentOption == 0 ? currentMenu->GetChildCount() - 1 : currentOption - 1;
    hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption, ClientMenuScrollDuration);
    InputSleep(ClientMenuScrollDuration);
}

void Client::OnScrollDown()
{
    currentOption = currentOption == (currentMenu->GetChildCount() - 1) ? 0 : currentOption + 1;
    hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption, ClientMenuScrollDuration);
    InputSleep(ClientMenuScrollDuration);
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

void ToggleMovementSpeed(int clientNum)
{
    float& speedScale = level.clients[clientNum].pers.moveSpeedScaleMultiplier;

    speedScale = (speedScale == 2.0) ? 1.0 : 2.0;

    GameMessage(clientNum, va("Movement speed x2: %s", (speedScale == 2.0) ? "^2On" : "^1Off"));
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
    users[clientNum].rocketJumpStrength += 64.0;

    if (users[clientNum].rocketJumpStrength > 512.0)
        users[clientNum].rocketJumpStrength = 128.0;

    GameMessage(clientNum, va("Rocket jump strength set to: ^2%.0f", users[clientNum].rocketJumpStrength));
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

void MenuColorRed(int clientNum)
{
    vec3_t color{ 0.9, 0.1, 0.01 };
    users[clientNum].hudLeftBorder->SetRGB(color, 200);
    users[clientNum].hudNavBar->SetRGB(color, 200);
    users[clientNum].hudAuthor->SetRGB(color, 200);
    users[clientNum].hudTitle->SetTextGlow(color);

    GameMessage(clientNum, "Menu color: ^1red");
}

void MenuColorGreen(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 0.008, 0.5, 0.2 });

    GameMessage(clientNum, "Menu color: ^2green");
}

void MenuColorBlue(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 0.0, 0.0, 1.0 });

    GameMessage(clientNum, "Menu color: ^4blue");
}

void MenuColorYellow(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 1.0, 1.0, 0.0 });

    GameMessage(clientNum, "Menu color: ^3yellow");
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
        //printf("Key: %i\n", key);

        switch (key)
        {
            // L1
        case(13): user.buttonBits |= CLIENT_BTN_L1; break;
        case(14): user.buttonBits &= ~CLIENT_BTN_L1; break;

            // Dpad up
        case(15): user.buttonBits |= CLIENT_BTN_DPAD_UP; break;
        case(16): user.buttonBits &= ~CLIENT_BTN_DPAD_UP; break;

            // Dpad down
        case(17): user.buttonBits |= CLIENT_BTN_DPAD_DOWN; break;
        case(18): user.buttonBits &= ~CLIENT_BTN_DPAD_DOWN; break;

            // Dpad left
        case(19): user.buttonBits |= CLIENT_BTN_DPAD_LEFT; break;
        case(20): user.buttonBits &= ~CLIENT_BTN_DPAD_LEFT; break;

            // Dpad right
        case(21): user.buttonBits |= CLIENT_BTN_DPAD_RIGHT; break;
        case(22): user.buttonBits &= ~CLIENT_BTN_DPAD_RIGHT; break;

            // Circle
        case(23): user.buttonBits |= CLIENT_BTN_CIRCLE; break;
        case(24): user.buttonBits &= ~CLIENT_BTN_CIRCLE; break;

            // Cross
        case(25): user.buttonBits |= CLIENT_BTN_CROSS; break;
        case(26): user.buttonBits &= ~CLIENT_BTN_CROSS; break;

            // R3
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
        printf("Destroying all menus\n");
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

            printf("VM_Notify: client %i '%s'\n", clientNum, notify);

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

namespace clients
{
    void start()
    {
        for (int i = 0; i < 2; i++)
        {
            users[i].ClearAll();
            users[i].clientNum = i;
        }

        hook::jump(0x13EC84, *reinterpret_cast<uintptr_t*>(ClientCommand_Hook));
        hook::jump(0x12F708, *reinterpret_cast<uintptr_t*>(ClientThink_real_Hook));
        hook::jump(0x2314B8, *reinterpret_cast<uintptr_t*>(VM_Notify_Hook));
        hook::jump(0x2BADA4, *reinterpret_cast<uintptr_t*>(PM_WeaponUseAmmo_Hook));
        hook::jump(0x18D930, *reinterpret_cast<uintptr_t*>(Weapon_RocketLauncher_Fire_Hook));
    }

    void stop()
    {
        hook::copy(0x13EC84, *reinterpret_cast<uint32_t**>(ClientCommand_Original), 4);
        hook::copy(0x12F708, *reinterpret_cast<uint32_t**>(ClientThink_real_Original), 4);
        hook::copy(0x2314B8, *reinterpret_cast<uint32_t**>(VM_Notify_Original), 4);
        hook::copy(0x2BADA4, *reinterpret_cast<uint32_t**>(PM_WeaponUseAmmo_Original), 4);
        hook::copy(0x18D930, *reinterpret_cast<uint32_t**>(Weapon_RocketLauncher_Fire_Original), 4);

        for (auto& user : users)
            user.ClearAll();
    }
}