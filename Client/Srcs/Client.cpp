#include "SnakeClientShared.hpp"

extern ofstream logfile;

extern Game GameObj;

SocketHandler::SocketHandler(void)
: mSockFD(0)
{
	memset(&mServerAddr, 0, sizeof(mServerAddr));
	memset(mBuffer, 0, sizeof(mBuffer));
	mTimeout.tv_sec = mTimeout.tv_usec = 0;
}

void SocketHandler::SendData(string message)
{
	logfile << "\nSending data to server : \n" << message << "\n";
	send(mSockFD, message.c_str(), message.length(), 0);
}

void SocketHandler::CreateClientSocket(void)
{
	int opt = 1;
	mSockFD = socket(AF_INET, SOCK_STREAM, 0);
	FD_ZERO(&mSet);
	FD_SET(mSockFD, &mSet);

	if (setsockopt(mSockFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) && 
		setsockopt(mSockFD, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void SocketHandler::InitServerAddress(string address, int port)
{
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_port = htons(port);

	if (inet_pton(AF_INET, address.c_str(), &mServerAddr.sin_addr) <= 0)
		printf("\nInvalid address/Address not supported \n");
}

void SocketHandler::ConnectToServer(string address, int port)
{
	CreateClientSocket();
	InitServerAddress(address, port);

	if (connect(mSockFD, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) < 0)
	{
		printf("\nConnection Failed\n");
		exit(1);
	}
}

void SocketHandler::ReadData(void)
{
	int val;

	FD_ZERO(&mSet);
	FD_SET(mSockFD, &mSet);

	val = select(mSockFD + 1, &mSet, NULL, NULL, &mTimeout);

	if (val > 0)
	{
		recv(mSockFD, mBuffer, 1024, 0);

		GameObj.HandleMessageFromServer(string(mBuffer));
	}
	else if (val == -1)
		perror("select");
}

void SocketHandler::CloseSocket(void)
{
	close(mSockFD);
}

