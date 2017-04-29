#ifndef _routine_h_
#define _routine_h_

#include "Display.h"

#define RANDOMSEED_PIN 2
#define JS_S_PIN 8
#define JS_X_PIN 0
#define JS_Y_PIN 1
#define JS_READ_RANGE 2
#define SNAKE_INITIAL_LENGTH 20

class Routine {

  public:
  /* Initialize the game */
  static void Initialize();

  /* Implements each frame of the game logic */
  static void Step();

  private:
  /* Game state */
  static uint8_t snake[][2];
  static uint16_t snakeLength;

  /* Game score and delay between each frame */
  static uint16_t score;
  static uint16_t delayTime;

  /* Snake's head position and current direction */
  static uint8_t cursor_x;
  static uint8_t cursor_y;
  static uint8_t cursor_d;

  /* Joystick input variables */
  static uint8_t js_switch;
  static float js_x_reading;
  static float js_y_reading;
  static uint8_t js_x_axis;
  static uint8_t js_y_axis;

  /* Strings for game over message */
  static String text[];

  /* Initialize a new game */
  static void NewGame();

  /* Read joystick input */
  static void ReadInput();

  /* Add a new element to the head of the snake */
  static void AddSnake();

  /* Pop an element from the back of the snakeLength */
  static void PopSnake();

  /* Test if snake has collided with itself or a barrier */
  static bool HasCollided();

  /* Game over screen */
  static void GameOver();

};


#endif
