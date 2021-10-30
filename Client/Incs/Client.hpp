#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/time.h>

# include "SnakeClientShared.hpp"

class SocketHandler
{
	struct sockaddr_in	mAddress;
	struct sockaddr_in	mServerAddr;
	int					mSockFD;
	char				mBuffer[1025];
	fd_set				mSet;
	struct				timeval mTimeout;

public:
	SocketHandler(void);
	void SendData(string message);
	void CreateClientSocket(void);
	void InitServerAddress(string address, int port);
	void ConnectToServer(string, int);
	void ReadData(void);
	void CloseSocket(void);
};

#endif