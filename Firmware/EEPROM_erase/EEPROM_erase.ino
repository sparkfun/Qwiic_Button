#include "EEPROM.h"

void setup() {
  for (int i = 0; i < 10; i++)
    EEPROM.put(i, 0xFF);
}
