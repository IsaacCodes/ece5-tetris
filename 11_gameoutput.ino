//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)
class GameOutputSerial {
private:
  //Buzzer output pin
  static constexpr int8_t buzzerPin = 2;
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