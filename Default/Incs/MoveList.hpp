#ifndef MOVELIST_HPP
#define MOVELIST_HPP

class MoveList
{
public:
	typedef deque<Move>::const_iterator const_iterator;
	typedef deque<Move>::const_reverse_iterator const_reverse_iterator;

	MoveList()
	: mLastMoveTimestamp(-1.f)
	{}

	const Move& AddMove(const Dir& inDir, float inTimestamp);
	bool AddMove(const Move& inMove);
	void RemoveProcessMoves(float inLastMoveProcessedOnServerTimestamp);

	float GetLastMoveTimestamp() const {
		return (mLastMoveTimestamp);
	}

	const Move& GetLatestMove() const {
		return (mMoves.back());
	}

	void Clear() {
		mMoves.clear();
	}

	bool HasMoves() const {
		return (!mMoves.empty());
	}

	size_t GetMoveCount() const {
		return (mMoves.size());
	}

	const_iterator begin() const {
		return (mMoves.begin());
	}
	const_iterator end() const {
		return (mMoves.end());
	}

	const Move& operator[](size_t i) const {
		return (mMoves[i]);
	}

private:
	float mLastMoveTimestamp;
	deque<Move> mMoves;
};

#endif