#include "pch.h"
#include "driver.h"
#include "Header.h"

bool menuSwitch::toMenu()
{
    menuScreen mW;
    return mW.menuWindow();

}

bool menuSwitch::toGame()
{
    gameScreen gW;
    return gW.gameWindow();
}

int main(int argc, const char** argv)
{
    source s;
    menuSwitch mS;
    //mS.toGame();
    s.runOpenCV(argc, argv, s);

    return 0;
}
