#include "SnakeGameShared.hpp"

extern int g_max_x;
extern int g_max_y;
extern ofstream logfile;

extern Game GameObj;

void Game::PrintAnimated(string msg, int speed)
{
	for (int i = 0; i < msg.length(); i++)
	{
		cout << msg[i];
		cout.flush();
		usleep(speed);
	}
}

int Game::getSnakeIndexFromID(int id)
{
	for (int i = 0; i < allSnakes.size(); i++)
	{
		if (allSnakes[i].getID() == id)
			return (i);
	}
	return (id);
}

void Game::PrintScores()
{
	int s = 0;
	string temp;

	coloron(WHITE);
	for (int i = 0; i < allSnakes.size(); i++)
	{
		temp = allSnakes[i].mPlayerName;
		mvprintw(0, s, "%s:%d", temp.c_str(), allSnakes[i].getScore());
		s += temp.length() + 4;
	}
	coloroff(WHITE);
}

void Game::AskNoPlayers(string player)
{
	char ch;

	for (;;)
	{
		ch = getch();
		if (ch == 27)
		{
			clear();
			getch();
			ch = getch();
			if (ch == 'A')
				player = "single";
			else if (ch == 'B')
				player = "multi";
		}
		else if (ch == 10)
		{
			GameObj.setGameMode(player);
			break;
		}

		coloron(BLUE);
		mvprintw(1, 5, "SINGLE PLAYER or");
		coloroff(BLUE);
		coloron(GREEN);
		mvprintw(1 + 1, 5, "MULTI PLAYER ? ");
		coloroff(GREEN);

		if (player == "single")
			mvprintw(1, 2, ">>>");
		else if (player == "multi")
			mvprintw(2, 2, ">>>");
		
		refresh();
		usleep(20000);
		clear();
	}

	if (GameObj.getGameMode() == "multi")
	{
		flushinp();
		GameObj.InitConsoleScreen("off");
		system("clear");
		int no_of_players;
		cout << "Enter the number of players on this device (any number of LAN clients can join this play) : ";
		cin >> no_of_players;
		GameObj.setNoOfPlayers(no_of_players);
	}

	if (GameObj.getNoOfPlayers())
	{
		Snake first_snake = Snake('A', 'B', 'C', 'D', 0);
		first_snake.setPlayerName("Default");
		GameObj.allSnakes.push_back(first_snake);
	}

	for (int i = 1; i < GameObj.getNoOfPlayers(); i++)
	{
		char left, right, up, down;
		cout << "Player " << i << " = Enter the key for Left Right Up Down : ";
		cin >> left >> right >> up >> down;

		Snake* snk = new Snake(up, down, right, left, -i);
		GameObj.allSnakes.push_back(*snk);
	}

	GameObj.InitConsoleScreen("on");
}

void Game::KeyPressHandler()
{
	char ch;
	if ((ch = getch()) != ERR)
	{
		if (ch == 27)
		{
			getch();
			ch = getch();
			if (GameObj.getNoOfPlayers())
				allSnakes[0].HandleMovementKeyPress(ch);
		}

		if (GameObj.getGameMode() == "multi")
		{
			for (int i = 0; i < allSnakes.size(); i++)
			{
				if (allSnakes[i].getSocketDescriptor() < 0 && allSnakes[i].getID() < 0)
					allSnakes[i].HandleMovementKeyPress(ch);
			}
		}

		if (ch == 45)
		{
			GameObj.setSpeed(GameObj.getSpeed() + 3000);
			if (GameObj.getGameMode() == "multi")
			{
				for (int i = 0; i < allSnakes.size(); i++)
				{
					int sd = allSnakes[i].getSocketDescriptor();
					if (sd > 0)
						mServer.SendData(sd, "-");
				}
			}
		}

		if (ch == 43)
		{
			GameObj.setSpeed(GameObj.getSpeed() - 3000);
			if (GameObj.getGameMode() == "multi")
			{
				for (int i = 0; i < allSnakes.size(); i++)
				{
					int sd = allSnakes[i].getSocketDescriptor();
					if (sd > 0)
						mServer.SendData(sd, "+");
				}
			}
		}
	}
}

void Game::DrawAllSnakes()
{
	for (int i = 0; i < allSnakes.size(); i++)
	{
		allSnakes[i].AddPart(mCenterX + 5, mCenterY);
		allSnakes[i].AddPart(mCenterX + 6, mCenterY);
		allSnakes[i].AddPart(mCenterX + 7, mCenterY);
		allSnakes[i].DrawSnake();
	}
}

