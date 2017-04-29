#include "Routine.h"

uint8_t Routine::snake[100][2];
uint16_t Routine::snakeLength = 0;

uint16_t Routine::score;
uint16_t Routine::delayTime;

uint8_t Routine::cursor_x;
uint8_t Routine::cursor_y;
uint8_t Routine::cursor_d;

uint8_t Routine::js_switch;
float Routine::js_x_reading;
float Routine::js_y_reading;
uint8_t Routine::js_x_axis;
uint8_t Routine::js_y_axis;

String Routine::text[4];

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
  NewGame();

}

void Routine::NewGame() {

    Display::Clear();

    Display::DrawLine(0, 0, LCD_WIDTH - 1, 0, BLACK);
    Display::DrawLine(0, LCD_HEIGHT-1, LCD_WIDTH-1, LCD_HEIGHT - 1, BLACK);
    Display::DrawLine(0, 0, 0, LCD_HEIGHT-1, BLACK);
    Display::DrawLine(LCD_WIDTH-1, 0, LCD_WIDTH-1, LCD_HEIGHT- 1, BLACK);

    score = 0;
    cursor_x = random(30, LCD_WIDTH - 30);
    cursor_y = random(30, LCD_HEIGHT - 30);
    cursor_d = random(0, 3);
    delayTime = 100;

    snakeLength = 0;
    for (uint8_t i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
      AddSnake();
    }

    Display::Update();

}

void Routine::AddSnake() {

  snake[snakeLength][0] = cursor_x;
  snake[snakeLength][1] = cursor_y;
  snakeLength++;

  Display::PixelOn(cursor_x, cursor_y);

  // move cursor to the next position
  if (cursor_d == 0 && cursor_y > 0)              cursor_y--;
  if (cursor_d == 1 && cursor_x < (LCD_WIDTH-1))  cursor_x++;
  if (cursor_d == 2 && cursor_y < (LCD_HEIGHT-1)) cursor_y++;
  if (cursor_d == 3 && cursor_x > 0)              cursor_x--;

}

void Routine::PopSnake() {

  Display::PixelOff(snake[0][0], snake[0][1]);

  for (uint16_t i = 1; i < snakeLength; i++) {
    snake[i-1][0] = snake[i][0];
    snake[i-1][1] = snake[i][1];
  }

  snakeLength--;

}

void Routine::ReadInput() {

  js_switch = digitalRead(JS_S_PIN);
  js_x_reading = analogRead(JS_X_PIN);
  js_y_reading = analogRead(JS_Y_PIN);
  js_x_axis = round(map(js_x_reading, 50, 950, 0, JS_READ_RANGE));
  js_y_axis = round(map(js_y_reading, 50, 950, 0, JS_READ_RANGE));

  if (js_x_axis == 1 && js_y_axis == 0 && cursor_d != 2) cursor_d = 0;
  if (js_x_axis == 2 && js_y_axis == 1 && cursor_d != 3) cursor_d = 1;
  if (js_x_axis == 1 && js_y_axis == 2 && cursor_d != 0) cursor_d = 2;
  if (js_x_axis == 0 && js_y_axis == 1 && cursor_d != 1) cursor_d = 3;

}

void Routine::Step() {

  score++;

  ReadInput();

  AddSnake();

  PopSnake();

  if (HasCollided()) {
    GameOver();
    NewGame();
  }

  Display::Update();

  delay(delayTime);

#ifdef DEBUG
  Serial.print(F("Switch:  "));
  Serial.print(js_switch);
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

}

bool Routine::HasCollided() {
  return Display::GetPixel(cursor_x, cursor_y) == 1;
}

void Routine::GameOver() {

  Display::RandomSwipe();

  text[0] = String(F("GAME OVER LOL"));
  text[1] = String(F("SCORE: ")) + String(score);

  Display::DrawText(1, 1, text[0]);
  Display::DrawText(1, 11, text[1]);
  Display::Update();
  delay(2000);

}
