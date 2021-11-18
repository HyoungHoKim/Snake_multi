#include "DefaultShared.hpp"

bool Move::Write(OutputMemoryBitStream& inOutputStream) const 
{
	inOutputStream.Write(mDir);
	inOutputStream.Write(mTimestamp);

	return (true);
}

bool Move::Read(InputMemoryBitStream& inInputStream)
{
	inInputStream.Read(mDir);
	inInputStream.Read(mTimestamp);

	return (true);
}