#ifndef GAME_HPP
#define GAME_HPP

# include "SnakeClientShared.hpp"

class Snake;

class Game 
{
private:
	int					mNoOfPlayers;
	food				mFoodObj;
	int					mCenterX, mCenterY;
	unsigned long int	mSpeed;
	Snake*				mMainSnakePtr;

public:
	vector<Snake> allSnakes;

	Game(int players = 1)
	: mNoOfPlayers(players), mSpeed(70000)
	{
		mFoodObj.x = 0;
		mFoodObj.y = 0;
	}

	SocketHandler		mSockObj;
	
	int					getFoodX(void) { return (mFoodObj.x); }
	int					getFoodY(void) { return (mFoodObj.y); }
	int					getCenterX(void) { return (mCenterX); }
	int					getCenterY(void) { return (mCenterY); }
	food				getFoodPos(void);
	int					getNoOfPlayers() { return (mNoOfPlayers); }
	long int			getSpeed(void) { return (mSpeed); }

	void				setMainSnakePtr(Snake* ptr) { mMainSnakePtr = ptr; }
	void				setFoodPos(int x, int y);
	void				setNoOfPlayers(int n) { mNoOfPlayers = n; }
	unsigned long int	setSpeed(unsigned long int s) {
		mSpeed = s > 200000 ? 200000 : s < 10000 ? 10000 : s;
		return (mSpeed);
	}

	void				GenerateFood();
	void				PrintFood(string status = "old");
	void				PrintAnimated(string msg, int speed = 60000);
	void				InitColors();
	void				HandleMessageFromServer(string msg);
	void				ResetMaxScreen();
	void				InitConsoleScreen(string);
};

#endif