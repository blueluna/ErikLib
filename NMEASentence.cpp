/*
* ErikLib NMEA Sentence
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#include "NMEASentence.h"
#include "ErikLib.h"

NMEASentence::NMEASentence(size_t capacity)
	: bufferCapacity(0)
	, buffer(0)
	, bufferCount(0)
{
	buffer = reinterpret_cast<uint8_t*>(malloc(capacity));
	if (buffer != 0) {
		bufferCapacity = capacity;
		clear();
	}
}

NMEASentence::~NMEASentence()
{
	free(buffer);
	bufferCapacity = 0;
	bufferCount = 0;
}

void NMEASentence::clear()
{
	bufferCount = 0;
	memset(buffer, 0, bufferCapacity);
}

size_t NMEASentence::write(uint8_t b)
{
	if (bufferCount < bufferCapacity) {
		buffer[bufferCount] = b;
		bufferCount++;
		return 1;
	}
	return 0;
}

size_t NMEASentence::write(const uint8_t *b, size_t s)
{
	if ((bufferCount + s) < bufferCapacity) {
		memcpy(buffer + bufferCount, b, s);
		bufferCount += s;
		return s;
	}
	return 0;
}

size_t NMEASentence::printTo(Print& p) const
{
	size_t count = p.write('$');
	uint8_t checksum = 0;
	uint8_t *ptr = buffer;
	uint8_t *end = buffer + bufferCount;
	while (ptr < end) {
		checksum ^= *ptr;
		ptr++;
	}
	count += p.write(buffer, bufferCount);
	count += p.write('*');
	count += ErikLib::HexPrint(p, checksum);
	count += p.write('\r');
	count += p.write('\n');
	return count;
}
