#include "SnakeClientShared.hpp"

extern int g_max_x;
extern int g_max_y;
extern ofstream logfile;

extern Game GameObj;

void DrawBorderWindow(int max_x ,int max_y)
{
    for(int i = 0 , j = 1; (i < max_x || j < max_y ); )
    {
        if(i < max_x)
            mvprintw(1, i++, "--");
        if(j < max_y)
            mvprintw(j++, 0, "|"); 
    }

    max_x -= 1;
	max_y -= 1; 

    for(int i = max_x , j = max_y; (i > 0 || j > 0); )
    {
        if(i > 0)
            mvprintw(max_y, i--, "--"); 
        if(j > 0)
            mvprintw(j--, max_x, "|"); 
    }
}

void PrintSpeed(Snake snk)
{
    coloron(CYAN); 
    mvprintw(0 , g_max_x - 15 , "Speed(+/-) = %lu" ,GameObj.getSpeed() / 100) ; 
    refresh();
    coloroff(CYAN); 
}

void SignalHandler(int code)
{
    GameObj.InitConsoleScreen("off"); 
    GameObj.PrintAnimated("\nThanks for playing :) Cya ..."); 
    GameObj.mSockObj.CloseSocket(); 
    logfile.close(); 
    exit(1); 
}