/*
* ErikLib
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef	ERIKLIB_H
#define ERIKLIB_H

#include <inttypes.h>
#include <stdio.h> // for size_t

#include "Buffer.h"
#include "NMEASentence.h"
#include "NMEAParser.h"
#include "Button.h"

class ErikLib
{
public:
	static size_t HexPrint(Print& p, const uint8_t value) { return HexPrint(p, value, 1); }
	static size_t HexPrint(Print& p, const uint16_t value) { return HexPrint(p, value, 2); }
	static size_t HexPrint(Print& p, const uint32_t value) { return HexPrint(p, value, 4); }
	static size_t HexPrint(Print& p, const uint64_t value) { return HexPrint(p, value, 8); }

protected:
	static size_t HexPrint(Print& p, const uint64_t value, const uint8_t bytes);
};

#endif
