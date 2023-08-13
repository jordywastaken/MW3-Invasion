
#include "scheduler.hpp"
#include "client/game/game.hpp"
#include "common/utils/hooking.hpp"
#include "common/utils/imports.hpp"
#include "common/utils/thread.hpp"
#include <sys/sys_time.h>

namespace scheduler
{
    static constexpr int taskStackMax = (10);

    struct task_t
    {
        void(*handler)();
        int interval;
        int lastCall;
    };

    struct task_pipeline_t
    {
        task_t stack[taskStackMax];

        void add(void(*handler)(), int interval)
        {
            for (int i = 0; i < taskStackMax; i++)
            {
                if (stack[i].handler)
                    continue;

                stack[i].handler = handler;
                stack[i].interval = interval;
                stack[i].lastCall = -1;
                return;
            }
        }

        void execute()
        {
            int currentTime = Sys_Milliseconds();
            for (int i = 0; i < taskStackMax; i++)
            {
                if (!stack[i].handler)
                    continue;

                if (currentTime > stack[i].lastCall + stack[i].interval)
                {
                    stack[i].handler();
                    stack[i].lastCall = currentTime;
                }
            }
        }
    };

    bool kill = false;
    task_pipeline_t pipelines[count];
    thread asyncThread;

    void CG_DrawNightVisionOverlay_Detour(HookContext_t* ctx)
    {
        // mr %r31, %r3;
        ctx->r31 = ctx->r3;

        pipelines[hud].execute();
    }

    void R_EndFrame_Detour(HookContext_t* ctx)
    {
        // bl nullsub

        pipelines[renderer].execute();
    }

    void start()
    {
        asyncThread = thread([]
            {
                while (!kill)
                {
                    pipelines[async].execute();

                    // we need to sleep a little bit, else kernel gets angy
                    sys_timer_usleep(1337);
                }

            }, "Async Scheduler", 420, 24 * 1024);

        hook::inject(0x1DCB54, [](HookContext_t* ctx)
            {
                ctx->r6 = ctx->r3;

                switch (ctx->r3)
                {
                case(0x121ULL):
                case(0x131ULL):
                    ctx->lr = 0x1DCC90ULL;
                    break;
                }
            });
    }

    void stop()
    {
        kill = true;
        asyncThread.join();
    }

    void add(pipeline_t thread, void(*handler)(), int interval)
    {
        pipelines[thread].add(handler, interval);
    }
}