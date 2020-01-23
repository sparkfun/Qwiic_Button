/******************************************************************************
led.h
Fischer Moseley @ SparkFun Electronics
Original Creation Date: July 31, 2019

This file defines the LEDconfig struct, which provides an easy interface for
handling how the LED behaves, in addition to storing configuration variables.

On the Qwiic Button, this LED is built into the button itself! 
On the Qwiic Switch, a pin for an external LED is provided on the 0.1" header
located on the bottom of the switch.

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/

//manages LED operations, stores configuration variables internally and provides an easy interface to updating the LED's status and blinking it
struct LEDconfig {

  //variables imported from registerMap
  uint8_t brightness = 0;  //Brightness of LED. If pulse cycle enabled, this is the max brightness of the pulse.
  uint8_t pulseGranularity = 0;  //Number of steps to take to get to ledBrightness. 1 is best for most applications.
  uint16_t pulseCycleTime = 0; //Total pulse cycle in ms, does not include off time. LED pulse disabled if zero.
  uint16_t pulseOffTime = 0; //Off time between pulses, in ms

  //variables that we calculate from the imported variables, and use internally
  unsigned long adjustmentStartTime; //Start time of micro adjustment
  unsigned long pulseStartTime; //Start time of overall LED pulse
  uint16_t pulseLedAdjustments; //Number of adjustments to LED to make: registerMap.ledBrightness / registerMap.ledPulseGranularity * 2
  unsigned long timePerAdjustment; //ms per adjustment step of LED: registerMap.ledPulseCycleTime / pulseLedAdjustments
  int16_t pulseLedBrightness = 0; //Analog brightness of LED
  int16_t brightnessStep; //Granularity but will become negative as we pass max brightness

  //updates all the LED variables, and resets the pulseValues if necessary
  void update(struct memoryMap* map) {
    //check if any of the values are different
    bool different = false;
    if(map->ledBrightness != brightness) different = true; 
    if(map->ledPulseGranularity != pulseGranularity) different = true; 
    if(map->ledPulseCycleTime != pulseCycleTime) different = true; 
    if(map->ledPulseOffTime != pulseOffTime) different = true; 
    
    //if they are different, calculate new values and then reset everything
    if(different) {
      brightness = map->ledBrightness;
      pulseGranularity = map->ledPulseGranularity;
      pulseCycleTime = map->ledPulseCycleTime;
      pulseOffTime = map->ledPulseOffTime;
      calculatePulseValues();
      resetPulseValues();
    }
  }
  
  //Calculate LED values based on pulse settings
  void calculatePulseValues() {
    pulseLedAdjustments = ceil((float)brightness / pulseGranularity * 2.0);
    timePerAdjustment = round((float)pulseCycleTime / pulseLedAdjustments);
    brightnessStep = pulseGranularity;
  }

  //At the completion of a pulse cycle, reset timers
  void resetPulseValues() {
    pulseStartTime = millis();
    adjustmentStartTime = millis();
    brightnessStep = pulseGranularity;
    pulseLedBrightness = 0;
  }

  void pulse(uint8_t ledPin) {
    //Pulse LED on/off based on settings
    //Brightness will increase with each cycle based on granularity value (5)
    //To get to max brightness (207) we will need ceil(207/5*2) LED adjustments = 83
    //At each discrete adjustment the LED will spend X amount of time at a brightness level
    //Time spent at this level is calc'd by taking total time (1000 ms) / number of adjustments / up/down (2) = 12ms per step

    if (pulseCycleTime == 0) { //Just set the LED to a static value if cycle time is zero
      analogWrite(ledPin, brightness);
      return;
    }

    if (pulseCycleTime > 0) { //Otherwise run in cyclic mode
      if (millis() - pulseStartTime <= pulseCycleTime){
        //Pulse LED
        //Change LED brightness if enough time has passed
        if (millis() - adjustmentStartTime >= timePerAdjustment) {
          pulseLedBrightness += brightnessStep;

          //If we've reached a max brightness, reverse step direction
          if (pulseLedBrightness > (brightness - pulseGranularity)) {
            brightnessStep *= -1;
          }

          if (pulseLedBrightness < 0) pulseLedBrightness = 0;
          if (pulseLedBrightness > 255) pulseLedBrightness = 255;

          analogWrite(ledPin, pulseLedBrightness);
          adjustmentStartTime = millis();
        }
      }

      else if (millis() - pulseStartTime <= pulseCycleTime + pulseOffTime) {
        //LED off
        analogWrite(ledPin, 0);
      }

      else {
        //Pulse cycle and off time have expired. Reset pulse settings.
        resetPulseValues();
      }
    }
  }
};
