#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "SnakeClientShared.hpp"

class Snake_part
{
public:
	int x, y;

	Snake_part(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

class Snake
{
	friend				Game;
private:
	vector<Snake_part>	mParts;
	string				mSnakeDirection;
	int					mBodyColor;
	int					mScore;
	string				mPlayerSight;
	char				mKeyUp, mKeyDown, mKeyRight, mKeyLeft;
	string				mPlayerName;
	int					mID;

public:
	Snake(char keyUp, char keyDown, char keyRight, char keyLeft,
		  int id, string playerName = "Name");

	int		getScore(void) { return (mScore); }
	int		getID(void) { return (mID); }
	string	getPlayerSight(void) { return (mPlayerSight);}
	int		getHeadX(void) { return (mParts.at(mParts.size() - 1).x); }
	int		getHeadY(void) { return (mParts.at(mParts.size() - 1).y); }
	string	getDirection(void) { return (mSnakeDirection); }
	int		getBodyColor(void) { return (mBodyColor); }
	
	void	setScore(int score) { mScore = score; }
	void	setPlayerName(string playerName) { mPlayerName = playerName; }
	void	setBodyColor(int bodyColor) { mBodyColor = bodyColor; }
	void	setPlayerSight(string sight) { mPlayerSight = sight; }

	void	DrawSnake();
	void	AddPart(int x, int y, string dir = "right");
	void	InitSnakeOnScreen();
	void	MoveSnake(string dir);
	void	CheckSnakeOverlap();
	void	HandleMovementKeyPress(char ch);
	void	GameOverHandler();
	void	PrintScores(string pos = "left");
};

#endif