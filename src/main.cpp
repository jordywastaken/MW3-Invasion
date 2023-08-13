
#include <sys/moduleexport.h>

#include "client/globals.hpp"
#include "client/game/game.hpp"
#include "client/components/hudelems.hpp"
#include "client/components/clients.hpp"
#include "client/components/scheduler.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/file.hpp"
#include "common/utils/imports.hpp"

#pragma diag_suppress 77
SYS_MODULE_INFO(iw5-mod, 0, 1, 1);
SYS_MODULE_START(entry_point);
SYS_MODULE_STOP(exit_point);

HudElem* test;

extern "C"
{
    int entry_point()
    {
        // utils
        hook::start();

        // components
        scheduler::start();
        clients::start();

        return 0;
    }

    int exit_point()
    {
        // utils 
        hook::stop();

        // components
        scheduler::stop();
        clients::stop();

        return 0;
    }
}
