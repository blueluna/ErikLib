/*
* ErikLib NMEA Sentence
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#include "NMEAParser.h"
#include <stdlib.h>
#include <Arduino.h>

NMEAParser::NMEAParser(const int16_t capacity)
	: parseBuffer(capacity)
	, parserPosition(0)
	, sentenceStart(-1)
	, sentenceEnd(-1)
	, sentenceChecksum(0)
	, sentence(capacity)
	, delimeterIndex(0)
	, tokens(0)
{
	for (int16_t n = 0; n < 16; n++) {
		delimterPositions[n] = -1;
		sentenceFields[n] = -1;
	}
}

NMEAParser::~NMEAParser()
{
}

int16_t NMEAParser::Parse(Stream &stream)
{
	int16_t result = parseBuffer.Transfer(stream);
	if (result < 0) {
		result = -result;
	}
	else {
		result = ParseSome();
	}
	return result;
}

int16_t NMEAParser::Parse(const uint8_t *dat, const int16_t len)
{
	int16_t result = parseBuffer.Write(dat, len);
	if (result < 0) {
		result = -result;
	}
	else {
		result = ParseSome();
	}
	return result;
}

int16_t NMEAParser::ParseSome()
{
	int16_t result = E_SUCCESS;
	int16_t endOfLine = -1;
	const uint8_t *buf = parseBuffer.Peek();
	const uint8_t *ptr = buf + parserPosition;
	const uint8_t *end = buf + parseBuffer.Used();
	while (ptr < end) {
		// bail out when new line is found
		if (*ptr == 0x0A) {
			endOfLine = parserPosition;
			break;
		}
		// mark when asterix sign is found
		if (*ptr == 0x2A) {
			sentenceEnd = parserPosition;
		}
		// mark when comma is found
		if (*ptr == 0x2C && sentenceStart > -1) {
			if (delimeterIndex < 16) {
				delimterPositions[delimeterIndex] = (parserPosition + 1);
				delimeterIndex++;
			}
		}
		// calculate checksum in message
		if (sentenceStart > -1 && sentenceEnd == -1) {
			sentenceChecksum ^= *ptr;
		}
		// mark when dollar sign is found
		if (*ptr == 0x24 && sentenceStart == -1) {
			sentenceStart = parserPosition;
			delimeterIndex = 0;
			delimterPositions[delimeterIndex] = (parserPosition + 1);
			delimeterIndex++;
		}
		parserPosition++;
		ptr++;
	}
	if (endOfLine >= 0) {
		if (endOfLine < 6 || sentenceStart == -1 || sentenceEnd == -1) {
			result = E_INVALID_VALUE;
		}
		if (result == E_SUCCESS && endOfLine > (sentenceEnd + 3)) {
			// read the checksum
			ptr = buf + (sentenceEnd + 1);
			unsigned long checksum_sentence = strtoul(reinterpret_cast<const char*>(ptr), 0, 16);
			if (sentenceChecksum != checksum_sentence) {
				result = E_INVALID_CHECKSUM;
			}
		}
		if (result == E_SUCCESS) {
			// move data to sentence buffer
			const int16_t first = sentenceStart + 1;
			sentence.Clear();
			parseBuffer.Read(0, first);
			sentence.Transfer(parseBuffer, sentenceEnd - first);
			parseBuffer.Read(0, endOfLine - sentenceEnd);

			for (uint8_t n = 0; n < 16; n++) {
				if (delimterPositions[n] >= 0) {
					sentenceFields[n] = delimterPositions[n] - first;
					delimterPositions[n] = -1;
				}
				else {
					tokens = n;
					break;
				}
			}
		}
		else {
			parseBuffer.Read(0, endOfLine + 1);
		}
		sentenceStart = -1;
		sentenceEnd = -1;
		sentenceChecksum = 0;
		parserPosition = parseBuffer.Used();
	}
	else {
		result = E_NOT_ENOUGH_DATA;
	}
	return result;
}

const uint8_t* NMEAParser::Token(const uint8_t index, int16_t &len)
{
	len = 0;
	if (index >= tokens) {
		return 0;
	}
	int16_t pos = sentenceFields[index];
	if (pos < 0) {
		return 0;
	}
	if (index == (tokens - 1)) {
		len =  sentence.Used() - sentenceFields[index];
	}
	else {
		len = sentenceFields[index+1] - sentenceFields[index] - 1;
	}
	if (len >= 32) {
		len = 0;
		return 0;
	}
	memcpy(token, sentence.Peek() + sentenceFields[index], len);
	token[len] = 0;
	return token;
}