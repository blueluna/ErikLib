/*
* ErikLib Button
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/


#ifndef	ERIKLIB_BUTTON_H
#define ERIKLIB_BUTTON_H

#include <inttypes.h>

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