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

/*  These macros define mappings between the pixel in
 *  (x,y) coordinates in the display space and in (i,j,k)
 *  coordinates in the buffer space
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define MAP(i, j) (i + LCD_WIDTH*j)
#define MAP_i(x, y) (x)
#define MAP_j(x, y) (y/8)
#define MAP_k(x, y) (y%8)

/* These macros define bitwise read/set/clear operations
 * in the buffer space given (i,j,k) coordinates
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define BIT_READ(buffer, i, j, k) bitRead(buffer[MAP(i, j)], k)
#define BIT_SET(buffer, i, j, k) bitSet(buffer[MAP(i, j)], k)
#define BIT_CLEAR(buffer, i, j, k) bitClear(buffer[MAP(i, j)], k)
#define BIT_WRITE(buffer, i, j, k, b) bitWrite(buffer[MAP(i, j)], k, b)

/* These macros define bitwise read/set/clear operations
 * in the buffer space given (x,y) coordinates
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define BIT_SET_xy(buffer, x, y) BIT_SET(buffer, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_READ_xy(buffer, x, y) BIT_READ(buffer, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))
#define BIT_WRITE_xy(buffer, x, y, b) BIT_WRITE(buffer, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y), b)
#define BIT_CLEAR_xy(buffer, x, y) BIT_CLEAR(buffer, MAP_i(x, y),  MAP_j(x, y), MAP_k(x, y))

/* checks if n is in the interval [a, b] */
#define BETWEEN(n, a, b) ((n >= a) && (n <= b))

/* iterate uint8_t i over matrix buffer width */
#define FOR_i for (uint8_t i = 0; i < LCD_WIDTH; i++)

/* iterate uint8_t j over matrix buffer height */
#define FOR_j for (uint8_t j = 0; j < LCD_HEIGHT8; j++)

/* iterate uint8_t k over matrix buffer's bytes bits */
#define FOR_k for (uint8_t k = 0; k < 8; k++)

/* iterate uint8_t x over screen width */
#define FOR_x for (uint8_t x = 0; x < LCD_WIDTH; x++)

/* iterate uint8_t y over screen height */
#define FOR_y for (uint8_t y = 0; y < LCD_HEIGHT; y++)

class Display {

  private:
  /* PCD8544 display controller */
  static Adafruit_PCD8544 controller;

  /* bitwise matrix to do stuff in memory with screen state (1bit=1pixel) */
  static uint8_t buffer[LCD_WIDTH * LCD_HEIGHT8];

  public:
  /* Initialize the LED display */
  static void Initialize();

  /* Update LCD display */
  static void Update();

  /* Clear LCD display */
  static void Clear();

  static void Set(uint8_t  x, uint8_t y, uint8_t color);
  static void Set(uint8_t  x, uint8_t y);
  static void Unset(uint8_t  x, uint8_t y);
  static uint8_t Read(uint8_t x, uint8_t y);
  static void FromBuffer(uint8_t  x, uint8_t y);

  static void DrawLine(uint8_t x0, uint8_t  y0, uint8_t  x1, uint8_t  x2, uint8_t color);
  static void DrawText(uint8_t x, uint8_t y, String &text);

  static void HorizontalSwipe();
  static void VerticalSwipe();
  static void RandomSwipe();

};

#endif
