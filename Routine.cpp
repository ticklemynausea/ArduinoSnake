#include "Routine.h"

uint8_t Routine::snake[SNAKE_MAX_LENGTH][2];
uint16_t Routine::snakeLength = 0;
uint8_t Routine::food[FOOD_MAX_LENGTH][2];
uint16_t Routine::foodLength = 0;

uint8_t Routine::level;
uint16_t Routine::score;

uint8_t Routine::cursor_x;
uint8_t Routine::cursor_y;
uint8_t Routine::cursor_d;

uint8_t Routine::switchClicked = 0;
uint8_t Routine::switchPrevState = 0;

void Routine::Initialize() {

  /* Initialize random number seed */
  randomSeed(analogRead(RANDOMSEED_PIN));

  /* Initialize joystick click */
  pinMode(JS_S_PIN, INPUT);
  digitalWrite(JS_S_PIN, HIGH);

  /* Initialize Nokia LCD display */
  Display::Initialize();

  #ifdef DEBUG
    Serial.begin(115198);
  #endif

  /* Initialize game */
  level = 1;
  score = 0;
  NewLevel();

}

void Routine::Step() {

  /* true if the snake ate food in this step */
  bool ateFood = false;

  /* update cursor from joystick input */
  ReadInput();

  /* Add a new pixel in the direction the snake is going */
  AddSnake();

  /* Test collisions with existing pixels */
  if (IsCollision()) {

    /* Try to eat food at current cursor location */
    ateFood = EatFood();

    /* If it didn't eat food, it hit a wall. Game Over */
    if (!ateFood) {
      GameOver();
      NewLevel();
    }

    /* If ate all the food */
    if (foodLength == 0) {
      level++;
      NewLevel();
    }
  }

  /* Remove the last pixel in the snake's tail if the snake didn't eat food
   * or if we have reached max size */
  if (!ateFood || snakeLength == SNAKE_MAX_LENGTH) {
    PopSnake();
  }

  Display::Update();

  delay(BASE_DELAY / level);

}

void Routine::ReadInput() {

  uint8_t js_switch = digitalRead(JS_S_PIN);
  float js_x_reading = analogRead(JS_X_PIN);
  float js_y_reading = analogRead(JS_Y_PIN);

  /* js_switch reads 0 when switch is down and 1 when it is up.
   * switchClicked will be 1 after a full click. */
  if (js_switch == 0) {
    if (switchPrevState == 0) {
      switchClicked = 1;
      switchPrevState = switchClicked;
    }
  } else {
    if (switchPrevState == 1) {
      switchClicked = 0;
      switchPrevState = switchClicked;
    }
  }

  /* map all read values into a smaller range (int8_t) */
  int8_t js_x_axis = map(round(js_x_reading), 0, 1023, -128, 127);
  int8_t js_y_axis = map(round(js_y_reading), 0, 1023, -128, 127);

  /* shoddy ascii explaining how inputs are mapped to snake movements:
   * with the current orientation of the joypad in the physical setup, readings will look like this:
   *
   * (127, -128) +-----------+ (-128, -128)
   *             | \       / |
   *             |  \  a  /  |
   *             |   \   /   |
   *             | d  \ /  b |    x <---+
   *             |    / \    |          |
   *             |   /   \   |          v
   *             |  /  c  \  |          y
   *             | /       \ |
   *  (127, 127) +-----------+ (-128,127)
   *
   * since the oblique lines are y = x and y = -x it's easy to use simple analitical geometry
   * to determine which of the a,b,c,d areas the joystick movement falls in, and then map
   * them to cursor directions. i also used a small threshold on the joystick readings to
   * avoid accidental movements with joystick light movements.
   */
  if ((abs(js_x_axis) > 20) || (abs(js_y_axis) > 20)) {
    if ((js_y_axis < js_x_axis) && (js_y_axis < - js_x_axis) && (cursor_d != 2)) cursor_d = 0;
    if ((js_y_axis > js_x_axis) && (js_y_axis < - js_x_axis) && (cursor_d != 1)) cursor_d = 3;
    if ((js_y_axis > js_x_axis) && (js_y_axis > - js_x_axis) && (cursor_d != 0)) cursor_d = 2;
    if ((js_y_axis < js_x_axis) && (js_y_axis > - js_x_axis) && (cursor_d != 3)) cursor_d = 1;
  }

#ifdef DEBUG
  Serial.print(F("Switch:  "));
  Serial.print(js_switch);
  Serial.print("\t");
  Serial.print(F("Switch Pressed:  "));
  Serial.print(switchClicked);
  Serial.print("\t");
  Serial.print(F("Switch Prev:  "));
  Serial.print(switchPrevState);
  Serial.print("\t");
  Serial.print(F("X-axis: "));
  Serial.print(js_x_axis);
  Serial.print("\t");
  Serial.print(F("Y-axis: "));
  Serial.print(js_y_axis);
  Serial.print("\t");
  Serial.print(F("Direction: "));
  Serial.print(cursor_d);
  Serial.print("\t");
  Serial.print(F("X-cursor: "));
  Serial.print(cursor_x);
  Serial.print("\t");
  Serial.print(F("Y-cursor: "));
  Serial.print(cursor_y);
  Serial.print(F("\n"));
#endif

  delay(50);

}

