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
  
  int16_t displayWidth, displayHeight, effectiveWidth, effectiveHeight;
  int16_t cellSize;
  int16_t playWidth, playHeight, playX, playY;

  GameGrid prevGrid;
  bool prevValid = false;

  void mapCell(int16_t x, int16_t y, int16_t playX, int16_t playY, int16_t cellSize, int16_t &screenX, int16_t &screenY) {
    switch(displayRotation) {
      case ROTATION_0:
        screenX = playX + x * cellSize;
        screenY = playY + y * cellSize;
        break;
      case ROTATION_90:
        screenX = playX + y * cellSize;
        screenY = playY + (gameGrid.width - 1 - x) * cellSize;
        break;
      case ROTATION_180:
        screenX = playX + (gameGrid.width - 1 - x) * cellSize;
        screenY = playY + (gameGrid.height - 1 - y) * cellSize;
        break;
      case ROTATION_270:
        screenX = playX + (gameGrid.height - 1 - y) * cellSize;
        screenY = playY + x * cellSize;
        break;
    }
  }

  void drawPlayfieldBackground() {
    mylcd.Set_Draw_color(BLUE);
    mylcd.Fill_Rectangle(playX, playY, playX + playWidth - 1, playY + playHeight - 1);
  }

public:
  bool screenInitialized = false;

  GameOutput() {
    //nothing atm
  }

  void init() {
    Serial.println("Initializing GameOutput");
    mylcd.Init_LCD();

    //Moved math here to init since it doesn't change
    displayWidth = mylcd.Get_Display_Width();
    displayHeight = mylcd.Get_Display_Height();

    if (displayRotation == ROTATION_0 || displayRotation == ROTATION_180)
      effectiveWidth = gameGrid.width, effectiveHeight = gameGrid.height;
    else
      effectiveWidth = gameGrid.height, effectiveHeight = gameGrid.width;

    cellSize = min(displayWidth / effectiveWidth, displayHeight / effectiveHeight);

    playWidth = cellSize * effectiveWidth;
    playHeight = cellSize * effectiveHeight;

    playX = (displayWidth - playWidth) / 2;
    playY = (displayHeight - playHeight) / 2;

    delay(200);
    // mylcd.Fill_Screen(BLACK);
    // drawPlayfieldBackground();
    prevValid = false;
  }

  void updateScreen() {
    for (uint8_t y = 0; y < gameGrid.height; y++) {
      for (uint8_t x = 0; x < gameGrid.width; x++) {
        bool filled = gameGrid.get(y, x);

        if (!prevValid || prevGrid.get(y, x) != filled) {
          int16_t screenX, screenY;

          mapCell(x, y, playX, playY, cellSize, screenX, screenY);

          mylcd.Set_Draw_color(filled ? WHITE : BLUE);
          mylcd.Fill_Rectangle(screenX, screenY, screenX + cellSize - 1, screenY + cellSize - 1);

          prevGrid.set(y, x, filled);
        }
      }
    }

    prevValid = true;
    screenInitialized = true;
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