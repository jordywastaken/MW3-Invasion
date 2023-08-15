
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

    auto* teleportMenu = MakeSubmenu("Teleport menu");
    teleportMenu->AddChild(MakeOption("Save position", SavePosition));
    teleportMenu->AddChild(MakeOption("Load position", LoadPosition));
    teleportMenu->AddChild(MakeOption("Teleport to crosshair", TeleportToCrosshair));
    teleportMenu->AddChild(MakeOption("Teleport gun", ToggleTeleportGun));
    mainMenu->AddChild(teleportMenu);

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
    hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption, 200);
    InputSleep(225);
}

void Client::OnScrollDown()
{
    currentOption = currentOption == (currentMenu->GetChildCount() - 1) ? 0 : currentOption + 1;
    hudNavBar->SetY(ClientMenuOptionY - 1 + ClientMenuNavBarHeight * currentOption, 200);
    InputSleep(225);
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
void Scr_ClearOutParams()
{
    while (scrVmPub.outparamcount)
    {
        RemoveRefToValue(scrVmPub.top->type, &scrVmPub.top->u);
        --scrVmPub.top;
        --scrVmPub.outparamcount;
    }
}

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

void MagicBullet(const char* weapon, float* start, float* end)
{
    Scr_AddVector(end);
    Scr_AddVector(start);
    Scr_AddString(weapon);
    scrVmPub.outparamcount = 3;
    hook::invoke<void>(0x162A38);
    Scr_ClearOutParams();
}

void PrintLine(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("gm \"%s\"", text));
}

void PrintLineBold(int clientNum, const char* text)
{
    SV_GameSendServerCommand(clientNum, va("gmb \"%s\"", text));
}

void SetClientDvar(int clientNum, const char* dvar, const char* value)
{
    SV_GameSendServerCommand(clientNum, va("setclientdvar \"%s\" \"%s\"", dvar, value));
}

void ToggleGodMode(int clientNum)
{
    const int bit = (1 << 0);
    int& value = level.clients[clientNum].ps.otherFlags;

    value ^= bit;

    PrintLine(clientNum, va("God mode: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleInfiniteAmmo(int clientNum)
{
    users[clientNum].infiniteAmmo ^= 1;

    PrintLine(clientNum, va("Infinite ammo: %s", users[clientNum].infiniteAmmo ? "^2On" : "^1Off"));
}

void ToggleNoSpread(int clientNum)
{
    const int bit = (1 << 1);
    int& value = level.clients[clientNum].ps.weapCommon.spreadOverrideState;

    value ^= bit;
    level.clients[clientNum].ps.weapCommon.spreadOverride = 0;

    PrintLine(clientNum, va("No spread: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleNoRecoil(int clientNum)
{
    const int bit = (1 << 10);
    int& value = level.clients[clientNum].ps.weapCommon.weapFlags;

    value ^= bit;
    level.clients[clientNum].ps.recoilScale = 0;

    PrintLine(clientNum, va("No recoil: %s", (value & bit) == bit ? "^2On" : "^1Off"));
}

void ToggleMovementSpeed(int clientNum)
{
    float& speedScale = level.clients[clientNum].pers.moveSpeedScaleMultiplier;

    speedScale = (speedScale == 2.0) ? 1.0 : 2.0;

    PrintLine(clientNum, va("Movement speed x2: %s", (speedScale == 2.0) ? "^2On" : "^1Off"));
}

void SavePosition(int clientNum)
{
    users[clientNum].savedPosition[0] = level.clients[clientNum].ps.origin[0];
    users[clientNum].savedPosition[1] = level.clients[clientNum].ps.origin[1];
    users[clientNum].savedPosition[2] = level.clients[clientNum].ps.origin[2];
    users[clientNum].positionSaved = true;

    PrintLine(clientNum, "Position: ^2Saved!");
}

void LoadPosition(int clientNum)
{
    if (users[clientNum].positionSaved)
    {
        level.clients[clientNum].ps.origin[0] = users[clientNum].savedPosition[0];
        level.clients[clientNum].ps.origin[1] = users[clientNum].savedPosition[1];
        level.clients[clientNum].ps.origin[2] = users[clientNum].savedPosition[2];

        PrintLine(clientNum, "Position: ^2Loaded!");
    }
    else
    {
        PrintLine(clientNum, "^1Error^7: Position wasn't saved!");
    }
}

void TeleportToCrosshair(int clientNum)
{
    float* crosshairPosition = GetCrosshairPos(clientNum);
    level.clients[clientNum].ps.origin[0] = crosshairPosition[0];
    level.clients[clientNum].ps.origin[1] = crosshairPosition[1];
    level.clients[clientNum].ps.origin[2] = crosshairPosition[2];

    PrintLine(clientNum, "Teleported to crosshair!");
}

void ToggleTeleportGun(int clientNum)
{
    users[clientNum].teleportGun ^= 1;

    PrintLine(clientNum, va("Teleport gun: %s", users[clientNum].teleportGun ? "^2On" : "^1Off"));
}

void MenuColorRed(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 0.9, 0.1, 0.01 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 0.9, 0.1, 0.01 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 0.9, 0.1, 0.01 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 0.9, 0.1, 0.01 });

    PrintLine(clientNum, "Menu color: ^1red");
}

void MenuColorGreen(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 0.008, 0.5, 0.2 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 0.008, 0.5, 0.2 });

    PrintLine(clientNum, "Menu color: ^2green");
}

void MenuColorBlue(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 0.0, 0.0, 1.0 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 0.0, 0.0, 1.0 });

    PrintLine(clientNum, "Menu color: ^4blue");
}

void MenuColorYellow(int clientNum)
{
    users[clientNum].hudLeftBorder->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudNavBar->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudAuthor->SetRGB({ 1.0, 1.0, 0.0 }, 200);
    users[clientNum].hudTitle->SetTextGlow({ 1.0, 1.0, 0.0 });

    PrintLine(clientNum, "Menu color: ^3yellow");
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
                // Magic bullet
                if (0)
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
                    MagicBullet("rpg_survival", startPos, tracePos);
                }

                if (users[clientNum].teleportGun)
                {
                    TeleportToCrosshair(clientNum);
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
    }

    void stop()
    {
        hook::copy(0x13EC84, *reinterpret_cast<uint32_t**>(ClientCommand_Original), 4);
        hook::copy(0x12F708, *reinterpret_cast<uint32_t**>(ClientThink_real_Original), 4);
        hook::copy(0x2314B8, *reinterpret_cast<uint32_t**>(VM_Notify_Original), 4);
        hook::copy(0x2BADA4, *reinterpret_cast<uint32_t**>(PM_WeaponUseAmmo_Original), 4);

        for (auto& user : users)
            user.ClearAll();
    }
}