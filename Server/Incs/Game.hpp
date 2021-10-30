#ifndef GAME_HPP
#define GAME_HPP

# include "SnakeGameShared.hpp"

class Snake;

class Game 
{
	friend				Snake;
private:
	int					mNoOfPlayers;
	string				mGameMode;
	SocketHandler		mServer;
	vector<int>			mClients;
	food				mFoodObj;
	int					mCenterX, mCenterY;
	unsigned long int	mSpeed;

public:
	vector<Snake> allSnakes;

	Game(int players = 1)
	: mNoOfPlayers(players), mSpeed(70000)
	{
		mFoodObj.x = 0;
		mFoodObj.y = 0;
	}

	friend void			signalHandler(int);

	int					getSnakeIndexFromDescriptor(int sd);
	int					getSnakeIndexFromID(int id);
	int					getFoodX(void) { return (mFoodObj.x); }
	int					getFoodY(void) { return (mFoodObj.y); }
	int					getCenterX(void) { return (mCenterX); }
	int					getCenterY(void) { return (mCenterY); }
	string				getGameMode() { return (mGameMode); }
	food				getFoodPos(void);
	int					getNoOfPlayers(void) { return (mNoOfPlayers); }
	unsigned long int	getSpeed(void) { return (mSpeed); }
	SocketHandler*		getServer(void) { return (&mServer); }

	void 				setFoodPos(int x, int y);
	void 				setGameMode(string Gamemode) { mGameMode = Gamemode; }
	void 				setClientsList();
	void 				setClientsList(vector<int> clientsList) { mClients = clientsList; }
	void 				setNoOfPlayers(int n) { mNoOfPlayers = n; }
	void 				setSpeed(unsigned long int s) { mSpeed = s > 200000 ? 200000 : s < 10000 ? 10000 : s; }

	void				GenerateFood();
	void				ShowInitialChoices();
	void				SyncSnakeWithClient(Snake& clientSnake);
	void				ResetMaxScreen();
	void				PrintAnimated(string msg, int speed = 40000);
	void				DrawAllSnakes();
	void				KeyPressHandler();
	void				PrintFood(string status = "old");
	void				MainEventLoop();
	void				AskNoPlayers(string player = "single");
	void				MoveAllSnakes();
	void				LANSendFoodCoordinates(int x, int y);
	void				PrintScores();
	void				HandleNewConnection();
	void				HandleIOActivity();
	int					CheckClientActivity();
	void				InitServerForMultiplayer();
	void				HandleActivity();
	void				InitConsoleScreen(string state);
};

#endif