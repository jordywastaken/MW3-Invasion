
#pragma once

namespace
{
    /***************************************************************
     * Functions
     **************************************************************/

    
    // libc
    static symbol<int(const char* fmt, ...)> printf{ 0x4370F4 };
    static symbol<const char* (const char* fmt, ...)> va{ 0x281470 };

    // universal
    static symbol<int()> Sys_Milliseconds{ 0x1E339C };
    static symbol<unsigned int(const char* str, int user)> SL_GetString{ 0x229078 };
    static symbol<const char* (unsigned int stringValue)> SL_ConvertToString{ 0x228630 };
    static symbol<void(const float* angles, float* forward, float* right, float* up)> AngleVectors{ 0x2723E8 };
    static symbol<dvar_t* (const char* name)> Dvar_FindVar{ 0x279380 };
    static symbol<void(const char* name, const char* value)> Dvar_SetFromStringByName{ 0x27D48C };
    static symbol<int(int min, int max)> G_irand{ 0x18BE98 };
    static symbol<float(float min, float max)> G_flrand{ 0x18BE38 };

    // assets
    static symbol<void(XAssetHeader* header, int type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{ 0xBB26C };

    // server
    static symbol<void(const char* string, int* entry)> G_SetLocalizedString{ 0x188EBC };
    static symbol<int(const char* string)> G_MaterialIndex{ 0x189008 };
    static symbol<void(trace_t* results, const float* start, const float* end, int passEntityNum, int contentmask, unsigned char* priorityMap)> G_LocationalTrace{ 0x14ECC0 };
    static symbol<void(int arg, char* buffer, int bufferLength)> SV_Cmd_ArgvBuffer{ 0x2025C4 };
    static symbol<void(int client, const char* cmd)> SV_GameSendServerCommand{ 0x2D94C8 };
    static symbol<void(gentity_s* ent, float* origin)> SetClientOrigin{ 0x132968 };
    static symbol<bool(gentity_s* ent, gentity_s* parent, unsigned int tagName)> G_EntLinkTo{ 0x18FEA8 };
    static symbol<void(gentity_s* ent)> G_EntUnlink{ 0x18FF80 };
    static symbol<void(gentity_s* ent)> G_InitPlayerLinkAngles{ 0x1909D8 };

    // game script
    static symbol<void(int value)> Scr_AddInt{ 0x238A28 };
    static symbol<void(float value)> Scr_AddFloat{ 0x238B10 };
    static symbol<void(const char* value)> Scr_AddString{ 0x2390A0 };
    static symbol<void(const float* value)> Scr_AddVector{ 0x23939C };
    static symbol<void(const gentity_s* value)> Scr_AddEntity{ 0x182EF8 };
    static symbol<void(int type, VariableUnion* u)> RemoveRefToValue{ 0x22A398 };
    static symbol<unsigned int(unsigned int threadId)> Scr_GetSelf{ 0x22BA0C };

    // weapon & perks
    static symbol<bool(playerState_s* ps, const Weapon weapon, int dualWield)> G_GivePlayerWeapon{ 0x18E3BC };
    static symbol<void(int clientNum, const Weapon weapon)> G_SelectWeapon{ 0x18EFCC };
    static symbol<void(Weapon* weapon, const char* name)> G_GetWeaponForName{ 0x18E82C };
    static symbol<void(gentity_s* ent, const Weapon weapon, bool isAlternate, int count, int fillClip)> Add_Ammo{ 0x145B34 };
    static symbol<void(playerState_s* ps, const Weapon weapon, bool isAlternate)> Fill_Clip{ 0x145878 };
    static symbol<bool(playerState_s* ps, const Weapon weapon)> BG_TakePlayerWeapon{ 0x2BE018 };
    static symbol<void(const Weapon weapon, bool isAlternate, char* output, unsigned int maxStringLen)> BG_GetWeaponNameComplete{ 0x2BBD64 };
    static symbol<bool(const Weapon weapon)> BG_HasUnderbarrelAmmo{ 0x2BCD38 };
    static symbol<weapFireType_t(const Weapon weapon, bool isAlternate)> BG_GetWeaponFireType { 0x2C1E48 };
    static symbol<int(const char* perkName)> BG_GetPerkCodeIndexForName{ 0x2A35E8 };

    // renderer
    static symbol<void(float* color, unsigned char* bytes)> R_ConvertColorToBytes{ 0x374978 };

    // client
    static symbol<void(int localClientNum, const char* cmd)> Cbuf_AddText{ 0x201488 };
    static symbol<bool(int localClientNum)> CL_IsLocalClientInGame{ 0x81658 };

    /***************************************************************
     * Variables
     **************************************************************/

    static game_hudelem_s* g_hudelems = reinterpret_cast<game_hudelem_s*>(0x10DB51C);
    static gentity_s* g_entities = reinterpret_cast<gentity_s*>(0x10EEAD8);
    static level_locals_t& level = *reinterpret_cast<level_locals_t*>(0x10E6128);
    static scrVmPub_t& scrVmPub = *reinterpret_cast<scrVmPub_t*>(0x1D7C0C8);

    static WeaponDef** bg_weaponDefs = reinterpret_cast<WeaponDef**>(0x1EC8D44);
    static WeaponCompleteDef** bg_weaponCompleteDefs = reinterpret_cast<WeaponCompleteDef**>(0x1EC8984);
    static const char** bg_perkNames = reinterpret_cast<const char**>(0x6AEA00);
}