void Routine::AddSnake() {

  snake[snakeLength][0] = cursor_x;
  snake[snakeLength][1] = cursor_y;
  snakeLength++;

  Display::PixelOn(cursor_x, cursor_y);

  // move cursor to the next position
  if (cursor_d == 0 && cursor_y > 0)              cursor_y--;
  if (cursor_d == 1 && cursor_x < (LCD_WIDTH-1))  cursor_x--;
  if (cursor_d == 2 && cursor_y < (LCD_HEIGHT-1)) cursor_y++;
  if (cursor_d == 3 && cursor_x > 0)              cursor_x++;

}

void Routine::PopSnake() {

  Display::PixelOff(snake[0][0], snake[0][1]);

  for (uint16_t i = 1; i < snakeLength; i++) {
    snake[i-1][0] = snake[i][0];
    snake[i-1][1] = snake[i][1];
  }

  snakeLength--;

}

void Routine::AddFood() {

  if (foodLength < FOOD_MAX_LENGTH) {

    uint8_t random_x = random(1, LCD_WIDTH-2);
    uint8_t random_y = random(1, LCD_HEIGHT-2);

    food[foodLength][0] = random_x;
    food[foodLength][1] = random_y;
    foodLength++;

    Display::PixelOn(random_x, random_y);

  }

}

bool Routine::EatFood() {

  for (uint8_t i = 0; i < foodLength; i++) {

    if (cursor_x == food[i][0] && cursor_y == food[i][1]) {

      score++;

      for (i = i + 1; i < foodLength; i++) {
        food[i-1][0] = food[i][0];
        food[i-1][1] = food[i][1];
      }

      foodLength--;

      return true;

    }

  }

  return false;

}

bool Routine::IsCollision() {
  return Display::GetPixel(cursor_x, cursor_y) == 1;
}


void Routine::NewLevel() {

    char strbuf[15];

    /* level intro */
    Display::Clear();
    Display::HorizontalSwipe();
    Display::DrawText(1, 1, F("LEVEL BEGIN!"));
    Display::DrawText(1, 11, F("LEVEL:"));
    Display::DrawText(40, 11, itoa(level, strbuf, 10));
    Display::DrawText(1, 21, F("SCORE:"));
    Display::DrawText(40, 21, itoa(score, strbuf, 10));
    Display::Update();

    while (!switchClicked) {
      ReadInput();
    }
    delay(100);


    /* clear the screen */
    Display::Clear();

    /* draw a border around the screen */
    Display::DrawLine(0, 0, LCD_WIDTH - 1, 0, BLACK);
    Display::DrawLine(0, LCD_HEIGHT-1, LCD_WIDTH-1, LCD_HEIGHT - 1, BLACK);
    Display::DrawLine(0, 0, 0, LCD_HEIGHT-1, BLACK);
    Display::DrawLine(LCD_WIDTH-1, 0, LCD_WIDTH-1, LCD_HEIGHT- 1, BLACK);

    /* initialize variables */
    cursor_x = random(30, LCD_WIDTH - 30);
    cursor_y = random(30, LCD_HEIGHT - 30);
    cursor_d = 1;

    /* initialize snake */
    snakeLength = 0;
    for (uint8_t i = 0; i < SNAKE_INIT_LENGTH * level && i < SNAKE_MAX_LENGTH; i++) {
      AddSnake();
    }

    /* initialize food */
    foodLength = 0;
    for (uint8_t i = 0; i < FOOD_INIT_LENGTH * level && i < FOOD_MAX_LENGTH; i++) {
      AddFood();
    }

    /* update screen */
    Display::Update();

}

void Routine::GameOver() {

  char strbuf[15];

  Display::VerticalSwipe();

  Display::DrawText(1, 1, F("GAME OVER LOL"));
  Display::DrawText(1, 11, F("LEVEL:"));
  Display::DrawText(40, 11, itoa(level, strbuf, 10));
  Display::DrawText(1, 21, F("SCORE:"));
  Display::DrawText(40, 21, itoa(score, strbuf, 10));
  Display::Update();

  level = 1;
  score = 0;

  while (!switchClicked) {
    ReadInput();
  }
  delay(100);

}
