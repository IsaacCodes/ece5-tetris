//Game grid info, modified by the active Piece and read by the GameIO
const uint8_t gameGridHeight = 20; //TODO: Should be 40 for the real game
const uint8_t gameGridWidth = 10;
int gameGrid[gameGridHeight][gameGridWidth] = {0};

//Enum for piece types
enum PieceType {
  L,
  T,
  NUM_PIECES
};

//Defines the piece designs
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

//Piece class to manage active piece movement
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
      //Always start at top center
      x = 3; y = 0;
      //Whether the piece should stop falling
      settled = false;
      //Sets a random piece type
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
      //Nothing atm. TODO: delete if nothing on final product
      return;
    }

    //Empties the section of the game_grid a piece occupies
    void erase() const {
      for (int i = 0; i <= height; i++) { // < height to prevent erasing lower blocks
        for (int j = 0; j < 3; j++) {
          gameGrid[y + i][x + j] = 0;
        }
      }
    }

    //Print to the gameGrid, which is then handlded by IO
    void print() const {
      for (int i = 0; i <= height; i++) {
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
      //Checks if there is a block below anywhere
      for (int j = 0; j < 3; j++) {
        if (gameGrid[y + height + 1][x + j] == 1) {
          settled = true;
          return;
        }
      }

      //Otherwise fall
      erase();
      y++;
      print();
    }

    //Returns settled state
    bool isSettled() const {
      return settled;
    }
};


//GameIO class, mostly temporary atm (will need to be updated with real screen and input functions)
class GameIO {
  private:
    //Temporary screen management code (mmmm thats eating a lot of ram)
    char buffer[gameGridHeight * (gameGridWidth+1) + 1]; //40 lines * (10 blocks + 1 '\n') + '\0'

  public:
    //GameIO constructor
    GameIO() {
      //Nothing atm
    }

    //Function to update the screen (using buffer atm)
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


//Global vars
Piece activePiece;
GameIO gameIO;
//Runs once on startup
void setup() {
  //Init Serial
  Serial.begin(115200);
  Serial.println("\n--------------------\n");
  //Init random
  //NOTE: If we ever connect something to A0, this might need to be changed
  randomSeed(analogRead(A0));
  //Init classes
  activePiece = Piece();
  gameIO = GameIO();
}


//Used to manage time in the game loop
//Time in ms between ticks
const uint16_t tickTime = 200;
uint16_t lastTick = 0;

//Main game loop
void loop() {
  //Every tick_time ms, fall + update the screen
  if (millis() - lastTick >= tickTime) {
    //Make active game piece fall + update screen
    activePiece.fall();
    gameIO.updateScreen();

    lastTick = millis();
  }

  if (activePiece.isSettled()) {
    //Create a new piece
    Serial.println("New piece");
    activePiece = Piece();
  }

  //TODO: Need code for when a new block must spawn (after old block is settled)
}