void Game::MoveAllSnakes()
{
	for (int i = 0; i < allSnakes.size(); i++)
	{
		allSnakes[i].MoveSnake(allSnakes[i].getDirection());
	}
}

void Game::setFoodPos(int x, int y)
{
	mFoodObj.x = x;
	mFoodObj.y = y;
}

void Game::InitConsoleScreen(string state)
{
	if (state == "on")
	{
		initscr(); // Init Screen
		noecho(); // Don't show any pressed char
		curs_set(false); // Don't show the cursor
		start_color();
		getmaxyx(stdscr, g_max_y, g_max_x);
		mCenterX = g_max_x / 2, mCenterY = g_max_y / 2;
		cbreak(); // Don't wait for enter to be pressed when using getch
		nodelay(stdscr, 1);
	}
	else if (state == "off")
	{
		clear();
		flushinp();
		fflush(stdin);
		use_default_colors();
		endwin();
		system("clear");
	}
}

food Game::getFoodPos()
{
	food obj = mFoodObj;
	return (obj);
}

void Game::LANSendFoodCoordinates(int x, int y)
{
	for (int i = 0; i < allSnakes.size(); i++)
	{
		int sd = allSnakes[i].getSocketDescriptor();

		if (sd > 0)
		{
			char foodCoord[20];
			sprintf(foodCoord, ":%03d,%03d", x, y);

			mServer.SendData(sd, string(foodCoord));
		}
	}
}

void Game::GenerateFood()
{
	int x = random() % g_max_x, y = random() % g_max_y;

	if (x < 2)
		x = 2;
	if (y < 2)
		y = 2;

	logfile << "\n\nGenerating food at pos:" << x << "," << y << "\n\n";
	coloron(RED);
	mvprintw(y, x, "#");
	coloroff(RED);
	setFoodPos(x, y);

	if (mGameMode == "multi")
		LANSendFoodCoordinates(getFoodX(), getFoodY());
}

void Game::PrintFood(string status)
{
	if (status == "new")
		GenerateFood();
	
	if (!getFoodX() && !getFoodY())
		GenerateFood();
	coloron(RED);
	mvprintw(getFoodY(), getFoodX(), "#");
	coloroff(RED);
}

void Game::InitServerForMultiplayer()
{
	mServer.BindServer();
	mServer.SetupClientDescriptors();
	mServer.StartServer();
}

void Game::HandleNewConnection()
{
	int client_socket = mServer.HandleNewConnection();
	Snake* clientSnakePtr = new Snake('A', 'B', 'C', 'D', 0, client_socket);
	GameObj.allSnakes.push_back(*clientSnakePtr);
	GameObj.setNoOfPlayers(GameObj.getNoOfPlayers() + 1);

	allSnakes[allSnakes.size() - 1].AddPart(mCenterX + 5, mCenterY);
	allSnakes[allSnakes.size() - 1].AddPart(mCenterX + 6, mCenterY);
	allSnakes[allSnakes.size() - 1].AddPart(mCenterX + 7, mCenterY);

	GameObj.GenerateFood();
}

void Game::SyncSnakeWithClient(Snake& clientSnake)
{
	int x_diff = getFoodX() - clientSnake.getHeadX();
	int y_diff = getFoodY() - clientSnake.getHeadY();

	for (int i = 0; i < clientSnake.mParts.size(); i++)
	{
		clientSnake.mParts[i].x += x_diff;
		clientSnake.mParts[i].y += y_diff;
	}
}

