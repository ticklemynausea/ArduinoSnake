#include "Routine.h"

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
String Routine::text[];

void Routine::Initialize() {

  Routine::score = 0;
  Routine::cursor_x = LCD_WIDTH / 2;
  Routine::cursor_y = LCD_HEIGHT / 2;
  Routine::cursor_d = 0;
  Routine::delayTime = 100;

  randomSeed(analogRead(RANDOMSEED_PIN));

  pinMode(JS_S_PIN, INPUT);
  digitalWrite(JS_S_PIN, HIGH);


  #ifdef DEBUG
    Serial.begin(115198);
  #endif

}

void Routine::Step() {

  Routine::score++;

  Routine::js_switch = digitalRead(JS_S_PIN);
  Routine::js_x_reading = analogRead(JS_X_PIN);
  Routine::js_y_reading = analogRead(JS_Y_PIN);
  Routine::js_x_axis = round(map(Routine::js_x_reading, 50, 950, 0, JS_READ_RANGE));
  Routine::js_y_axis = round(map(Routine::js_y_reading, 50, 950, 0, JS_READ_RANGE));

  if (Routine::js_x_axis == 1 && Routine::js_y_axis == 0 && Routine::cursor_d != 2) Routine::cursor_d = 0;
  if (Routine::js_x_axis == 2 && Routine::js_y_axis == 1 && Routine::cursor_d != 3) Routine::cursor_d = 1;
  if (Routine::js_x_axis == 1 && Routine::js_y_axis == 2 && Routine::cursor_d != 0) Routine::cursor_d = 2;
  if (Routine::js_x_axis == 0 && Routine::js_y_axis == 1 && Routine::cursor_d != 1) Routine::cursor_d = 3;                                                                                                                                                                                                                                                                                                                          ;

  if (Routine::cursor_d == 0 && Routine::cursor_y > 0)              Routine::cursor_y--;
  if (Routine::cursor_d == 1 && Routine::cursor_x < (LCD_WIDTH-1))  Routine::cursor_x++;
  if (Routine::cursor_d == 2 && Routine::cursor_y < (LCD_HEIGHT-1)) Routine::cursor_y++;
  if (Routine::cursor_d == 3 && Routine::cursor_x > 0)              Routine::cursor_x--;

#ifdef DEBUG
  Serial.print("Switch:  ");
  Serial.print(js_switch);
  Serial.print("\t");
  Serial.print("X-axis: ");
  Serial.print(js_x_axis);
  Serial.print("\t");
  Serial.print("Y-axis: ");
  Serial.print(js_y_axis);
  Serial.print("\t");
  Serial.print("Direction: ");
  Serial.print(cursor_d);
  Serial.print("\t");
  Serial.print("X-cursor: ");
  Serial.print(cursor_x);
  Serial.print("\t");
  Serial.print("Y-cursor: ");
  Serial.print(cursor_y);
  Serial.print("\n");
#endif

}

void Routine::Update() {
    Display::Set(cursor_x, cursor_y);
    Display::Update();

    delay(Routine::delayTime);

}

bool Routine::HasCollided() {
  return Display::Read(Routine::cursor_x, Routine::cursor_y) == 1;
}

void Routine::GameOver() {

  Display::RandomSwipe();

  Routine::text[0] = String("GAME OVER LOL");
  Routine::text[1] = String("SCORE: ") + String(score);

  Display::DrawText(0, 0, text[0]);
  Display::DrawText(0, 20, text[1]);

  Display::Update();
  delay(1000);

  Display::Clear();

  Routine::score = 0;
  Routine::cursor_x = random(10, LCD_WIDTH - 10);
  Routine::cursor_y = random(10, LCD_HEIGHT - 10);
  Routine::cursor_d = random(0, 3);

}
