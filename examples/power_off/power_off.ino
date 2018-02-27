/**************************************************************************/
/*!
    @file       power_off.ino
    @author     Marc-Oliver Ristau <marc.ristau@mristau.eu>
    @version    1.0

    Example Sketch to demonstrate Power Off Mode and wakeup by interrupt
    Any external interrupt would work, buttons, interrupt capable sensors, ..
    It will go to power off mode after some loop cycles.
    interrupt will reset the cycle counter.

    @section    HISTORY

    v1.0        - First Release
*/
/**************************************************************************/

#include <Arduino_nRF5x_lowPower.h> // LowPower Library for nRF5x

#define INTERRUPT_PIN   19
#define LOOP_CYCLES     500
#define CYCLE_TIME_MS   100

volatile int cycles = 0;
volatile bool int1 = false;

void intHandler() {
    cycles = 0;
    int1 = true;
}

void setup() {
    pinMode(INTERRUPT_PIN, INPUT);
  
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), intHandler, RISING);
    nRF5x_lowPower.enableWakeupByInterrupt(INTERRUPT_PIN, RISING);
}

void loop() {
  
  if (cycles >= LOOP_CYCLES) {
      nRF5x_lowPower.powerMode(POWER_MODE_OFF);
  }
   
  cycles++;
  delay(CYCLE_TIME_MS);
}