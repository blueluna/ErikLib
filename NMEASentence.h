/*
* ErikLib NMEA Sentence
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef	ERIKLIB_NMEASENTENCE_H
#define ERIKLIB_NMEASENTENCE_H

#include <inttypes.h>
#include <Print.h>
#include <Printable.h>
#include "Buffer.h"

class NMEASentence
	: public Print
	, public Printable
{
protected:
	Buffer  buffer;

public:
	NMEASentence(size_t capacity);
	~NMEASentence();

    void 	clear();

    size_t 	write(uint8_t);
    size_t 	write(const uint8_t *buffer, size_t size);
    size_t 	comma();

    size_t 	printTo(Print& p) const;
};

#endif
