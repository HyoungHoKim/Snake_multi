#include "SnakeClientShared.hpp"

int g_max_x = 0, g_max_y = 0;
ofstream logfile;

Game GameObj(1);

int main()
{
	signal(SIGINT, SignalHandler);
	logfile.open("logfileClient.log", ios::out);
	srand(time(NULL));
	system("clear");
	string serverAddress, playerName, playerSight;
	cout << "Enter your name : ";

	getline(cin, playerName);

	cout << "Enter the IP address of the Controller Server : ";
	cin >> serverAddress;

	cout << "\nDo you want your snake to be displayed on your screen ? \n"; 
    cout << "1.Yes (You will see your snake on your screen and the same snake appears on the game controller.Default choice) : " << endl << endl; 
    cout << "2.No  (You will be able to control your snake by watching the game controller screen and your keyboard acts as a controller." << endl; 
    cout << endl << "Enter choice (1 or 2) : "; 
    cin >> playerSight;

	if (playerSight == "2")
		playerSight = "s";
	else
		playerSight = "c";

	GameObj.mSockObj.ConnectToServer(serverAddress, 8888);

	Snake first_snake('A', 'B', 'C', 'D', 0, playerName);
	first_snake.setPlayerSight(playerSight);
	GameObj.setMainSnakePtr(&first_snake);

	GameObj.mSockObj.SendData("init~~" + playerName + "~~" + playerSight + "~~" + "&" + to_string(first_snake.getBodyColor()) + "&");

	GameObj.InitConsoleScreen("on");
	GameObj.InitColors();

	first_snake.InitSnakeOnScreen();

	char ch;

	while (true)
	{
		GameObj.ResetMaxScreen() ; 

        if((ch = getch()) != ERR)
        {
            if(ch==27)
            {
                getch(); // clear and reject 91 from buffer
                ch = getch(); //Now store the actual value of arrow key pressed               getch() ; 
                string ch_string(1, ch); 
                GameObj.mSockObj.SendData(ch_string) ; 
                if(first_snake.getPlayerSight() == "c")
                    first_snake.HandleMovementKeyPress(ch); 
            }
        }

        flushinp();
        clear();

        if(first_snake.getPlayerSight() == "c")
        {
            first_snake.MoveSnake(first_snake.getDirection());

            PrintSpeed(first_snake); 
            GameObj.PrintFood();
            first_snake.PrintScores() ; 
        }

        refresh() ;
        //Read for any incoming data from server ,,,
        GameObj.mSockObj.ReadData(); 

        usleep(GameObj.getSpeed()) ;
	}
}