#include "SnakeGameShared.hpp"

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

void HANDLE_EVERYTHING_TILL_EVENT_LOOP()
{
    srand(time(NULL));
    GameObj.InitConsoleScreen("on"); 

    init_pair(RED, COLOR_RED, COLOR_BLACK); 
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK); 
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK); 
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN , COLOR_CYAN, COLOR_BLACK); 

    //Asks the number of players who want to play  (single or multiplayer (with 2 players))
    GameObj.AskNoPlayers(); 

    GameObj.DrawAllSnakes(); 
}

void  SignalHandler(int code)
{
    GameObj.InitConsoleScreen("off"); 
    cout.flush();
    stringstream out; 
    out << "1.Continue" << endl; 
    out << "2.Exit" << endl; 
    out << "\nEnter choice : " << endl; 
    cout << out.str(); 
    int ch; 
    cin >> ch; 
    string credits = ""; 

    switch(ch)
    {
        case 2:
            GameObj.getServer()->StopServer();
            logfile.close(); 
            GameObj.PrintAnimated("\nThanks for playing ^_^ !!"); 
            exit(1); 
            break; 
        case 1:
            GameObj.InitConsoleScreen("on"); 
            break; 
        default:
            GameObj.getServer()->StopServer(); 
            logfile.close(); 
            exit(1);
    }
}