void Game::HandleIOActivity()
{
	string msg;
	int snake_index;

	for (int i = 0; i < mClients.size(); i++)
	{
		snake_index = getSnakeIndexFromDescriptor(mClients[i]);

		msg = mServer.HandleIOActivity(mClients[i]);

		if (msg == "")
		{
			GameObj.InitConsoleScreen("off");
			system("clear");
			cout << allSnakes[snake_index].mPlayerName << " left the game.\n";
			cout << "Score : " << allSnakes[snake_index].mScore << "\n\n";
			GameObj.mServer.CloseSocket(mClients[i]);
			allSnakes.erase(allSnakes.begin() + snake_index);
			setNoOfPlayers(getNoOfPlayers() - 1);
			sleep(2);
			GameObj.InitConsoleScreen("on");
		}
		else if (msg.find("init~~") != string::npos)
		{
			LANSendFoodCoordinates(mFoodObj.x, mFoodObj.y);
			string name = "", sight = "";
			int pos = msg.find("init~~");
			int j;

			for (j = 6; msg[j] != '~'; j++)
				name += msg[j];

			sight = msg[j + 2];

			logfile << endl << "/nName of player : " << name << " , sight = " << sight << endl << endl;

			GameObj.InitConsoleScreen("off");
			system("clear");
			cout << "\n\n" << name << " joined the game.\n\n";
			sleep(1);
			system("clear");
			GameObj.InitConsoleScreen("on");
			allSnakes[snake_index].setPlayerName(name);
			allSnakes[snake_index].setPlayerSight(sight);
			allSnakes[snake_index].setBodyColor(int(msg[msg.find("&") + 1] - '0'));
		}
		else if (msg.find("$") != string::npos)
		{
			int start = msg.find("$");
			allSnakes[snake_index].GameOverHandler();
		}
		else
		{
			for (int j = 0; j < msg.length(); j++)
			{
				if (msg[j] == '#')
				{
					logfile << "\nMessage from client handler :\n" << msg << "\n";

					allSnakes[snake_index].AddPart(allSnakes[snake_index].getHeadX(), allSnakes[snake_index].getHeadY());

					allSnakes[snake_index].mScore++;

					SyncSnakeWithClient(allSnakes[snake_index]);

					PrintFood("new");

					if (mGameMode == "multi")
						LANSendFoodCoordinates(getFoodX(), getFoodY());
				}
				else if (msg[j] == 'A' || msg[j] == 'B' || msg[j] == 'C' || msg[j] == 'D')
					allSnakes[snake_index].HandleMovementKeyPress(msg[j]);
			}
			logfile << "\n\nMessage from client : " << msg << endl << endl;
		}
	}
}

int Game::getSnakeIndexFromDescriptor(int sd)
{
	for (int i = 0; i < allSnakes.size(); i++)
	{
		if (sd == allSnakes[i].getSocketDescriptor())
			return (i);
	}
	return (sd);
}

void Game::HandleActivity()
{
	mClients = mServer.HandleActivity();

	if (mClients[0] == -1)
		HandleNewConnection();
	else
		HandleIOActivity();
}

int Game::CheckClientActivity()
{
	return (mServer.CheckClientActivity());
}

void Game::ResetMaxScreen()
{
	getmaxyx(stdscr, g_max_y, g_max_x);
}

void Game::ShowInitialChoices()
{
	
    system("clear") ; 
    cout << "\n1. Start Game *"  ; 
    cout << "\n2. Show Features" ; 
    cout << "\n3. Exit " ;
    cout << "\n\nEnter choice : " ; 
    int ch ; 
    cin >> ch ; 
    string feat="" ;  //features string 

	switch(ch)
    {
        case 1:
            return ;   
        case 2:
            feat += "\nGame has two related programs , this is the Game Controller (main program) and other one is the client program which can be used to join the multiplayer mode of the game in a LAN network ." ; 
            feat += "\n\nHere are the features of the Game Controller : " ;
            feat += "\n1. Different Modes : \"Single\" or \"Multiplayer\" modes." ; 
            feat += "\n2. Parallel Control : Parallel controlling of all the snakes in multiplayer mode without any delay. This greatly ups user experience."  ; 
            feat += "\n3. Speed Control : User can change the speed of the snakes while playing." ; 
            feat += "\n4. Scoring System : It has a scoring system for both single and multiplayer modes." ; 
            feat += "\n5. Looping screen :  On going through the edge of screen , the snake pops out of the opposite edge." ; 
            feat += "\n6. Beautiful colors: The snakes will be presented with good looking colors." ; 

            feat += "\n\n\nComing to Client Side Program : " ; 
            feat += "\n\n7. LAN MULTIPLAYER option: Play with anyone in the same network and see the snakes in both the screen play in synchronization." ; 
            feat += "\n8. Speed Changes in the Game Controller reflects back at the client side too . " ; 
            feat += "\n9. Food generated at the server side and client side are also synced." ; 
            feat += "\n10. On gameover of any client , the players are notified of that player's final score which is shown at the Game Controller." ; 
            feat += "\n11. Client user can either choose to have his snake displayed in his screen in addition to the snake on the game controller or to only control the snake on game controller without anything displayed on his screen. This is useful , if the client only wants to use his keyboard as a controller." ; 

            GameObj.PrintAnimated(feat , 10000) ; 

            cout << "\n\nGo back ? (y) : " ; 
            char temp ; 
            cin >> temp ; 
            break; 
        case 3:
            exit(5) ; 
        default:
            return ; 
    }

	ShowInitialChoices();
}

