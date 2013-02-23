/*
* ErikLib NMEA Parser
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#ifndef	ERIKLIB_NMEAPARSER_H
#define ERIKLIB_NMEAPARSER_H

#include "Buffer.h"

class NMEAParser
{
protected:
	Buffer  	parseBuffer;
	int16_t		parserPosition;
	int16_t		sentenceStart;
	int16_t		sentenceEnd;
	uint8_t		sentenceChecksum;
	uint8_t     delimeterIndex;
	int16_t     delimterPositions[16];
	Buffer     	sentence;
	int16_t     sentenceFields[16];
	uint8_t     tokens;
	uint8_t     token[32];

public:
	NMEAParser(const int16_t capacity);
	~NMEAParser();

	int16_t Parse(Stream &stream);
	int16_t Parse(const uint8_t *dat, const int16_t len);

	inline const Buffer& GetSentence() { return sentence; }

	uint8_t Tokens() const { return tokens; }
	const uint8_t* Token(const uint8_t index, int16_t &len);

	void Clear() { sentence.Clear(); }

protected:
	int16_t ParseSome();
};

#endif