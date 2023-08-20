
#include <sys/moduleexport.h>

#include "client/globals.hpp"
#include "client/game/game.hpp"
#include "client/components/hudelems.hpp"
#include "client/components/clients.hpp"
#include "client/components/scheduler.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/file.hpp"
#include "common/utils/imports.hpp"

///
/// Module exports
/// 
#pragma diag_suppress 77
SYS_MODULE_INFO(Invasion, 0, 1, 1);
SYS_MODULE_START(_Z13InvasionStartv);
SYS_MODULE_STOP(_Z12InvasionStopv);

int InvasionStart()
{
    debug_print("[Invasion]: Started\n");

    // utils
    hook::start(init_proc, 20);

    // components
    scheduler::start();
    clients::start();

    return 0;
}

int InvasionStop()
{
    debug_print("[Invasion]: Stopped\n");

    // utils 
    hook::stop();

    // components
    scheduler::stop();
    clients::stop();

    return 0;
}