// ErikLib
// 2012 (c) Erik Svensson <erik.public@gmail.com>

#include "ErikLib.h"
#include <Arduino.h>

size_t ErikLib::HexPrint(Print& p, const uint64_t value, const uint8_t bytes)
{
  uint8_t nibbles = bytes << 1;
  uint8_t buf[17] = {0};
  uint64_t data = value;
  uint8_t nibble;
  char c;
  for (int i = (nibbles - 1); i >= 0; i--) {
    nibble = data & 0x0F;
    if (nibble <= 9) {
      c = 0x30 + nibble;
    }
    else {
      c = 0x37 + nibble; // 'A' - 10 + nibble 
    }
    buf[i] = c;
    data = data >> 4;
  }
  return p.write(buf, nibbles);
}

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


Button::Button(const uint8_t pin, const uint32_t timeout)
	: btnPin(pin)
	, btnState(Button::High)
	, btnTimeout(timeout)
	, btnTime(millis())
{
	btnState = digitalRead(btnPin) << 4;
}

uint8_t Button::Check(const uint32_t t)
{
	uint8_t result = btnState & 0x0f;
	uint8_t state = digitalRead(btnPin);
	uint8_t newState = btnState >> 4;
	if (state == newState) {
	  if (t >= (btnTime + btnTimeout)) {
	    result = newState;
	    if (btnState != newState) {
	      result = newState == Button::High ? Button::Rising : Button::Falling;
	    }
	    btnState = (btnState & 0xf0) | newState;
	  }
	}
	else {
	  btnTime = t;
	  btnState = (btnState & 0x0f) | state << 4;
	}
	return result;
}

uint8_t Button::Check()
{
	return Check(millis());
}