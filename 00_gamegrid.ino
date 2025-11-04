//GameGrid class. Basically a wrapper over an array of bit-packed ints, saving about 175 bytes compared to a 2d bool array
class GameGrid {
  public:
    static constexpr uint8_t height = 20;
    static constexpr uint8_t width = 10;

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
        return false;
      }
      //Bitwise math to find the position in the bitGrid
      const uint16_t i = y * width + x;
      const uint8_t bitmask = 1 << (i % 8);
      //Set bit /w or
      if (val) bitGrid[i >> 3] |= bitmask;
      //Clear bit /w and not
      else bitGrid[i >> 3] &= ~bitmask;
    }
};

//Create an instance of the class. Doing that here atm to be used globally
//TODO: Change other classes to use a pointer to gameGrid?
GameGrid gameGrid;