//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)

#include "LCDWIKI_SPI.h"
#include "LCDWIKI_GUI.h"

#define MODEL ST7796S
#define CS 10    
#define CD 9
#define RST 8
#define MOSI 11
#define MISO 12
#define SCK 13
#define LED -1 // we set this to -1, because the LED is always on.

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

LCDWIKI_SPI mylcd(MODEL,CS,CD,MISO,MOSI,RST,SCK,LED);

#define ROTATION_0   0
#define ROTATION_90  1
#define ROTATION_180 2
#define ROTATION_270 3

uint8_t displayRotation = ROTATION_180;

class GameOutput {
private:
  void mapCell(int gx, int gy, int playX, int playY, int cellSize, int &outX, int &outY) {
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
    int displayW = mylcd.Get_Display_Width();
    int displayH = mylcd.Get_Display_Height();

    int effW, effH;
    if (displayRotation == ROTATION_0 || displayRotation == ROTATION_180) {
      effW = gameGrid.width;
      effH = gameGrid.height;
    } else {
      effW = gameGrid.height;
      effH = gameGrid.width;
    }

    int cellSize = min(displayW / effW, displayH / effH);
    int playW = cellSize * effW;
    int playH = cellSize * effH;

    int playX = (displayW - playW) / 2;
    int playY = (displayH - playH) / 2;

    mylcd.Set_Draw_color(BLUE);
    mylcd.Fill_Rectangle(playX, playY, playX + playW - 1, playY + playH - 1);
  }

public:
  bool prevGrid[gameGrid.height][gameGrid.width];
  bool prevValid = false;

  GameOutput() {
    //nothing atm
  }

  void init() {
    Serial.println("Initiated");
    mylcd.Init_LCD();
    delay(200);
    mylcd.Fill_Screen(BLACK);
    drawPlayfieldBackground();
    prevValid = false;
  }

  void updateScreen() {
    int displayW = mylcd.Get_Display_Width();
    int displayH = mylcd.Get_Display_Height();

    int effW, effH;
    if (displayRotation == ROTATION_0 || displayRotation == ROTATION_180) {
      effW = gameGrid.width;
      effH = gameGrid.height;
    } else {
      effW = gameGrid.height;
      effH = gameGrid.width;
    }

    int cellSize = min(displayW / effW, displayH / effH);

    int playW = cellSize * effW;
    int playH = cellSize * effH;

    int playX = (displayW - playW) / 2;
    int playY = (displayH - playH) / 2;

    for (uint8_t y = 0; y < gameGrid.height; y++) {
      for (uint8_t x = 0; x < gameGrid.width; x++) {
        bool filled = gameGrid.get(y, x);

        if (!prevValid || prevGrid[y][x] != filled) {
          int sx;
          int sy;

          mapCell(x, y, playX, playY, cellSize, sx, sy);

          mylcd.Set_Draw_color(filled ? WHITE : BLUE);
          mylcd.Fill_Rectangle(sx, sy, sx + cellSize - 1, sy + cellSize - 1);

          prevGrid[y][x] = filled;
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