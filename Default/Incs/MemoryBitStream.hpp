#ifndef MEMORYBITSTREAM_HPP
#define MEMORYBITSTREAM_HPP

#include <cstdint>
#include <cstdlib>
#include <string>

class GameObject;

inline uint32_t ConvertToFixed(float inNumber, float inMin, float inPrecision)
{
	return (static_cast<int>((inNumber - inMin) / inPrecision));
}

inline float ConvertFromFixed(uint32_t inNumber, float inMin, float inPrecision)
{
	return (static_cast<int>(inNumber) * inPrecision + inMin);
}

class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream()
	: mBitHead(0), mBuffer(nullptr)
	{
		ReallocBuffer(1500 * 8);
	}

	~OutputMemoryBitStream() {
		std::free(mBuffer);
	}

	void WriteBits(uint8_t inData, uint32_t inBitCount);
	void WriteBits(const void* inData, uint32_t inBitCount);

	const char* GetBufferPtr() const {
		return (mBuffer);
	}
	uint32_t GetBitLength() const {
		return (mBitHead);
	}
	uint32_t GetByteLength() const {
		return (mBitHead + 7) >> 3;
	}

	void WriteBytes(const void* inData, uint32_t inByteCount) {
		WriteBits(inData, inByteCount << 3);
	}

	template<typename T>
	void Write(T inData, uint32_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
					"Generic Write only supports primitives data types");
		WriteBits(&inData, inBitCount);
	}

	void Write(bool inData) {
		WriteBits(&inData, 1);
	}

	void Write(const Vector3& inVector);
	void Write(const std::string& inString)
	{
		uint32_t elementCount = static_cast<uint32_t>(inString.size());
		Write(elementCount);
		for (const auto& element : inString)
			Write(element);
	}

private:
	void ReallocBuffer(uint32_t inNewBitCapacity);

	char *mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
};

class InputMemoryBitStream
{
public:
	InputMemoryBitStream(char* inBuffer, uint32_t inBitCount)
	: mBuffer(inBuffer), mBitCapacity(inBitCount), mBitHead(0), mIsBufferOwner(false)
	{}

	InputMemoryBitStream(const InputMemoryBitStream& inOther)
	: mBitCapacity(inOther.mBitCapacity), mBitHead(inOther.mBitHead), mIsBufferOwner(true)
	{
		int byteCount = mBitCapacity / 8;
		mBuffer = static_cast<char*>(malloc(byteCount));
		memcpy(mBuffer, inOther.mBuffer, byteCount);
	}

	~InputMemoryBitStream() {
		if(mIsBufferOwner) {
			free(mBuffer);
		}
	}

	const char* GetBufferPtr() const {
		return (mBuffer);
	}

	uint32_t GetRemainingBitCount() const {
		return (mBitCapacity - mBitHead);
	}

	void ReadBits(uint8_t& outData, uint32_t inBitCount);
	void ReadBits(void* outData, uint32_t inBitCount);
	
	void ReadBytes(void* outData, uint32_t inByteCount) {
        ReadBits(outData, inByteCount << 3);
    }
}

#endif