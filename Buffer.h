/*
* ErikLib Buffer
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef ERIKLIB_BUFFER_H
#define ERIKLIB_BUFFER_H

#include <inttypes.h>
#include <Arduino.h>

#include "ErrorCodes.h"

class Buffer
{
protected:
	int16_t     alloc_size;
	int16_t     buffer_size;
	int16_t    	buffer_used;
	uint8_t    	*buffer_data;

public:
	/*
	 * Create a buffer.
	 * if size == 0: dynamic sized buffer, if size > 0: fixed sized buffer.
	 * when the buffer is dynamic sized the size of the buffer will be a
	 * multiple of alloc_chunk_size.
	 */
	Buffer(const int16_t size, const int16_t alloc_chunk_size = 64);
	~Buffer();

	void    Destroy();

	/*
	 * Create a buffer.
	 * if size == 0: dynamic sized buffer, if size > 0: fixed sized buffer.
	 * when the buffer is dynamic sized the size of the buffer will be a
	 * multiple of alloc_chunk_size.
	 */
	int16_t Create(const int16_t size = 0);

	inline int16_t Size() const
	{
		return buffer_size;
	}

	inline int16_t Free() const
	{
		return buffer_size - buffer_used;
	}

	inline int16_t Used() const
	{
		return buffer_used;
	}

	int16_t Read(uint8_t *data, const int16_t len);
	int16_t Write(const uint8_t *data, const int16_t len);

	void Clear()
	{
		buffer_used = 0;
	}

	void DeepClear()
	{
		Destroy();
		Create();
	}

	inline const uint8_t * Peek() const
	{
		return buffer_data;
	}

	int16_t Transfer(Buffer &other, const int16_t len);

	int16_t Transfer(Stream &stream);

protected:
	int16_t Resize(const int16_t minsize);
};

#endif // ERIKLIB_BUFFER_H
