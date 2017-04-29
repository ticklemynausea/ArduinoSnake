#ifndef _display_h_
#define _display_h_

#ifndef _include_libs_adafruit_
#define _include_libs_adafruit_
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#endif _include_libs_adafruit_

/* LCD Parameters */
#define LCD_WIDTH 84
#define LCD_HEIGHT 48
#define LCD_HEIGHT8 (LCD_HEIGHT >> 3)

/* LCD Display Pinout:
 * D7 - Serial clock out (CLK oder SCLK)
 * D6 - Serial data out (DIN)
 * D5 - Data/Command select (DC oder D/C)
 * D4 - LCD chip select (CE oder CS)
 * D3 - LCD reset (RST)
 * * * * * * * * * * * * * * * * * * * * * * */
#define PIN_CLK 7
#define PIN_DIN 6
#define PIN_DC 5
#define PIN_CE 4
#define PIN_RST 3

/* checks if n is in the interval [a, b] */
#define BETWEEN(n, a, b) ((n >= a) && (n <= b))

/* iterate uint8_t x over screen width */
#define FOR_x for (uint8_t x = 0; x < LCD_WIDTH; x++)

/* iterate uint8_t y over screen height */
#define FOR_y for (uint8_t y = 0; y < LCD_HEIGHT; y++)

class Display {

  private:
  /* PCD8544 display controller */
  static Adafruit_PCD8544 controller;

  public:
  /* Initialize the LED display */
  static void Initialize();

  /* Update LCD display */
  static void Update();

  /* Clear LCD display */
  static void Clear();

  static void PixelOn(uint8_t, uint8_t);
  static void PixelOff(uint8_t, uint8_t);
  static uint8_t GetPixel(uint8_t, uint8_t);

  static void DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
  static void DrawText(uint8_t, uint8_t, const __FlashStringHelper *);
  static void DrawText(uint8_t x, uint8_t y, char * text);
  static void HorizontalSwipe();
  static void VerticalSwipe();
  static void RandomSwipe();

};

#endif
