/******************************************************************************
  registers.h
  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: July 31, 2019

  This file defines the memoryMap struct, which acts as the pseudo register map
  of the Qwiic Button/Switch. It also serves as an easy way to access variables
  and manipulate the state of the device.

  During I2C transactions, the memoryMap object is wrapped as a collection of
  bytes. The byte that the user is interested in (either to read or write) is
  selected with a register pointer. For instance, if the user sets the pointer
  to 0x0e, they will be addressing the 4th uint8_t sized object in this struct.
  In this case, that would be the interruptConfig register!

  This code is beerware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Distributed as-is; no warranty is given.
******************************************************************************/

typedef union {
  struct {
    bool eventAvailable : 1; //This is bit 0. User mutable, gets set to 1 when a new event occurs. User is expected to write 0 to clear the flag.
    bool hasBeenClicked : 1; //Defaults to zero on POR. Gets set to one when the button gets clicked. Must be cleared by the user.
    bool isPressed : 1;  //Gets set to one if button is pushed.
    bool : 5;
  };
  uint8_t byteWrapped;
} statusRegisterBitField;

typedef union {
  struct {
    bool clickedEnable : 1; //This is bit 0. user mutable, set to 1 to enable an interrupt when the button is clicked. Defaults to 0.
    bool pressedEnable : 1; //user mutable, set to 1 to enable an interrupt when the button is pressed. Defaults to 0.
    bool: 6;
  };
  uint8_t byteWrapped;
} interruptConfigBitField;

typedef union {
  struct {
    bool popRequest : 1; //This is bit 0. User mutable, user sets to 1 to pop from queue, we pop from queue and set the bit back to zero.
    bool isEmpty : 1; //user immutable, returns 1 or 0 depending on whether or not the queue is empty
    bool isFull : 1; //user immutable, returns 1 or 0 depending on whether or not the queue is full
    bool: 5;
  };
  uint8_t byteWrapped;
} queueStatusBitField;

typedef struct memoryMap {
  //Button Status/Configuration                       Register Address
  uint8_t id;                                             // 0x00
  uint8_t firmwareMinor;                                  // 0x01
  uint8_t firmwareMajor;                                  // 0x02

  statusRegisterBitField buttonStatus;                    // 0x03

  //Interrupt Configuration
  interruptConfigBitField interruptConfigure;                // 0x04
  uint16_t buttonDebounceTime;                            // 0x05

  //ButtonPressed queue manipulation and status functions
  queueStatusBitField pressedQueueStatus;                 // 0x07
  unsigned long pressedQueueFront;                        // 0x08
  unsigned long pressedQueueBack;                         // 0x0C

  queueStatusBitField clickedQueueStatus;                 // 0x10
  unsigned long clickedQueueFront;                        // 0x11
  unsigned long clickedQueueBack;                         // 0x15

  //LED Configuration
  uint8_t ledBrightness;                                  // 0x19
  uint8_t ledPulseGranularity;                            // 0x1A
  uint16_t ledPulseCycleTime;                             // 0x1B
  uint16_t ledPulseOffTime;                               // 0x1D

  //Device Configuration
  uint8_t i2cAddress;                                     // 0x1F
};
