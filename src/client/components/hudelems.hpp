
#pragma once

#include "client/game/game.hpp"

enum he_align_x_t
{
	HE_ALIGN_X_LEFT,
	HE_ALIGN_X_CENTER,
	HE_ALIGN_X_RIGHT
};

enum he_align_y_t
{
	HE_ALIGN_Y_TOP,
	HE_ALIGN_Y_MIDDLE,
	HE_ALIGN_Y_BOTTOM
};

enum he_horzalign_t
{
	HE_HORZALIGN_SUBLEFT,
	HE_HORZALIGN_LEFT,
	HE_HORZALIGN_CENTER,
	HE_HORZALIGN_RIGHT,
	HE_HORZALIGN_FULLSCREEN,
	HE_HORZALIGN_NOSCALE,
	HE_HORZALIGN_ALIGN_TO_640,
	HE_HORZALIGN_CENTER_SAFE_AREA,
	HE_HORZALIGN_USER_LEFT,
	HE_HORZALIGN_USER_CENTER,
	HE_HORZALIGN_USER_RIGHT
};

enum he_vertalign_t
{
	HE_VERTALIGN_SUBTOP,
	HE_VERTALIGN_TOP,
	HE_VERTALIGN_MIDDLE,
	HE_VERTALIGN_BOTTOM,
	HE_VERTALIGN_FULLSCREEN,
	HE_VERTALIGN_NOSCALE,
	HE_VERTALIGN_ALIGN_TO_480,
	HE_VERTALIGN_CENTER_SAFE_AREA,
	HE_VERTALIGN_USER_TOP,
	HE_VERTALIGN_USER_CENTER,
	HE_VERTALIGN_USER_BOTTOM
};

class HudElem
{
public:
    static game_hudelem_s* Alloc(int client);
    static void Destroy(game_hudelem_s* hud);

    HudElem(int client);
    ~HudElem();

    void SetDefaults();

    void MoveOvertime(float x, float y, int durationMs = 0, int startDelayMs = 0);
    void SetX(float x, int durationMs = 0, int startDelayMs = 0);
    void SetY(float y, int durationMs = 0, int startDelayMs = 0);
	void AddX(float x, int durationMs = 0, int startDelayMs = 0);
	void AddY(float y, int durationMs = 0, int startDelayMs = 0);

    void ScaleOvertime(int width, int height, int durationMs = 0, int startDelayMs = 0);
    void SetWidth(int width, int durationMs = 0, int startDelayMs = 0);
    void SetHeight(int height, int durationMs = 0, int startDelayMs = 0);
	void AddWidth(int width, int durationMs = 0, int startDelayMs = 0);
	void AddHeight(int height, int durationMs = 0, int startDelayMs = 0);

	void FadeOvertime(vec4_t color, int durationMs = 0, int startDelayMs = 0);
	void SetRGB(vec3_t rgb, int durationMs = 0, int startDelayMs = 0);
	void SetAlpha(float alpha, int durationMs = 0, int startDelayMs = 0);

    void SetTextScale(float scale, int durationMs = 0, int startDelayMs = 0);
    void SetTextFont(he_font_t font);
	void SetTextGlow(vec3_t rgb, float alpha = 1.0);

    void SetText(const char* text);
	void SetMaterial(const char* material);

	void SetAlignOrg(he_align_x_t alignX, he_align_y_t alignY);
	void SetAlignX(he_align_x_t alignX);
	void SetAlignY(he_align_y_t alignY);

	void SetAlignScreen(he_horzalign_t horzAlign, he_vertalign_t vertAlign);
	void SetHorzAlign(he_horzalign_t horzAlign);
	void SetVertAlign(he_vertalign_t vertAlign);

	void SetSort(float sort);

    game_hudelem_s* hud;
};

HudElem* CreateTextHudElem(int client, const char* text, float scale, float x, float y, float sort, vec3_t rgb, float alpha);
HudElem* CreateMaterialHudElem(int client, const char* material, int width, int height, float x, float y, float sort, vec3_t rgb, float alpha);