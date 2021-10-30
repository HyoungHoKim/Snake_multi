#include "SnakeGameShared.hpp"

extern int g_max_x;
extern int g_max_y;
extern ofstream logfile;

extern Game GameObj;

Snake::Snake(char keyUp, char keyDown, char keyRight, char keyLeft,
		  int id, int sockFD,
		  string playerName, string playerSight) :
	mKeyUp(keyUp), mKeyDown(keyDown), mKeyRight(keyRight), mKeyLeft(keyLeft),
	mID(id), mSockFD(sockFD), 
	mPlayerSight(playerSight)
{
	mScore = 0;
	mSnakeDirection = "right";
	mID = id;
	mSockFD = sockFD;
	mPlayerName = playerName + std::to_string(-1 * id);
	mBodyColor = rand() % 5 + 1;
	logfile << "\n\nbody color : " << mBodyColor << endl << endl;
}

void Snake::DrawSnake(void)
{
	int i;
	coloron(mBodyColor);
	for (i = 0; i < mParts.size() - 1; i++)
		mvprintw(mParts[i].y, mParts[i].x, "o");
	coloroff(mBodyColor);
	mvprintw(mParts[i].y, mParts[i].x, "+");
}

void Snake::AddPart(int x, int y, string dir)
{
	Snake_part obj(x, y);
	if (dir == "right")
		mParts.push_back(obj);
	else if (dir == "left")
		mParts.insert(mParts.begin(), obj);
}

void Snake::InitSnakeOnScreen()
{
	AddPart(GameObj.getCenterX(), GameObj.getCenterY());
	AddPart(GameObj.getCenterX() + 1, GameObj.getCenterY());
	AddPart(GameObj.getCenterX() + 2, GameObj.getCenterY());

	DrawSnake();
}

void Snake::MoveSnake(string dir)
{
	mParts.erase(mParts.begin());
	Snake_part last_part = mParts.at(mParts.size() - 1);

	if (dir == "right")
	{
		AddPart((last_part.x + 1) % g_max_x, last_part.y);
		mSnakeDirection = "right";
	}
	else if (dir == "left")
	{
		AddPart((last_part.x - 1) < 0 ? g_max_x - 1 : (last_part.x - 1), last_part.y);
		mSnakeDirection = "left";
	}
	else if (dir == "up")
	{
		AddPart(last_part.x, (last_part.y - 1) < 0 ? g_max_y - 1 : (last_part.y - 1));
		mSnakeDirection = "up";
	}
	else if (dir == "down")
	{
		AddPart(last_part.x, (last_part.y + 1) % g_max_y);
		mSnakeDirection = "down";
	}

	CheckSnakeOverlap();

	if (getHeadX() == GameObj.getFoodX() && getHeadY() == GameObj.getFoodY())
	{
		if (mSockFD < 0 || mPlayerSight == "s")
		{
			AddPart(GameObj.getFoodX(), GameObj.getFoodY());
			setScore(getScore() + 1);
			GameObj.PrintFood("new");
		}
	}
	DrawSnake();
	refresh();
}

void Snake::CheckSnakeOverlap()
{
	int headX = getHeadX(), headY = getHeadY();
	for (int i = 0; i < mParts.size() - 1; i++)
	{
		if (mParts[i].x == headX && mParts[i].y == headY)
		{
			if (mSockFD > 0 && mPlayerSight == "c")
				return ;
			if (mPlayerSight == "s")
				GameObj.mServer.SendData(mSockFD, "$" + std::to_string(mScore) + "$");
			GameOverHandler();
		}
	}
}

void Snake::HandleMovementKeyPress(char ch)
{
	if (mKeyUp == ch)
	{
		if (getDirection() != "down")
			MoveSnake("up");
	}
	else if (mKeyDown == ch)
	{
		if (getDirection() != "up")
			MoveSnake("down");
	}
	else if (mKeyRight == ch)
	{
		if (getDirection() != "left")
			MoveSnake("right");
	}
	else if (mKeyLeft == ch)
	{
		if (getDirection() != "right")
		MoveSnake("left");
	}
	else
		return ;
}

void Snake::GameOverHandler()
{
	clear();
	GameObj.InitConsoleScreen("off");
	system("clear");
	string game_over_message = "\n\n\nGAME OVER FOR " + mPlayerName + " :(\n\n";
	game_over_message += "Score : " + std::to_string(mScore) + "\nBetter Luck Next time :)\n\n";

	GameObj.PrintAnimated(game_over_message);

	if (mSockFD > 0)
	{
		GameObj.allSnakes.erase(GameObj.allSnakes.begin() + GameObj.getSnakeIndexFromDescriptor(mSockFD));
		GameObj.mServer.CloseSocket(mSockFD);
	}
	else
	{
		GameObj.allSnakes.erase(GameObj.allSnakes.begin() + GameObj.getSnakeIndexFromID(mID)); 
	}

	GameObj.setNoOfPlayers(GameObj.getNoOfPlayers() - 1);

	if(GameObj.getNoOfPlayers() == 0)
    {
        GameObj.InitConsoleScreen("off"); 
        cout << "\nNo Snakes Left to Play :(  Exiting Game ..." << endl << endl; 
        sleep(2);
        exit(3); 
    }

    GameObj.PrintAnimated("\n\nGame will continue in few seconds." ); 
    sleep(3);
}