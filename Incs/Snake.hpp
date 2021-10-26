#ifndef SNAKE_HPP
#define SNAKE_HPP

# include "SnakeGameShared.hpp"

class Snake_part
{
public:
	int x, y;

	Snake_part(int _x, int _y)
	{
		this->x = _x;
		this->y = _y;
	}
};

class Snake
{
	friend				Game;
private:
	vector<snake_part>	mParts;
	string				mSnakeDirection;
	int					mBodyColor;
	int					mSockFD;
	int					mScore;
	string				mPlayerSight;
	char				mKeyUp, mKeyDown, mKeyRight, mKeyLeft;
	string				mPlayerName;
	int					mID;

public:
	Snake(char keyUp, char keyDown, char keyRight, char keyLeft,
		  int id, int sockFD = -1,
		  string playerName = "Name", string playerSight = "s");
	
	friend void signalHandler(int);

	int		getScore(void) { return (mScore); }
	int		getID(void) { return (mID); }
	string	getPlayerSight(void) { return (mPlayerSight);}
	int		getSocketDescriptor(void) { return (mSockFD); }
	int		getHeadX(void) { return (mParts.at(parts.size() - 1).x); }
	int		getHeadY(void) { return (mParts.at(parts.size() - 1).y); }
	string	getDirection(void) { return (snakeDirection); }
	
	void	setScore(int score) { mScore = score; }
	void	setPlayerName(sting playerName) { mPlayerName = playerName; }
	void	setBodyColor(int bodyColor) { mBodyColor = bodyColor; }

	void	DrawSnake();
	void	AddPart(int x, int y, string dir = "right");
	void	InitSnakeOnScreen();
	void	MoveSnake(string dir);
	void	CheckSnakeOverlap();
	void	HandleMovementKeyPress(char);
};

#endif

