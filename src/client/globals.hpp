
#pragma once

class variables
{
public:
    float fRecoilTweak{ 1.0 };
    float fSpreadTweak{ 1.0 };

    float fMenuX{ 100.0 };
    float fMenuY{ 100.0 };
    float fMenuWidth{ 300.0 };
    float fMenuMargin{ 6.0 };
    float fMenuHeaderSize{ 25.0 };
    float fMenuTitleSize{ 22.0 };
    float fMenuOptionSize{ 20.0 };

    float colorAccent[3]{};
    float colorAccentHSV[3]{ 200.0, 1.0, 1.0 };
};

extern variables* g_vars;

extern float colorWhite[3];
extern float colorBlack[3];

namespace globals
{
    void start();
    void stop();
}