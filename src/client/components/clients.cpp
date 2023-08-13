
#include "clients.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/imports.hpp"

Client users[2];

///
/// Methods
///
void Client::SetDefaults()
{
    opened = false;
    buttonBits = 0;
    buttonTick = 0;
    currentOption = 0;
    hudBackground = 0;
    hudLeftBorder = 0;
    hudNavBar = 0;
    hudTitle = 0;
    hudAuthor = 0;
    hudCurrentMenu = 0;
    
    for (int i = 0; i < 20; i++)
        hudOptions[i] = 0;

    bGodMode = false;
    menuColor = { 0.008, 0.5, 0.2 };
}

void Client::Destroy()
{
    delete hudBackground;
    delete hudLeftBorder;
    delete hudNavBar;
    delete hudTitle;
    delete hudAuthor;
    delete hudCurrentMenu;

    for(int i = 0; i < 20; i++)
        delete hudOptions[i];

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
            if (buttonBits & CLIENT_BTN_CIRCLE)
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

static constexpr int ClientMenuWidth = 250L;
static constexpr int ClientMenuHeight = 480L;
static constexpr int ClientMenuBorderWidth = 2L;
static constexpr int ClientMenuAnimationDuration = 400;

static constexpr float ClientMenuX = 67.0;
static constexpr float ClientMenuY = -ClientMenuHeight;
static constexpr float ClientMenuNavBarSize = 15.0;

void Client::CreateHudElems()
{
    hudBackground = CreateMaterialHudElem(clientNum, "white", ClientMenuWidth, ClientMenuHeight, ClientMenuX, 0.0, 0.0, { 0.0, 0.0, 0.0 }, 0.0);
    hudBackground->SetHorzAlign(HE_HORZALIGN_RIGHT);

    hudLeftBorder = CreateMaterialHudElem(clientNum, "white", ClientMenuBorderWidth, ClientMenuHeight, ClientMenuX - ClientMenuBorderWidth, 0.0, 1.0, menuColor, 0.0);
    hudLeftBorder->SetHorzAlign(HE_HORZALIGN_RIGHT);

    hudNavBar = CreateMaterialHudElem(clientNum, "white", ClientMenuWidth, (int)ClientMenuNavBarSize, ClientMenuX, 114.0, 1.0, menuColor, 0.0);
    hudNavBar->SetHorzAlign(HE_HORZALIGN_RIGHT);

    hudTitle = CreateTextHudElem(clientNum, "Invasion", 2.0, ClientMenuX + ClientMenuWidth / 2, 50.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
    hudTitle->SetHorzAlign(HE_HORZALIGN_RIGHT);
    hudTitle->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);
    hudTitle->SetTextGlow(menuColor);
    hudTitle->SetTextFont(HE_FONT_OBJECTIVE);

    hudAuthor = CreateTextHudElem(clientNum, "by Jordy", 1.0, ClientMenuX + ClientMenuWidth / 2, 70.0, 1.0, menuColor, 0.0);
    hudAuthor->SetHorzAlign(HE_HORZALIGN_RIGHT);
    hudAuthor->SetAlignOrg(HE_ALIGN_X_CENTER, HE_ALIGN_Y_MIDDLE);

    hudCurrentMenu = CreateTextHudElem(clientNum, "Main menu", 1.6, ClientMenuX + 7.5, 85.0, 1.0, { 1.0, 1.0, 1.0 }, 0.0);
    hudCurrentMenu->SetHorzAlign(HE_HORZALIGN_RIGHT);

    for (int i = 0; i < 20; i++)
    {
        hudOptions[i] = CreateTextHudElem(clientNum, va("Option %i", i + 1), 1.0, ClientMenuX + 7.5, 115.0 + ClientMenuNavBarSize * i, 2.0, { 1.0, 1.0, 1.0 }, 0.0);
        hudOptions[i]->SetHorzAlign(HE_HORZALIGN_RIGHT);
    }
}

void Client::OnOpen()
{
    opened = true;

    if (!hudBackground)
    {
        CreateHudElems();
    }

    hudBackground->SetAlpha(0.6, ClientMenuAnimationDuration * 2);
    hudBackground->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    hudLeftBorder->SetAlpha(1.0, ClientMenuAnimationDuration * 2);
    hudLeftBorder->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    hudNavBar->SetAlpha(0.6, ClientMenuAnimationDuration, ClientMenuAnimationDuration);
    hudNavBar->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    hudTitle->SetAlpha(1.0, ClientMenuAnimationDuration * 2);
    hudTitle->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    hudAuthor->SetAlpha(1.0, ClientMenuAnimationDuration * 2);
    hudAuthor->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    hudCurrentMenu->SetAlpha(1.0, ClientMenuAnimationDuration, ClientMenuAnimationDuration);
    hudCurrentMenu->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);

    for (int i = 0; i < 20; i++)
    {
        hudOptions[i]->SetAlpha(1.0, ClientMenuAnimationDuration, ClientMenuAnimationDuration);
        hudOptions[i]->AddX(-ClientMenuWidth, ClientMenuAnimationDuration);
    }

    InputSleep(ClientMenuAnimationDuration * 2);
}

