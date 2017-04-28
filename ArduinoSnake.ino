#include "Routine.h"
#include "Display.h"


String line1;
String line2;

void setup() {

  /* Initialize game routines */
  Routine::Initialize();

  /* Initialize Nokia LCD display */
  Display::Initialize();

}



void loop() {

  Routine::Step();

  if (Routine::HasCollided()) {
    Routine::GameOver();
  }

  Routine::Update();

  delay(100);


}
