//GameGrid class. Basically a wrapper over an array of bit-packed ints, saving about 175 bytes compared to a 2d bool array
class GameGrid {
public:
  static constexpr uint8_t height = 20, width = 10;
  uint32_t score = 0;
private:
  uint8_t bitGrid[(height * width + 7) / 8] = {0};

public:
  //GameGrid constructor
  GameGrid() {
    //Nothing atm
  }

  //Get the square status by positon
  bool get(uint8_t y, uint8_t x) {
    //Bound checks for debugging
    if (y >= height || x >= width) {
      Serial.print("Out of bounds get call to "); Serial.print(y); Serial.print(", "); Serial.println(x);
      return false;
    }
    //Bitwise math to find the position in the bitGrid
    const uint16_t i = y * width + x;
    const uint8_t bitmask = 1 << (i % 8);
    return (bitGrid[i >> 3] & bitmask) != 0;
  }

  //Set the square status by positon
  void set(uint8_t y, uint8_t x, bool val) {
    //Bound check for debugging
    if (y >= height || x >= width) {
      Serial.print("Out of bounds set call to "); Serial.print(y); Serial.print(", "); Serial.println(x);
      return;
    }
    //Bitwise math to find the position in the bitGrid
    const uint16_t i = y * width + x;
    const uint8_t bitmask = 1 << (i % 8);
    //Set bit /w or
    if (val) bitGrid[i >> 3] |= bitmask;
    //Clear bit /w and not
    else bitGrid[i >> 3] &= ~bitmask;
  }

  //Clears the line and moves down pieces. Returns true if clear occured
  bool lineClear() {
    uint8_t linesCleared = 0;

    //Loops thru bottom up
    for (int8_t line = height - 1; line >= 0; line--) {
      //Checks whether line is full
      bool full = true;
      for (int8_t j = 0; j < width; j++) {
        if (!get(line, j)) {
          full = false;
          break;
        }
      }

      if (full) {
        linesCleared++;
        //Shift all rows above down
        for (int8_t i = line; i > 0; i--) {
          for (int8_t j = 0; j < width; j++) {
            set(i, j, get(i - 1, j));
          }
        }
        //Clear top row
        for (int8_t j = 0; j < width; j++) {
          set(0, j, 0);
        }
        //Stay on the same line because it now has new data
        line++;
      }
    }

    if (linesCleared >= 1) {
      score += 100 * (1 << (linesCleared - 1));

      Serial.print("Cleared ");
      Serial.print(linesCleared);
      Serial.println(" lines.");
      Serial.print("Score: ");
      Serial.println(score);

      return true;
    }

    return false;
  }
};

//Create an instance of gameGrid, placed here to be used globally
GameGrid gameGrid;