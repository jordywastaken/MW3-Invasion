
#pragma once

namespace scheduler
{
    enum pipeline_t
    {
        // Asynchronuous pipeline, disconnected from the game
        async,

        // The game's hud rendering pipeline
        hud,

        // The game's rendering pipeline
        renderer,

        count
    };

    void start();
    void stop();
    void add(pipeline_t thread, void(*handler)(), int interval = 0);
}