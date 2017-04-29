#include "Display.h"

/* PCD8544 controller */
Adafruit_PCD8544 Display::controller = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

void Display::Initialize() {

  // initialize controller
  controller.begin();

  // set contrast
  controller.setContrast(50);

  // clears the screen
  Clear();

}

void Display::Clear() {

  controller.clearDisplay();
  controller.display();

}

void Display::Update() {
  controller.display();
}

void Display::PixelOn(uint8_t x, uint8_t y) {
  controller.drawPixel(x, y, BLACK);
}

void Display::PixelOff(uint8_t x, uint8_t  y) {
  controller.drawPixel(x, y, WHITE);
}

uint8_t Display::GetPixel(uint8_t x, uint8_t y) {
  return controller.getPixel(x, y);
}

void Display::DrawLine(uint8_t  x0, uint8_t  y0, uint8_t  x1, uint8_t  x2, uint8_t  color) {
  controller.drawLine(x0, y0, x1, x2, color);
};

void Display::DrawText(uint8_t x, uint8_t y, String &text) {

  controller.setCursor(x, y);
  controller.setTextColor(WHITE, BLACK);
  controller.print(text);

};

void Display::HorizontalSwipe() {

  FOR_x {
    DrawLine(x, 0, x, LCD_HEIGHT - 1, BLACK);
    Update();
    delay(10);
  }

}

void Display::VerticalSwipe() {

  FOR_y {
    DrawLine(0, y, LCD_WIDTH - 1, y, BLACK);
    Update();
    delay(10);
  }

}

void Display::RandomSwipe() {

  if (random(256) < 128) {
    HorizontalSwipe();
  } else {
    VerticalSwipe();
  }

}
