/******************************************************************************
nvm.h
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 31, 2019

This file defines the locations in EEPROM where the configuration data is to
be stored.

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//Location in EEPROM for each thing we want to store between power cycles
enum eepromLocations {
  LOCATION_I2C_ADDRESS = 0x00, //Device's address
  LOCATION_INTERRUPTS = 0x01,
  LOCATION_LED_BRIGHTNESS = 0x02,
  LOCATION_LED_PULSEGRANULARITY = 0x03,
  LOCATION_LED_PULSECYCLETIME = 0x04,
  LOCATION_LED_PULSEOFFTIME = 0x06,
  LOCATION_BUTTON_DEBOUNCE_TIME = 0x08,
  LOCATION_ADDRESS_TYPE = 0x0A,
};
