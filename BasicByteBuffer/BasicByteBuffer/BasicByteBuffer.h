#pragma once

#include <vector>

typedef unsigned char byte;

// CPU cache size for most of the architectures
static constexpr int initialSize = 64;

// WARNING: Use these classes only when 'sizeof' macro returns the correct size

class BasicByteWriter
{
public:
	BasicByteWriter()
	{
		buffer.reserve(initialSize);
	}

	template<typename T>
	inline void write(const T& data)
	{
		const auto dataStartAddress = reinterpret_cast<const byte*>(&data);
		const auto dataSize = sizeof(T);
		// Double the capacity if it is not enough
		while (buffer.capacity() < buffer.size() + dataSize)
		{
			buffer.reserve(buffer.capacity() * 2);
		}
		
		// copy the data to the buffer
		std::copy(dataStartAddress, dataStartAddress + dataSize, std::back_inserter(buffer));
	}

	inline std::vector<byte> getDataAndResetBuffer()
	{
		std::vector<byte> result = std::move(buffer);
		*this = BasicByteWriter();
		return result;
	}

private:
	std::vector<byte> buffer;
};

// WARNING: Since you have to initialize your data first, and then read the value into it, 
//			it is not recommended to use this class when reading data to create large classes, 
//			altough there is an example in main.cpp

class BasicByteReader
{
public:
	BasicByteReader(byte* data, const size_t size)
		: buffer(data)
		, bytesRemaining(size)
	{

	}

	template<typename T>
	inline bool read(T& value)
	{
		const auto dataSize = sizeof(T);
		if (dataSize > bytesRemaining)
		{
			return false;
		}
		// cast byte* to T*, dereference it, copy/move it to the value
		value = *(reinterpret_cast<T*>(buffer));
		bytesRemaining -= dataSize;
		buffer += dataSize;
		return true;
	}
private:
	byte* buffer;
	size_t bytesRemaining;
};