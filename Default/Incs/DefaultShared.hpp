#ifndef DEFAULTSHARED_HPP
#define DEFAULTSHARED_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
// typedef void* receiveBuffer_t;
typedef int SOCKET;
const int NO_ERROR = 0;
const int INVAILD_SOCKET = -1;
const int WSAECONNRESET = ECONNRESET;
const int WSAEWOULDBLOCK = EAGAIN;
const int SOCKET_ERROR = -1;

#include <memory>

#include <vector>
#include <unordered_map>
#include <string>
#include <list>
#include <queue>
#include <deque>
#include <unordered_set>
#include <cassert>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;
using std::queue;
using std::list;
using std::deque;
using std::unordered_map;
using std::string;
using std::unordered_set;

class GameObject;

enum Dir
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

#include "DefaultMath.hpp"

#include "StringUtils.hpp"
#include "SocketAddress.hpp"
#include "SocketAddressFactory.hpp"
#include "UDPSocket.hpp"
#include "TCPSocket.hpp"
#include "SocketUtil.hpp"

#include "MemoryBitStream.hpp"

#include "InputState.hpp"

#include "GameObject.hpp"


#endif