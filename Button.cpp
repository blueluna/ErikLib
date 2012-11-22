/*
* ErikLib Button
* 2012 (c) Erik Svensson <erik.public@gmail.com>
* Licensed under the MIT license.
*/

#include "Button.h"
#include <Arduino.h>

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
