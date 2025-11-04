//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)
class GameOutput {
private:
  //Buzzer output pin
  static constexpr int8_t buzzerPin = 2;
  //Temporary screen management code (mmmm thats eating a lot of ram)
  char buffer[gameHeight * (gameWidth + 1) + 1];  //40 lines * (10 blocks + 1 '\n') + '\0'

public:
  //GameOutput constructor
  GameOutput() {
    //Init buzzer
    pinMode(buzzerPin, OUTPUT);
  }

  //Function to update the screen (using buffer atm)
  void updateScreen() {
    //Loop through game screen, keeping track of index
    uint16_t i = 0;
    for (uint8_t y = 0; y < gameHeight; y++) {
      for (uint8_t x = 0; x < gameWidth; x++) {
        if (gameGrid[y][x]) {
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

  //Make a buzz noise
  void buzz() {
    tone(buzzerPin, 500);
    delay(500); //TODO: is blocking atm, may need to change
    noTone(buzzerPin);
  }
};