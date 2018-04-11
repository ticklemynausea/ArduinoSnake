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

uint8_t Routine::buttons[2] = {A0, A1};
uint8_t Routine::buttons_state[2] = {HIGH, HIGH};
uint8_t Routine::buttons_state_prev[2] = {HIGH, HIGH};
void (*Routine::buttons_callbacks[2])() = {
  &Routine::MoveLeft,
  &Routine::MoveRight,
};
  
void Routine::Initialize() {

  /* Initialize random number seed */
  randomSeed(analogRead(RANDOMSEED_PIN));

  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

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

void Routine::NewLevel() {

    char strbuf[15];

    /* level intro */
    Display::HorizontalSwipe();
    Display::DrawText(1, 1, F("LEVEL BEGIN!"));
    Display::DrawText(1, 11, F("LEVEL:"));
    Display::DrawText(40, 11, itoa(level, strbuf, 10));
    Display::DrawText(1, 21, F("SCORE:"));
    Display::DrawText(40, 21, itoa(score, strbuf, 10));
    Display::Update();

    delay(2000);

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

void Routine::ReadInput() {

  for (uint8_t i = 0; i <= 1; i++) {
    Routine::buttons_state[i] = digitalRead(Routine::buttons[i]);
    if (Routine::buttons_state[i] == LOW && Routine::buttons_state_prev[i] == HIGH) {
        Routine::buttons_callbacks[i]();
        Routine::buttons_state_prev[i] = LOW; 
    } else if (Routine::buttons_state[i] == HIGH && Routine::buttons_state_prev[i] == LOW) {
        Routine::buttons_state_prev[i] = HIGH; 
    }
  }

}

void Routine::MoveLeft() {
   if (Routine::cursor_d == 3) {
      Routine::cursor_d = 0; 
   } else {
      Routine::cursor_d++; 
   }
}

void Routine::MoveRight() {
   if (Routine::cursor_d == 0) {
      Routine::cursor_d = 3; 
   } else {
      Routine::cursor_d--; 
   }
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

  delay(2000);

}
