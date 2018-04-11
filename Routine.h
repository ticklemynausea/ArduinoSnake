#ifndef _routine_h_
#define _routine_h_

#include "Display.h"

#define DEBUG 1

#define KEY_LEFT A0
#define KEY_UP A1
#define KEY_DOWN A2
#define KEY_RIGHT A3 

/* random seed floating pin */
#define RANDOMSEED_PIN 2

#define BASE_DELAY 100
#define SNAKE_INIT_LENGTH 10
#define SNAKE_MAX_LENGTH 100
#define FOOD_INIT_LENGTH 10
#define FOOD_MAX_LENGTH 200

class Routine {

  public:
  /* Initialize the game */
  static void Initialize();

  /* Implements each frame of the game logic */
  static void Step();

  private:
  /* Game state */
  /* Array of x,y positions the snake is currently in and its length */
  static uint8_t snake[][2];
  static uint16_t snakeLength;
  /* Array of x,y positions the food pieces are currently in and its length */
  static uint8_t food[][2];
  static uint16_t foodLength;

  /* Game level and score */
  static uint8_t level;
  static uint16_t score;

  /* Snake's head position and current direction */
  static uint8_t cursor_x;
  static uint8_t cursor_y;
  static uint8_t cursor_d;

  /* Button states and handling routines */
  static uint8_t buttons[2];
  static uint8_t buttons_state[2];
  static uint8_t buttons_state_prev[2];
  static void (*buttons_callbacks[2])();

  /* Move Snake  */
  static void MoveLeft(); 
  static void MoveRight();
  
  /* Initialize a new level */
  static void NewLevel();

  /* Read joystick input */
  static void ReadInput();

  /* Test if snake has collided with itself or a barrier */
  static bool IsCollision();

  /* Add a new element to the head of the snake */
  static void AddSnake();

  /* Pop an element from the back of the snakeLength */
  static void PopSnake();

  /* Add a food item to a random screen position */
  static void AddFood();

  /* Remove the food item at the cursor's position from the food array, if it exists. returns false if it doesn't. */
  static bool EatFood();

  /* Game over screen */
  static void GameOver();

};


#endif
