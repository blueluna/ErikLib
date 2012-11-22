// ErikLib
// Copyright 2012 Erik Svensson <erik.public@gmail.com>

#ifndef	ERIKLIB_H
#define ERIKLIB_H

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <Print.h>
#include <Printable.h>

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

class NMEASentence
	: public Print
	, public Printable
{
protected:
	size_t 	bufferCapacity;
	uint8_t *buffer;
	size_t 	bufferCount;

public:
	NMEASentence(size_t capacity);
	~NMEASentence();

    void 	clear();

    size_t 	write(uint8_t);
    size_t 	write(const uint8_t *buffer, size_t size);

    size_t 	printTo(Print& p) const;
};

class NMEAParser
{
protected:
public:
	NMEAParser(size_t capacity);
};

class Button
{
public:	
	enum {
	  Low     = 0x00,
	  High    = 0x01,
	  Rising  = 0x02,
	  Falling = 0x03
	};

protected:
	uint8_t btnPin;
	uint8_t btnState;
	uint32_t btnTimeout;
	uint32_t btnTime;

public:
	Button(const uint8_t pin, const uint32_t timeout);
	
	uint8_t Check(const uint32_t t);
	uint8_t Check();
};

#endif
