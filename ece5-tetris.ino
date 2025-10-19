//Game grid info
const uint8_t gameGridHeight = 20; //TODO: Should be 40 for the real game
const uint8_t gameGridWidth = 10;
int gameGrid[gameGridHeight][gameGridWidth] = {0};

//Enum for piece types
enum PieceType {
  L,
  T,
  NUM_PIECES
};

// Define the static piece designs
const uint8_t pieceDesign[NUM_PIECES][3][3] = {
  //L
  {
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 0}
  },
  //T
  {
    {0, 1, 0},
    {1, 1, 1},
    {0, 0, 0}
  }
};

//GamePiece class
class Piece {
  private:
    //Piece type + pos
    PieceType type;
    //x, y relative to top left
    uint8_t x, y, height;
    bool settled;

  public:
    //Piece constructor
    Piece() {
      type = static_cast<PieceType>(random(NUM_PIECES));

      //Finds the bottom of the piece (used by fall logic)
      for (int i = 2; i >= 0; i--) {
        for (int j = 0; j < 3; j++) {
          if (pieceDesign[type][i][j] == 1) {
            height = y + i;
            goto breakLoops; //break from both loops
          }
        }
      } 
      //Used by goto
      breakLoops:
      //Always start at top center
      x = 3; y = 0;
      //Whether the piece should stop falling
      settled = false;
    }

    //Empties the section of the game_grid a piece occupies
    void erase() {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          gameGrid[y + i][x + j] = 0;
        }
      }
    }

    //Temp print to Serial
    void print() {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          gameGrid[y + i][x + j] = pieceDesign[type][i][j];
        }
      }
    }

    //Simple gravity, can add collision later
    void fall() {
      //If at the bottom of the screen, set it to settled
      if (y + height == gameGridHeight - 1) {
        settled = true;
        return;
      }
      //Need to write code to set it to settled if there is a block below

      //Otherwise fall
      erase();
      y++;
      print();
    }
};


//GameScreen class, mostly temporary atm (will need to be updated with real screen functions)
class GameScreen {
  private:
    //Temporary screen management code (mmmm thats eating a lot of ram)
    char buffer[gameGridHeight * (gameGridWidth+1) + 1]; //40 lines * (10 blocks + 1 '\n') + '\0'

  public:
    //gameScreen constructor
    GameScreen() {
      //Nothing atm
    }

    void updateScreen() {
      //Loop through game screen, keeping track of index
      uint16_t i = 0;
      for(uint8_t y = 0; y < gameGridHeight; y++) {
        for(uint8_t x = 0; x < gameGridWidth; x++) {
          buffer[i] = '0' + gameGrid[y][x]; //Convert it to a char and put it on the screen string
          i++;
        }
        buffer[i] = '\n';
        i++;
      }
      buffer[i] = '\0';

      Serial.println(buffer);
    }
};


//Init
Piece activePiece;
GameScreen screen;

void setup() {
  Serial.begin(115200);
  Serial.println("\n--------------------\n");
  //Init classes
  activePiece = Piece();
  screen = GameScreen();
}


//Used to manage time in the game loop
//Time in ms between ticks
const uint16_t tickTime = 1000;
uint16_t lastTick = 0;

//Main game loop
void loop() {
  //Every tick_time ms, fall + update the screen
  if (millis() - lastTick >= tickTime) {
    //Make active game piece fall + update screen
    activePiece.fall();
    screen.updateScreen();

    lastTick = millis();
  }
}