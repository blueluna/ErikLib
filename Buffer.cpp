/*
* ErikLib
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#include "Buffer.h"
#include <stdlib.h> // malloc, free
#include <string.h> // memmove, memcpy
#include <math.h> // ceil

Buffer::Buffer(const int16_t size, const int16_t alloc_chunk_size)
	: alloc_size(alloc_chunk_size)
	, buffer_size(0)
	, buffer_used(0)
	, buffer_data(0)
{
	Create(size);
}

Buffer::~Buffer()
{
	Destroy();
}

void Buffer::Destroy()
{
	buffer_size = 0;
	buffer_used = 0;
	free(buffer_data);
	buffer_data = 0;
}

int16_t Buffer::Create(const int16_t size)
{
	if (buffer_data == 0) {
		int16_t allocateBytes = 0;
		int16_t chunk_size = 0;
		if (size == 0) {
			allocateBytes = alloc_size;
			chunk_size = alloc_size;
		}
		else {
			allocateBytes = size;
		}
		buffer_data = reinterpret_cast<uint8_t*>(malloc(allocateBytes));
		if (buffer_data == 0) {
			buffer_size = 0;
			buffer_used = 0;
		}
		else {
			buffer_size = allocateBytes;
			buffer_used = 0;
			alloc_size = chunk_size;
		}
	}
	return buffer_data == 0 ? E_OUT_OF_MEMORY : E_SUCCESS;
}

int16_t Buffer::Read(uint8_t *data, const int16_t len)
{
	if (buffer_used == 0) {
		return 0;
	}
	// Use the smallest of len and buffer_used.
	int16_t read_len = len > buffer_used ? buffer_used : len;
	uint8_t * src_ptr = buffer_data;
	uint8_t * dst_ptr = data;
	if (dst_ptr != 0) {
		memcpy(dst_ptr, src_ptr, read_len);
	}
	int16_t left = buffer_used - read_len;
	if (left > 0) {
		memmove(buffer_data, buffer_data+read_len, left);
	}
	buffer_used -= read_len;
	return read_len;
}

int16_t Buffer::Write(const uint8_t *data, const int16_t len)
{
	if (len > Free()) {
		if (alloc_size == 0) { // fixed size buffer
			return -E_BUFFER_FULL;
		}
		else { // dynamic size buffer
			int16_t result = Resize(Used() + len);
			if (result != E_SUCCESS) {
				return -result;
			}
		}
	}
	uint8_t * dst_ptr = buffer_data + buffer_used;
	memcpy(dst_ptr, data, len);
	buffer_used += len;
	return len;
}

int16_t Buffer::Resize(const int16_t minsize)
{
	int16_t new_buffer_size = ceil(minsize / float(alloc_size)) * alloc_size;
	if (new_buffer_size < buffer_used) {
		return E_INVALID_VALUE;
	}
	uint8_t * new_buffer_data = reinterpret_cast<uint8_t*>(malloc(new_buffer_size));
	if (new_buffer_data == 0) {
		return E_OUT_OF_MEMORY;
	}
	if (buffer_used > 0) {
		memcpy(new_buffer_data, buffer_data, buffer_used);
	}
	free(buffer_data);
	buffer_data = new_buffer_data;
	buffer_size = new_buffer_size;
	return E_SUCCESS;
}

int16_t Buffer::Transfer(Buffer &other, const int16_t len)
{
	if (Free() < len && other.Used() < len) {
		return E_NOT_ENOUGH_DATA;
	}
	Write(other.Peek(), len);
	other.Read(0, len);
	return E_SUCCESS;
}

int16_t Buffer::Transfer(Stream &stream)
{
	uint8_t * dst_ptr = buffer_data + buffer_used;
	int16_t result = stream.readBytes(reinterpret_cast<char*>(dst_ptr), Free());
	buffer_used += result;
	return result;
}