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

//Pin Labels:       MODEL, CS, CD, MISO, MOSI, RST, SCK, LED
LCDWIKI_SPI mylcd(ST7796S, 10,  9,   12,   11,   8,  13,  -1);

//GameOutput class for real screen
class GameOutput {
private:
  //Buzzer output pin
  static constexpr int8_t buzzerPin = 2;
  uint32_t lastBuzz;

  //Info about screen
  uint16_t cellSize, playOffsetX, playOffsetY;
  uint16_t padding = 22;

  bool screenInitialized = false;

  //Info about previous grid
  GameGrid prevGrid;

  //Info about previous score
  int32_t prevScore = -1;

public:
  //Initializes 
  void init() {
    Serial.println("Initializing GameOutput");

    //Init display
    mylcd.Init_LCD();
    mylcd.Set_Rotation(90);

    //Setup display variables 
    uint16_t displayWidth = mylcd.Get_Display_Width(), displayHeight = mylcd.Get_Display_Height();
    uint16_t paddedDisplayWidth = displayWidth - 2 * padding, paddedDisplayHeight = displayHeight - padding;

    uint16_t gridWidth = gameGrid.width, gridHeight = gameGrid.height;

    cellSize = min(paddedDisplayWidth / gridWidth, paddedDisplayHeight / gridHeight);

    uint16_t playWidth = cellSize * gridWidth;
    uint16_t playHeight = cellSize * gridHeight;

    playOffsetX = padding + (paddedDisplayWidth - playWidth) / 2;
    playOffsetY = displayHeight - playHeight;

    //Setup text stuff    
    mylcd.Set_Draw_color(BLACK);
    mylcd.Fill_Rectangle(playOffsetX, 0, playOffsetX + gameGrid.width * cellSize - 1, padding * 2);

    mylcd.Set_Text_colour(WHITE);
    mylcd.Set_Text_Back_colour(BLACK);
    mylcd.Set_Text_Size(3);
    mylcd.Print_String("Score: ", playOffsetX, 0);
  }

  void updateScreen() {
    for (uint8_t y = 0; y < gameGrid.height; y++) {
      for (uint8_t x = 0; x < gameGrid.width; x++) {
        bool filled = gameGrid.get(y, x);
        
        //On first run draw all grid squares, if not only draw changed squares
        if (!screenInitialized || prevGrid.get(y, x) != filled) {
          uint16_t screenX, screenY;

          screenX = playOffsetX + x * cellSize;
          screenY = playOffsetY + y * cellSize;

          mylcd.Set_Draw_color(filled ? WHITE : BLACK);
          mylcd.Fill_Rectangle(screenX, screenY, screenX + cellSize - 1, screenY + cellSize - 1);

          prevGrid.set(y, x, filled);
        }
      }
    }

    //Only update score if score changes
    if(prevScore != gameGrid.score) {
      mylcd.Print_String(String(gameGrid.score), playOffsetX + 125, 0);
      prevScore = gameGrid.score;
    }

    //If updateScreen has finished first run
    if(!screenInitialized) {
      screenInitialized = true;
    }
  }

  //Screen to play when game is over
  void gameOver() {
    //TODO: Write this
  }

  //Make a buzz noise
  void startBuzz() {
    tone(buzzerPin, 500);
  }

  void endBuzz() {
    noTone(buzzerPin);
  }
};



//GameOutputSerial class, for early on simulating game in Serial (may not work with most recent code)
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
  void startBuzz() {
    tone(buzzerPin, 500);
  }

  void endBuzz() {
    noTone(buzzerPin);
  }
};