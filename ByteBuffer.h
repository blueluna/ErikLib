/*
* ErikLib Byte Buffer
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef	ERIKLIB_BYTEBUFFER_H
#define ERIKLIB_BYTEBUFFER_H

class ByteBuffer
{
protected:
	size_t 	bufferCapacity;
	uint8_t *buffer;
	size_t 	bufferCount;

public:
	ByteBuffer(const size_t capacity);
	~ByteBuffer();
};

#endif