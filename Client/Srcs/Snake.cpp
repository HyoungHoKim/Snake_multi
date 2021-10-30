#include "SnakeClientShared.hpp"

extern int g_max_x;
extern int g_max_y;
extern ofstream logfile;

extern Game GameObj;

Snake::Snake(char keyUp, char keyDown, char keyRight, char keyLeft,
		  int id, string playerName) :
	mKeyUp(keyUp), mKeyDown(keyDown), mKeyRight(keyRight), mKeyLeft(keyLeft),
	mID(id), mPlayerName(playerName)
{
	mScore = 0;
	mSnakeDirection = "right";
	mID = id;
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
		AddPart(GameObj.getFoodX(), GameObj.getFoodY());
		setScore(getScore() + 1);
		GameObj.PrintFood("new");

		GameObj.setFoodPos(-10, -10);
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
			GameOverHandler();
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
	GameObj.mSockObj.SendData("$");
	clear();
	GameObj.InitConsoleScreen("off");
	system("clear");
	string gameovermessage = "\n\n\nGAME OVER " + mPlayerName + " :(\n\n"; 
    gameovermessage += "Score : " + std::to_string(mScore) + "\nBetter Luck Next time :)\n\n"; 
    gameovermessage += "\n\nPress ctrl+c to exit."; 
    GameObj.PrintAnimated(gameovermessage); 
    GameObj.mSockObj.CloseSocket(); 
    sleep(50000);
}

void Snake::PrintScores(string pos)
{
	if (pos == "right")
		mvprintw(0, 15, "Score = %d", mScore);
	mvprintw(0, 0, "Score = %d", mScore);
}