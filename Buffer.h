/*
* ErikLib Buffer
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef ERIKLIB_BUFFER_H
#define ERIKLIB_BUFFER_H

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "ErrorCodes.h"

class Buffer
{
protected:
	size_t      alloc_size;
	size_t     	buffer_size;
	size_t    	buffer_used;
	uint8_t    	*buffer_data;

public:
	/*
	 * Create a buffer.
	 * if size == 0: dynamic sized buffer, if size > 0: fixed sized buffer.
	 * when the buffer is dynamic sized the size of the buffer will be a
	 * multiple of alloc_chunk_size.
	 */
	Buffer(const size_t size, const size_t alloc_chunk_size = 64);
	~Buffer();

	void    Destroy();

	/*
	 * Create a buffer.
	 * if size == 0: dynamic sized buffer, if size > 0: fixed sized buffer.
	 * when the buffer is dynamic sized the size of the buffer will be a
	 * multiple of alloc_chunk_size.
	 */
	int32_t Create(const size_t size = 0);

	inline size_t Size() const
	{
		return buffer_size;
	}

	inline size_t Free() const
	{
		return buffer_size - buffer_used;
	}

	inline size_t Used() const
	{
		return buffer_used;
	}

	int32_t Read(uint8_t *data, const size_t len);
	int32_t Write(const uint8_t *data, const size_t len);

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

protected:
	int32_t Resize(const size_t minsize);
};

#endif // ERIKLIB_BUFFER_H
