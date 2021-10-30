#ifndef SERVER_HPP
#define SERVER_HPP

# include "SnakeGameShared.hpp"

# include <arpa/inet.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/time.h>

class SocketHandler
{
private:
	int					mOpt;
	int					mServSock;
	int					mAddrlen;
	int					mPort;
	int					mNewSock;
	int					mClientSock[30];
	int					mMaxClients;
	int					mActivity;
	int					mValread;
	int					mSockFD;
	int					mMaxSockFD;
	struct sockaddr_in	mAddress;
	char 				mMessage[50];
	struct timeval		mTimeout;

	char				mBuffer[1025];

	fd_set				mReadfds;

	void 				InitServerSocket();

public:
	SocketHandler(void);
	void				BindServer();
	vector<int>			HandleActivity();
	void				SetupClientDescriptors();
	void				StartServer();
	void				StopServer();
	void				SendData(int, std::string);
	int					CheckClientActivity();
	void				CloseSocket(int sd);
	int					HandleNewConnection();
	string				HandleIOActivity(int);
};


#endif