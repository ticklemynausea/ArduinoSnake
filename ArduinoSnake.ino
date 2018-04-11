#include "Routine.h"

#define _use_esp8266_display_ 1
#define _use_adafruit_display_ 2
//#define _use_display_ _use_esp8266_display_
#define _use_ _use_adafruit_display_

void setup() {
  Routine::Initialize();
}

void loop() {
  Routine::Step();
}
