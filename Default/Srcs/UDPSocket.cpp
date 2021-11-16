#include "DefaultShared.hpp"

int UDPSocket::Bind(const SocketAddress &inBindAddress)
{
	int error = bind(mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize());
	if (error != 0)
	{
		SocketUtil::ReportError("UDPSocket::Bind");
		return (SocketUtil::GetLastError());
	}
	return (NO_ERROR);
}

int UDPSocket::SendTo(const void* inToSend, int inLength, const SocketAddress &inAddress)
{
	int byteSentCount = sendto(mSocket,
								static_cast<const char*>(inToSend),
								inLength,
								0,
								&inToAddress.mSockAddr,
								inToAddress.GetSize());
	if (byteSentCount <= 0)
	{
		SocketUtil::ReportError("UDPSocket::Sendto");
		return (-SocketUtil::GetLastError());
	}
	else
		return (byteSentCount);
}

int UDPSocket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)
{
	socklen_t fromLength = outFromAddress.GetSize();

	int readByteCount = recvfrom(mSocket,
								static_cast<char*>(inToReceive),
								inMaxLength,
								0,
								&outFromAddress.mSockAddr,
								&fromLength);
	if (readByteCount >= 0)
		return (readByteCount);
	else
	{
		int error = SocketUtil::GetLastError();

		if (error == WSAEWOULDBLOCK)
			return (0);
		else if (error == WSAECONNRESET)
		{
			LOG("Connection reset from %s", outFromAddress.ToString().c_str());
			return (-WSAECONNRESET);
		}
		else
		{
			SocketUtil::ReportError("UDPSocket::ReceiveFrom");
			return (-error);
		}
	}
}

UDPSocket::~UDPSocket()
{
	close(mSocket);
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
	int flags = fcntl(mSocket, F_GETFL, 0);
	flags = inShouldBeNonBlocking ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
	int result = fcntl(mSocket, F_SETFL, flags);

	if (result == SOCKET_ERROR)
	{
		SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
		return (SocketUtil::GetLastError());
	}
	else
		return (NO_ERROR);
}