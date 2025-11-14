//Libraries must be installed to your Arduino environement from:
//https://www.lcdwiki.com/4.0inch_SPI_Module_ST7796
#include <LCDWIKI_SPI.h>
#include <LCDWIKI_GUI.h>

enum Color : uint16_t {
  BLACK   = 0x0000,
  BLUE    = 0x001F,
  RED     = 0xF800,
  GREEN   = 0x07E0,
  CYAN    = 0x07FF,
  MAGENTA = 0xF81F,
  YELLOW  = 0xFFE0,
  WHITE   = 0xFFFF
};

enum Rotation : uint8_t {
  ROTATION_0,
  ROTATION_90,
  ROTATION_180,
  ROTATION_270
};

//Pin Labels:       MODEL, CS, CD, MISO, MOSI, RST, SCK, LED
LCDWIKI_SPI mylcd(ST7796S, 10,  9,   12,   11,   8,  13,  -1);

//GameOutput class for real screen
class GameOutput {
private:
  static constexpr uint8_t displayRotation = ROTATION_180;
  int16_t displayW, displayH, effW, effH;
  int16_t cellSize;
  int16_t playW, playH, playX, playY;

  GameGrid prevGrid;
  bool prevValid = false;

  void mapCell(int16_t gx, int16_t gy, int16_t playX, int16_t playY, int16_t cellSize, int16_t &outX, int16_t &outY) {
    switch(displayRotation) {
      case ROTATION_0:
        outX = playX + gx * cellSize;
        outY = playY + gy * cellSize;
        break;
      case ROTATION_90:
        outX = playX + gy * cellSize;
        outY = playY + (gameGrid.width - 1 - gx) * cellSize;
        break;
      case ROTATION_180:
        outX = playX + (gameGrid.width - 1 - gx) * cellSize;
        outY = playY + (gameGrid.height - 1 - gy) * cellSize;
        break;
      case ROTATION_270:
        outX = playX + (gameGrid.height - 1 - gy) * cellSize;
        outY = playY + gx * cellSize;
        break;
    }
  }

  void drawPlayfieldBackground() {
    mylcd.Set_Draw_color(BLUE);
    mylcd.Fill_Rectangle(playX, playY, playX + playW - 1, playY + playH - 1);
  }

public:
  GameOutput() {
    //nothing atm
  }

  void init() {
    Serial.println("Initializing GameOutput");
    Serial.println(sizeof(mylcd));
    mylcd.Init_LCD();
    //Moved math here to init since it doesn't change
    displayW = mylcd.Get_Display_Width();
    displayH = mylcd.Get_Display_Height();

    if (displayRotation == ROTATION_0 || displayRotation == ROTATION_180)
      effW = gameGrid.width, effH = gameGrid.height;
    else
      effW = gameGrid.height, effH = gameGrid.width;

    int16_t cellSize = min(displayW / effW, displayH / effH);

    int16_t playW = cellSize * effW;
    int16_t playH = cellSize * effH;

    int16_t playX = (displayW - playW) / 2;
    int16_t playY = (displayH - playH) / 2;

    delay(200);
    mylcd.Fill_Screen(BLACK);
    drawPlayfieldBackground();
    prevValid = false;
  }

  void updateScreen() {
    for (uint8_t y = 0; y < gameGrid.height; y++) {
      for (uint8_t x = 0; x < gameGrid.width; x++) {
        bool filled = gameGrid.get(y, x);

        if (!prevValid || prevGrid.get(y, x) != filled) {
          int16_t sx, sy;

          mapCell(x, y, playX, playY, cellSize, sx, sy);

          mylcd.Set_Draw_color(filled ? WHITE : BLUE);
          mylcd.Fill_Rectangle(sx, sy, sx + cellSize - 1, sy + cellSize - 1);

          prevGrid.set(y, x, filled);
        }
      }
    }

    prevValid = true;
  }

  void gameOver() {
    
  }

  void buzz() {
    
  }
};



//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)
class GameOutputSerial {
private:
  //Buzzer output pin
  static constexpr int8_t buzzerPin = -1;
  //Temporary screen management code (mmmm thats eating a lot of ram)
  char buffer[gameGrid.height * (gameGrid.width + 1) + 1];  //20 lines * (10 blocks + 1 '\n') + '\0'

public:
  //GameOutputSerial constructor
  GameOutputSerial() {
    //Init buzzer
    pinMode(buzzerPin, OUTPUT);
  }

  //Function to update the screen (using buffer atm)
  void updateScreen() {
    //Loop through game screen, keeping track of index
    uint16_t i = 0;
    for (uint8_t y = 0; y < gameGrid.height; y++) {
      for (uint8_t x = 0; x < gameGrid.width; x++) {
        if (gameGrid.get(y, x)) {
          buffer[i] = '#';
        } else {
          buffer[i] = '.';
        }
        i++;
      }
      buffer[i] = '\n';
      i++;
    }
    buffer[i] = '\0';

    Serial.println(buffer);
  }

  //Screen to play when game is over
  void gameOver() {
    Serial.println("Game Over D:");
    delay(5000);
  }

  //Make a buzz noise
  void buzz() {
    tone(buzzerPin, 500);
    delay(500);  //TODO: is blocking atm, may need to change
    noTone(buzzerPin);
  }
};