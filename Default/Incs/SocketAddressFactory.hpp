#ifndef SOCKETADDRESSFACTORY_HPP
#define SOCKETADDRESSFACTORY_HPP

#include "DefaultShared.hpp"

class SocketAddressFactory
{
public:
    static SocketAddressPtr CreateIPv4FromString(const string& inString);
};

#endif