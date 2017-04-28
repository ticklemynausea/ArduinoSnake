#include "Display.h"

/* PCD8544 controller */
Adafruit_PCD8544 Display::controller = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_CE, PIN_RST);

/* bitwise buffer to do stuff in memory with screen state (1bit=1pixel) */
uint8_t Display::buffer[LCD_WIDTH * LCD_HEIGHT8];

void Display::Initialize() {

  // initialize controller
  Display::controller.begin();

  // set contrast
  Display::controller.setContrast(60);

  // clears the screen and buffer
  Display::Clear();

}

void Display::Clear() {

  /* clear matrix */
  FOR_i {
    FOR_j {
      Display::buffer[MAP(i, j)] = 0;
    }
  }

  Display::controller.clearDisplay();

  Display::controller.display();

}

void Display::Update() {
  Display::controller.display();
}

void Display::Set(uint8_t x, uint8_t y, uint8_t color) {
  BIT_WRITE_xy(Display::buffer, x, y, color);
  Display::controller.drawPixel(x, y, color);
}

void Display::Set(uint8_t x, uint8_t y) {
  BIT_SET_xy(Display::buffer, x, y);
  Display::controller.drawPixel(x, y, BLACK);
}

void Display::Unset(uint8_t x, uint8_t  y) {
  BIT_CLEAR_xy(Display::buffer, x, y);
  Display::controller.drawPixel(x, y, WHITE);
}

uint8_t Display::Read(uint8_t x, uint8_t y) {
  return BIT_READ_xy(Display::buffer, x, y);
}

void Display::FromBuffer(uint8_t x, uint8_t y) {
  Display::controller.drawPixel(x, y, BIT_READ_xy(Display::buffer, x, y) == 1 ? BLACK : WHITE);
}

void Display::DrawLine(uint8_t  x0, uint8_t  y0, uint8_t  x1, uint8_t  x2, uint8_t  color) {
  Display::controller.drawLine(x0, y0, x1, x2, color);
};

void Display::DrawText(uint8_t x, uint8_t y, String &text) {
  Display::controller.setCursor(x, y);
  Display::controller.setTextColor(WHITE, BLACK);
  Display::controller.print(text);
};


void Display::HorizontalSwipe() {

  FOR_x {

    Display::DrawLine(x, 0, x, LCD_HEIGHT - 1, BLACK);
    if (x >= 1) {
      uint16_t x2 = x - 1;
      Display::DrawLine(x2, 0, x2, LCD_HEIGHT - 1, WHITE);
    }

    if (x > 1) {
      FOR_y {
        uint16_t x2 = x - 2;
        Display::Set(x2, y);
      }
    }

    Display::Update();

  }

  for (uint8_t x = LCD_WIDTH - 2; x < LCD_WIDTH; x++) {
    FOR_y {
      Display::Set(x, y);
    }
    Display::Update();
  }

}

void Display::VerticalSwipe() {

  FOR_y {

    Display::DrawLine(0, y, LCD_WIDTH - 1, y, BLACK);

    if (y >= 1) {
      uint16_t y2 = y - 1;
      Display::DrawLine(0, y2, LCD_WIDTH - 1, y2, WHITE);
    }

    if (y > 1) {
      FOR_x {
        uint16_t y2 = y - 2;
        Display::Set(x, y2);
      }
    }

    Display::Update();

  }

  for (uint8_t y = LCD_HEIGHT - 2; y < LCD_HEIGHT; y++) {
    FOR_x {
      Display::Set(x, y);
    }
    Display::Update();
  }

}

void Display::RandomSwipe() {
  if (random(256) < 128) {
    Display::HorizontalSwipe();
  } else {
    Display::VerticalSwipe();
  }
}
