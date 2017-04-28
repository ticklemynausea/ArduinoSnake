#ifndef _routine_h_
#define _routine_h_

#include "Display.h"

#define RANDOMSEED_PIN 2
#define JS_S_PIN 8
#define JS_X_PIN 0
#define JS_Y_PIN 1
#define JS_READ_RANGE 2

class Routine {

  public:
  static void Initialize();
  static void Step();
  static void Update();
  static bool HasCollided();
  static void GameOver();

  private:
  static String text[4];
  static uint16_t score;
  static uint16_t delayTime;
  static uint8_t cursor_x;
  static uint8_t cursor_y;
  static uint8_t cursor_d;
  static uint8_t js_switch;
  static float js_x_reading;
  static float js_y_reading;
  static uint8_t js_x_axis;
  static uint8_t js_y_axis;

};


#endif
