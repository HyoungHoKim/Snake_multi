#include "DefaultShared.hpp"

void OutputMemoryBitStream::WriteBits(uint8_t inData, uint32_t inBitCount)
{
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);

	if (nextBitHead > mBitCapacity)
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;

	if (bitsFreeThisByte < inBitCount)
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	
	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);
	while (inBitCount > 8)
	{
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0)
		WriteBits(*srcByte, inBitCount);
}

void OutputMemoryBitStream::Write(const Vector3& inVector)
{
	Write(inVector.mX);
	Write(inVector.mY);
	Write(inVector.mZ);
}

void InputMemoryBitStream::Read(Vector3& outVector)
{
	Read(outVector.mX);
	Read(outVector.mY);
	Read(outVector.mZ);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitLength)
{
	if (mBuffer == nullptr)
	{
		// mBuffer = static_cast<char*>(std::malloc(inNewBitLength >> 3));
		mBuffer = new char(inNewBitLength >> 3);
		memset(mBuffer, 0, inNewBitLength >> 3);
	}
	else
	{
		//char* tempBuffer = static_cast<char*>(std::malloc(inNewBitLength >> 3));
		char* tempBuffer = new char(inNewBitLength >> 3);
		memset(tempBuffer, 0, inNewBitLength >> 3);
		memcpy(tempBuffer, mBuffer, mBitCapacity >> 3);
		std::free(mBuffer);
		mBuffer = tempBuffer;
	}

	mBitCapacity = inNewBitLength;
}

void InputMemoryBitStream::ReadBits(uint8_t& outData, uint32_t inBitCount)
{
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;
	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount)
		outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	
	outData &= (~(0x00ff << inBitCount));

	mBitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, uint32_t inBitCount)
{
	uint8_t* desByte = reinterpret_cast<uint8_t*>(outData);
	while (inBitCount > 8)
	{
		ReadBits(*desByte, 8);
		++desByte;
		inBitCount -= 8;
	}
	if (inBitCount > 0)
		ReadBits(*desByte, inBitCount);
}
