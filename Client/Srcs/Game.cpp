#include "SnakeClientShared.hpp"

extern int g_max_x;
extern int g_max_y;
extern ofstream logfile;

extern Game GameObj;

food Game::getFoodPos()
{
	food obj = mFoodObj;
	return (obj);
}

void Game::setFoodPos(int x, int y)
{
	mFoodObj.x = x;
	mFoodObj.y = y;
}

void Game::HandleMessageFromServer(string msg)
{
	logfile << "\nServer sent msg : " << msg << "\n";

	if (msg.find(":") != string::npos)
	{
		int start_colon = msg.find(":");

		int comma = msg.find(",");
		string str_x = msg.substr(start_colon + 1, comma - 1);
		string str_y = msg.substr(comma + 1, 3);

		int x = stoi(str_x);
		int y = stoi(str_y);

		GameObj.setFoodPos(x, y);
	}

	if (msg.find("$") != string::npos)
	{
		int start = msg.find("$");
		string num = "";
		for (int i = start + 1; msg[i] != '$'; i++)
			num += msg[i];
		mMainSnakePtr->setScore(stoi(num));
		mMainSnakePtr->GameOverHandler();
	}
	else
	{
		for (int i = 0; i < msg.length(); i++)
		{
			if (msg[i] == '-')
				GameObj.setSpeed(GameObj.getSpeed() + 3000);
			else if (msg[i] == '+')
				GameObj.setSpeed(GameObj.getSpeed() - 3000);
		}
	}
}

void Game::InitConsoleScreen(string state)
{
	if(state == "on")
    {
       	initscr(); // Init Screen
        noecho(); // Dont show any pressed char  
        curs_set(false); // Don't show the cursor 
        start_color(); 
        getmaxyx(stdscr, g_max_y, g_max_x); 
    	mCenterX = g_max_x / 2, mCenterY = g_max_y / 2; 
        cbreak(); //Dont wait for enter to be pressed when using getch 
        nodelay(stdscr , 1); //Use non blocking input for getch which just returns ERR if there is no input (ERR=-1)
    }
    else if(state == "off"){
        clear(); 
        flushinp(); 
        fflush(stdin); 
        use_default_colors(); 
        endwin(); 
        cout.flush(); 
        system("clear");
    }
}

void Game::GenerateFood()
{
	int x = random() % g_max_x, y = random() % g_max_y;

	if (!x)
		x = 2;
	if (!y)
		y = 2;

	coloron(RED);
	mvprintw(y, x, "#");
	coloroff(RED);
	setFoodPos(x, y);
}

void Game::PrintFood(string status)
{
	if (status == "new")
		GameObj.GenerateFood();
	
	if (!getFoodX() && !getFoodY())
		GameObj.GenerateFood();
	coloron(RED);
	mvprintw(getFoodY(), getFoodX(), "#");
	coloroff(RED);
}

void Game::PrintAnimated(string msg, int speed)
{
	for(int c = 0  ; msg[c] ; c++)
	{
		cout << msg[c]; 
		cout.flush(); 
		usleep(speed); 
	}
}

void Game::InitColors()
{
    init_pair(RED , COLOR_RED , COLOR_BLACK); 
    init_pair(YELLOW , COLOR_YELLOW , COLOR_BLACK); 
    init_pair(GREEN , COLOR_GREEN , COLOR_BLACK);
    init_pair(WHITE , COLOR_WHITE , COLOR_BLACK); 
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CYAN , COLOR_CYAN , COLOR_BLACK); 
}

void Game::ResetMaxScreen()
{
    getmaxyx(stdscr, g_max_y, g_max_x); 
}