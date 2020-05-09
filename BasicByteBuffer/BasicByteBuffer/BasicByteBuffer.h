#pragma once

#include <vector>

typedef unsigned char byte;

// CPU cache size for most of the architectures
static const int initialSize = 64;

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

class BasicByteReader
{
public:
	BasicByteReader(const byte* data, const size_t size)
		: buffer(const_cast<byte*>(data))
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
		// cast byte* to T*, dereference it, move the return value to the parameter value
		value = std::move(*(reinterpret_cast<T*>(buffer)));
		bytesRemaining -= dataSize;
		buffer += dataSize;
		return true;
	}

	template<typename T>
	inline bool read(T*& value)
	{
		const auto dataSize = sizeof(T);
		if (dataSize > bytesRemaining)
		{
			return false;
		}
		// cast byte* to T*, dereference it, move the return value to the parameter value
		value = std::move(reinterpret_cast<T*>(buffer));
		bytesRemaining -= dataSize;
		buffer += dataSize;
		return true;
	}

private:
	byte* buffer;
	size_t bytesRemaining;
};