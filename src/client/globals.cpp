
#include "globals.hpp"
#include "common/utils/imports.hpp"

variables* g_vars;

float colorWhite[3]{ 1.0, 1.0, 1.0 };
float colorBlack[3]{ 0.0, 0.0, 0.0 };

namespace globals
{
    void start()
    {
        g_vars = new variables();
    }

    void stop()
    {
        _sys_memset(g_vars, 0, sizeof(variables));
        delete g_vars;
    }
}