
#include "hudelems.hpp"
#include "common/utils/imports.hpp"

game_hudelem_s* HudElem::Alloc(int client)
{
    for (int i = 0; i < 1024; i++)
    {
        if (g_hudelems[i].elem.type == HE_TYPE_FREE)
        {
            auto* hud = &g_hudelems[i];
            debug_print("HudElem::Alloc -> 0x%p\n", hud);
            hud->clientNum = client;
            return hud;
        }
    }
    return nullptr;
}

void HudElem::Destroy(game_hudelem_s* hud)
{
    if (hud)
    {
        _sys_memset(hud, 0, sizeof(game_hudelem_s));
        hud = 0;
    }
}

HudElem::HudElem(int client)
    : hud(Alloc(client))
{
    SetDefaults();
}

HudElem::~HudElem()
{
    Destroy(hud);
}

void HudElem::SetDefaults()
{
    if (hud)
    {
        hud->elem.type = HE_TYPE_TEXT;
        hud->elem.font = HE_FONT_DEFAULT;
        hud->elem.x = 0.0;
        hud->elem.y = 0.0;
        hud->elem.z = 0.0;
        hud->elem.fromX = 0.0;
        hud->elem.fromY = 0.0;
        hud->elem.moveStartTime = 0;
        hud->elem.moveTime = 0;
        hud->elem.targetEntNum = 2047;
        hud->elem.fontScale = 1.0;
        hud->elem.fromFontScale = 0.0;
        hud->elem.fontScaleStartTime = 0;
        hud->elem.fontScaleTime = 0;
        hud->elem.width = 0;
        hud->elem.height = 0;
        hud->elem.fromWidth = 0;
        hud->elem.fromHeight = 0;
        hud->elem.scaleStartTime = 0;
        hud->elem.scaleTime = 0;
        hud->elem.alignOrg = 0;
        hud->elem.alignScreen = 0;
        hud->elem.fromAlignOrg = 0;
        hud->elem.fromAlignScreen = 0;
        hud->elem.color.rgba = -1;
        hud->elem.glowColor.rgba = 0;
        hud->elem.fromColor.rgba = 0;
        hud->elem.fadeStartTime = 0;
        hud->elem.fadeTime = 0;
        hud->elem.materialIndex = 0;
        hud->elem.time = 0;
        hud->elem.duration = 0;
        hud->elem.value = 0.0;
        hud->elem.sort = 0.0;
        hud->elem.flags = HUDELEMFLAG_FOREGROUND;
        hud->elem.soundID = 0;
        hud->elem.fxBirthTime = 0;
        hud->elem.fxLetterTime = 0;
        hud->elem.fxDecayStartTime = 0;
        hud->elem.fxDecayDuration = 0;
        G_SetLocalizedString("", &hud->elem.label);
        G_SetLocalizedString("", &hud->elem.text);
    }
}

void HudElem::MoveOvertime(float x, float y, int durationMs, int startDelayMs)
{
    hud->elem.fromAlignOrg = hud->elem.alignOrg;
    hud->elem.fromAlignScreen = hud->elem.alignScreen;

    hud->elem.fromX = hud->elem.x;
    hud->elem.fromY = hud->elem.y;
    hud->elem.x = x;
    hud->elem.y = y;
    hud->elem.moveStartTime = level.time + startDelayMs;
    hud->elem.moveTime = durationMs;
}

void HudElem::SetX(float x, int durationMs, int startDelayMs)
{
    MoveOvertime(x, hud->elem.y, durationMs, startDelayMs);
}

void HudElem::SetY(float y, int durationMs, int startDelayMs)
{
    MoveOvertime(hud->elem.x, y, durationMs, startDelayMs);
}

void HudElem::AddPosOvertime(float x, float y, int durationMs, int startDelayMs)
{
    MoveOvertime(hud->elem.x + x, hud->elem.y + y, durationMs, startDelayMs);
}

void HudElem::AddX(float x, int durationMs, int startDelayMs)
{
    AddPosOvertime(x, 0.0, durationMs, startDelayMs);
}

void HudElem::AddY(float y, int durationMs, int startDelayMs)
{
    AddPosOvertime(0.0, y, durationMs, startDelayMs);
}

void HudElem::ScaleOvertime(int width, int height, int durationMs, int startDelayMs)
{
    hud->elem.fromWidth = hud->elem.width;
    hud->elem.fromHeight = hud->elem.height;
    hud->elem.width = width;
    hud->elem.height = height;
    hud->elem.scaleStartTime = level.time + startDelayMs;
    hud->elem.scaleTime = durationMs;
}

void HudElem::SetWidth(int width, int durationMs, int startDelayMs)
{
    ScaleOvertime(width, hud->elem.height, durationMs, startDelayMs);
}

void HudElem::SetHeight(int height, int durationMs, int startDelayMs)
{
    ScaleOvertime(hud->elem.width, height, durationMs, startDelayMs);
}

void HudElem::AddWidth(int width, int durationMs, int startDelayMs)
{
    SetWidth(hud->elem.width + width, durationMs, startDelayMs);
}

void HudElem::AddHeight(int height, int durationMs, int startDelayMs)
{
    SetHeight(hud->elem.height + height, durationMs, startDelayMs);
}

void HudElem::FadeOvertime(vec4_t color, int durationMs, int startDelayMs)
{
    hud->elem.fromColor.rgba = hud->elem.color.rgba;
    R_ConvertColorToBytes(color.v, (unsigned char*)&hud->elem.color.rgba);
    hud->elem.fadeStartTime = level.time + startDelayMs;
    hud->elem.fadeTime = durationMs;
}

