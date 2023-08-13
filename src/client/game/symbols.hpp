
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

    // assets
    static symbol<void(XAssetHeader* header, int type, const char* name, int allowCreateDefault)> DB_FindXAssetHeader{ 0xBB26C };

    // server
    static symbol<void(const char* string, int* entry)> G_SetLocalizedString{ 0x188EBC };
    static symbol<int(const char* string)> G_MaterialIndex{ 0x189008 };
    static symbol<void(int arg, char* buffer, int bufferLength)> SV_Cmd_ArgvBuffer{ 0x2025C4 };

    // renderer
    static symbol<void(float* color, unsigned char* bytes)> R_ConvertColorToBytes{ 0x374978 };

    /***************************************************************
     * Variables
     **************************************************************/

    static game_hudelem_s* g_hudelems = (game_hudelem_s*)(0x10DB51C);
    static level_locals_t& level = *(level_locals_t*)(0x10E6128);
}