void Client::OnClose()
{
    opened = false;
    hudBackground->SetAlpha(0.0, ClientMenuAnimationDuration * 2);
    hudBackground->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    hudLeftBorder->SetAlpha(0.0, ClientMenuAnimationDuration * 2);
    hudLeftBorder->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    hudNavBar->SetAlpha(0.0, ClientMenuAnimationDuration);
    hudNavBar->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    hudTitle->SetAlpha(0.0, ClientMenuAnimationDuration * 2);
    hudTitle->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    hudAuthor->SetAlpha(0.0, ClientMenuAnimationDuration * 2);
    hudAuthor->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    hudCurrentMenu->SetAlpha(0.0, ClientMenuAnimationDuration);
    hudCurrentMenu->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);

    for (int i = 0; i < 20; i++)
    {
        hudOptions[i]->SetAlpha(0.0, ClientMenuAnimationDuration);
        hudOptions[i]->AddX(ClientMenuWidth, ClientMenuAnimationDuration, ClientMenuAnimationDuration);
    }

    InputSleep(ClientMenuAnimationDuration * 2);
}

void Client::OnCancel()
{
    OnClose();
}

void Client::OnScrollUp()
{
    currentOption = currentOption == 0 ? 19 : currentOption - 1;
    hudNavBar->SetY(114.0 + ClientMenuNavBarSize * currentOption, 200);
    InputSleep(225);
}

void Client::OnScrollDown()
{
    currentOption = currentOption == 19 ? 0 : currentOption + 1;
    hudNavBar->SetY(114.0 + ClientMenuNavBarSize * currentOption, 200);
    InputSleep(225);
}

void Client::Run()
{
    HandleInput();
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
    Client& user = users[clientNum];

    if (_sys_strncmp(s, "n ", 2) == 0)
    {
        int key = atoi(s + 2);
        //printf("Key: %i\n", key);

        switch (key)
        {
            // Cross
        case(25): user.buttonBits |= CLIENT_BTN_CROSS; break;
        case(26): user.buttonBits &= ~CLIENT_BTN_CROSS; break;

            // Circle
        case(23): user.buttonBits |= CLIENT_BTN_CIRCLE; break;
        case(24): user.buttonBits &= ~CLIENT_BTN_CIRCLE; break;

            // L1
        case(13): user.buttonBits |= CLIENT_BTN_L1; break;
        case(14): user.buttonBits &= ~CLIENT_BTN_L1; break;

            // R3
        case(27): user.buttonBits |= CLIENT_BTN_R3; break;
        case(28): user.buttonBits &= ~CLIENT_BTN_R3; break;

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

        default:
            break;
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
    if (ent && ent->client)
    {
        gclient_s& client = *ent->client;
        Client& user = users[client.ps.clientNum];

        user.Run();
    }

    ClientThink_real_Original(ent);
}

namespace clients
{
    void start()
    {
        for (int i = 0; i < 2; i++)
        {
            users[i].SetDefaults();
        }

        hook::jump(0x13EC84, *(uintptr_t*)(ClientCommand_Hook));
        hook::jump(0x12F708, *(uintptr_t*)(ClientThink_real_Hook));
    }

    void stop()
    {
        hook::restore(0x13EC84, *(uint32_t**)(ClientCommand_Original), 4);
        hook::restore(0x12F708, *(uint32_t**)(ClientThink_real_Original), 4);

        for (int i = 0; i < 2; i++)
        {
            users[i].Destroy();
        }
    }
}