void HudElem::SetRGB(vec3_t rgb, int durationMs, int startDelayMs)
{
    vec4_t color
    {
        rgb[0],
        rgb[1],
        rgb[2],
        hud->elem.color.a / 255.f
    };
    FadeOvertime(color, durationMs, startDelayMs);
}

void HudElem::SetAlpha(float alpha, int durationMs, int startDelayMs)
{
    vec4_t color
    {
        hud->elem.color.r / 255.f,
        hud->elem.color.g / 255.f,
        hud->elem.color.b / 255.f,
        alpha
    };
    FadeOvertime(color, durationMs, startDelayMs);
}

void HudElem::SetTextScale(float scale, int durationMs, int startDelayMs)
{
    hud->elem.fromFontScale = hud->elem.fontScale;
    hud->elem.fontScale = scale;
    hud->elem.fontScaleStartTime = level.time + startDelayMs;
    hud->elem.fontScaleTime = durationMs;
}

void HudElem::SetTextFont(he_font_t font)
{
    hud->elem.font = font;
}

void HudElem::SetTextGlow(vec3_t rgb, float alpha)
{
    vec4_t color
    {
        rgb[0],
        rgb[1],
        rgb[2],
        alpha
    };
    R_ConvertColorToBytes(color.v, (unsigned char*)&hud->elem.glowColor.rgba);
}

void HudElem::SetTextGlowAlpha(float alpha)
{
    vec4_t color
    {
        hud->elem.glowColor.r / 255.f,
        hud->elem.glowColor.g / 255.f,
        hud->elem.glowColor.b / 255.f,
        alpha
    };
    R_ConvertColorToBytes(color.v, (unsigned char*)&hud->elem.glowColor.rgba);
}

void HudElem::SetText(const char* text)
{
    hud->elem.type = HE_TYPE_TEXT;
    G_SetLocalizedString(text, &hud->elem.text);
}

void HudElem::SetMaterial(const char* material)
{
    hud->elem.type = HE_TYPE_MATERIAL;
    hud->elem.materialIndex = G_MaterialIndex(material);
}

constexpr int alignOrg_mask = 3;
constexpr int alignOrg_shift = 2;
constexpr int alignScreen_mask = 15;
constexpr int alignScreen_shift = 4;

void HudElem::SetAlignOrg(he_align_x_t alignX, he_align_y_t alignY)
{
    hud->elem.alignOrg = (alignX << alignOrg_shift) | alignY;
}

void HudElem::SetAlignX(he_align_x_t alignX)
{
    constexpr int mask = alignOrg_mask;
    constexpr int shift = alignOrg_shift;

    hud->elem.alignOrg &= ~(mask << shift);
    hud->elem.alignOrg |= (alignX << shift);
}

void HudElem::SetAlignY(he_align_y_t alignY)
{
    constexpr int mask = alignOrg_mask;
    constexpr int shift = 0;

    hud->elem.alignOrg &= ~(mask << shift);
    hud->elem.alignOrg |= (alignY << shift);
}

void HudElem::SetAlignScreen(he_horzalign_t horzAlign, he_vertalign_t vertAlign)
{
    hud->elem.alignScreen = (horzAlign << alignScreen_shift) | vertAlign;
}

void HudElem::SetHorzAlign(he_horzalign_t horzAlign)
{
    constexpr int mask = alignScreen_mask;
    constexpr int shift = alignScreen_shift;

    hud->elem.alignScreen &= ~(mask << shift);
    hud->elem.alignScreen |= (horzAlign << shift);
}

void HudElem::SetVertAlign(he_vertalign_t vertAlign)
{
    constexpr int mask = alignScreen_mask;
    constexpr int shift = 0;

    hud->elem.alignScreen &= ~(mask << shift);
    hud->elem.alignScreen |= (vertAlign << shift);
}

void HudElem::SetSort(float sort)
{
    hud->elem.sort = sort;
}

HudElem* CreateTextHudElem(int client, const char* text, float scale, float x, float y, float sort, vec3_t rgb, float alpha)
{
    auto* hud = new HudElem(client);
    hud->SetText(text);
    hud->SetTextFont(HE_FONT_DEFAULT);
    hud->SetTextScale(scale);
    hud->MoveOvertime(x, y);
    hud->SetSort(sort);
    hud->SetRGB(rgb);
    hud->SetAlpha(alpha);
    hud->SetAlignOrg(HE_ALIGN_X_LEFT, HE_ALIGN_Y_TOP);
    hud->SetAlignScreen(HE_HORZALIGN_SUBLEFT, HE_VERTALIGN_SUBTOP);
    return hud;
}

HudElem* CreateMaterialHudElem(int client, const char* material, int width, int height, float x, float y, float sort, vec3_t rgb, float alpha)
{
    auto* hud = new HudElem(client);
    hud->SetMaterial(material);
    hud->ScaleOvertime(width, height);
    hud->MoveOvertime(x, y);
    hud->SetSort(sort);
    hud->SetRGB(rgb);
    hud->SetAlpha(alpha);
    hud->SetAlignOrg(HE_ALIGN_X_LEFT, HE_ALIGN_Y_TOP);
    hud->SetAlignScreen(HE_HORZALIGN_SUBLEFT, HE_VERTALIGN_SUBTOP);
    return hud;
}