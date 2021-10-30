#include "SnakeGameShared.hpp"

int g_max_x = 0, g_max_y = 0;
ofstream logfile;

std::ostringstream ss;

Game GameObj(1);

int main(int argc, char* argv[])
{
	logfile.open("logfile.log", ios::out);
	GameObj.ShowInitialChoices();

	signal(SIGINT, SignalHandler);
	HANDLE_EVERYTHING_TILL_EVENT_LOOP();
	int activity;

	if (GameObj.getGameMode() == "multi")
		GameObj.InitServerForMultiplayer();

	while (true)
	{
		GameObj.ResetMaxScreen();
		GameObj.KeyPressHandler();

		if (GameObj.getGameMode() == "multi")
		{
			activity = GameObj.CheckClientActivity();
			if (activity >= 1)
				GameObj.HandleActivity();
		}

		flushinp();
		clear();

		GameObj.MoveAllSnakes();
		GameObj.PrintScores();

		if (GameObj.getNoOfPlayers())
			PrintSpeed(GameObj.allSnakes[0]);

		GameObj.PrintFood();
		refresh();
		usleep(GameObj.getSpeed());
	}
}