/*
* ErikLib
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

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
