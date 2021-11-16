#include "DefaultShared.hpp"

int TCPSocket::Connect(const SocketAddress& inAddress)
{
	int error = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());

	if (error < 0)
	{
		SocketUtil::ReportError("TCPSocket::Connect");
		return (-SocketUtil::GetLastError());
	}
	return (NO_ERROR);
}

int TCPSocket::Listen(int inBackLog)
{
	int error = listen(mSocket, inBackLog);
	
	if (error < 0)
	{
		SocketUtil::ReportError("TCPSocket::Listen");
		return (-SocketUtil::GetLastError());
	}
	return (NO_ERROR);
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
    socklen_t length = inFromAddress.GetSize();
    SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);
    
    if (newSocket != INVAILD_SOCKET)
        return (TCPSocketPtr(new TCPSocket(newSocket)));
    else
    {
        SocketUtil::ReportError("TCPSocket::Accept");
        return (nullptr);
    }
}

int32_t TCPSocket::Send(const void *inData, size_t inLen)
{
    int byteSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);
    if (byteSentCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Send");
        return (-SocketUtil::GetLastError());
    }
    return (byteSentCount);
}

int TCPSocket::Receive(void *inData, size_t inLen)
{
    int byteReceivedCount = recv(mSocket, static_cast<char*>(inData), inLen, 0);
    if (byteReceivedCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Receive");
        return (-SocketUtil::GetLastError());
    }
    return (byteReceivedCount);
}

int TCPSocket::Bind(const SocketAddress& inBindAddress)
{
    int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
    if (error != 0)
    {
        SocketUtil::ReportError("TCPSocket::Bind");
        return (SocketUtil::GetLastError());
    }
    return (NO_ERROR);
}

TCPSocket::~TCPSocket()
{
    close(mSocket);
}
