#ifndef MOVE_HPP
#define MOVE_HPP

#include "DefaultShared.hpp"

class Move
{
public:
	Move()
	{}

	Move(const Dir& inDir, float inTimestamp, float inDeltaTime)
	: mDir(inDir), mTimestamp(inTimestamp), mDeltaTime(inDeltaTime)
	{}

	const Dir& GetDir() const {
		return (mDir);
	}
	float GetTimestamp() const {
		return (mTimestamp);
	}
	float GetDeltaTime() const {
		return (mDeltaTime);
	}

	bool Write(OutputMemoryBitStream& inOutputStream) const;
	bool Read(InputMemoryBitStream& inInputStream);

private:
	Dir mDir;
	float mTimestamp;
	float mDeltaTime;
};

#endif