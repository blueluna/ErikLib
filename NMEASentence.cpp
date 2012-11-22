/*
* ErikLib NMEA Sentence
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#include "NMEASentence.h"
#include "ErikLib.h"

NMEASentence::NMEASentence(size_t capacity)
	: buffer(capacity)
{
}

NMEASentence::~NMEASentence()
{
}

void NMEASentence::clear()
{
	buffer.Clear();
}

size_t NMEASentence::write(uint8_t b)
{
	int32_t result = buffer.Write(&b, 1);
	if (result >= 0) {
		return (size_t)(result);
	}
	return 0;
}

size_t NMEASentence::write(const uint8_t *b, size_t s)
{
	int32_t result = buffer.Write(b, s);
	if (result >= 0) {
		return (size_t)(result);
	}
	return 0;
}

size_t NMEASentence::comma()
{
	return write(0x2c);
}

size_t NMEASentence::printTo(Print& p) const
{
	size_t count = p.write('$');
	uint8_t checksum = 0;
	const uint8_t *ptr = buffer.Peek();
	const uint8_t *end = ptr + buffer.Used();
	while (ptr < end) {
		checksum ^= *ptr;
		ptr++;
	}
	count += p.write(buffer.Peek(), buffer.Used());
	count += p.write('*');
	count += ErikLib::HexPrint(p, checksum);
	count += p.write('\r');
	count += p.write('\n');
	return count;
}
