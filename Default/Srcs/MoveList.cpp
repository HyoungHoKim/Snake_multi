#include "DefaultShared.hpp"

const Move& MoveList::AddMove(const Dir& inDir, float inTimestamp)
{
	float deltaTime = mLastMoveTimestamp >= 0.f ? inTimestamp - mLastMoveTimestamp : 0.f;

	mMoves.emplace_back(inDir, inTimestamp, deltaTime);

	mLastMoveTimestamp = inTimestamp;

	return (mMoves.back());
}

bool MoveList::AddMove(const Move& inMove)
{
	float timeStamp = inMove.GetTimestamp();
	float deltaTime = mLastMoveTimestamp >= 0.f ? timeStamp - mLastMoveTimestamp : 0.f;
	mLastMoveTimestamp = timeStamp;
	mMoves.emplace_back(inMove.GetInputState(), timeStamp, deltaTime);
	return (true);
}

void MoveList::RemovedProcessMoves(float inLastMoveProcessesOnServerTimestamp)
{
	while (!mMoves.empty() && mMoves.front().GetTimestamp() <= inLastMoveProcessesOnServerTimestamp)
	{
		mMoves.pop_front();
	}
}