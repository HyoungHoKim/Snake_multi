#include "SnakeGameShared.hpp"

SocketHandler::SocketHandler(void)
{
	mMaxClients = 30;
	mValread = 0;
	mPort = 8888;
	mOpt = true;

	mTimeout.tv_sec = mTimeout.tv_usec = 0;

	memset(mClientSock, 0, sizeof(mClientSock));
	memset(&mAddress, 0, sizeof(mAddress));
	memset(mBuffer, 0, sizeof(mBuffer));
	strcpy(mMessage, "Successfully Connected \r\n");
}

void SocketHandler::InitServerSocket()
{
	if ((mServSock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(mServSock, SOL_SOCKET, SO_REUSEADDR, (char*)&mOpt, sizeof(mOpt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void SocketHandler::BindServer()
{
	InitServerSocket();

	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	mAddress.sin_port = htons(mPort);

	int error = ::bind(mServSock, (struct sockaddr *)&mAddress, sizeof(mAddress));
	if (error != 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void SocketHandler::SetupClientDescriptors()
{
	FD_ZERO(&mReadfds);

	FD_SET(mServSock, &mReadfds);
	mMaxSockFD = mServSock;

	for (int i = 0; i < mMaxClients; i++)
	{
		mSockFD = mClientSock[i];

		if (mSockFD > 0)
			FD_SET(mSockFD, &mReadfds);
		
		if (mSockFD > mMaxSockFD)
			mMaxSockFD = mSockFD;
	}
}

void SocketHandler::StartServer()
{
	if (listen(mServSock, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	mAddrlen = sizeof(mAddress);
	puts("Waiting for connections...");
}

int SocketHandler::CheckClientActivity()
{
	SetupClientDescriptors();

	mActivity = select(mMaxSockFD + 1, &mReadfds, NULL, NULL, &mTimeout);

	if ((mActivity < 0) && (errno != EINTR))
		printf("select error\n");
	
	return (mActivity);
}

void SocketHandler::CloseSocket(int sd)
{
	close(sd);
	for (int i = 0; i < mMaxClients; i++)
	{
		if (mClientSock[i] == sd)
			mClientSock[i] = 0;
	}
}

void SocketHandler::StopServer()
{
	close(mServSock);
}

int SocketHandler::HandleNewConnection()
{
	if ((mNewSock = accept(mServSock, (struct sockaddr*)&mAddress, (socklen_t*)&mAddrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < mMaxClients; i++)
	{
		if (mClientSock[i] == 0)
		{
			mClientSock[i] = mNewSock;
			printf("Adding to list of sockets as %d\n", i);
			break;
		}
	}

	return (mNewSock);
}

string SocketHandler::HandleIOActivity(int client_sd)
{
	if ((mValread = read(client_sd, mBuffer, 1024)) <= 0)
	{
		getpeername(client_sd, (struct sockaddr*)&mAddress, (socklen_t*)&mAddrlen);

		close(client_sd);
		for (int i = 0; i < mMaxClients; i++)
		{
			if (mClientSock[i] == client_sd)
				mClientSock[i] = 0;
		}

		return ("");
	}
	else
	{
		mBuffer[mValread] = '\0';
		string message = mBuffer;
		return (message);
	}
}

void SocketHandler::SendData(int client_sd, string msg)
{
	send(client_sd, msg.c_str(), msg.length(), 0);
}

vector<int> SocketHandler::HandleActivity()
{
	vector<int> descriptors;

	if (FD_ISSET(mServSock, &mReadfds))
		descriptors.push_back(-1);
	else
	{
		for (int i = 0; i < mMaxClients; i++)
		{
			mSockFD = mClientSock[i];

			if (FD_ISSET(mSockFD, &mReadfds))
				descriptors.push_back(mSockFD);
		}
	}
	
	return (descriptors);
}