//Game grid info
constexpr uint8_t gameHeight = 20, gameWidth = 10; //TODO: Should be height 40 for the real game
bool gameGrid[gameHeight][gameWidth] = {0};

/* EOF? */

//Enum for piece types
enum PieceType {
  L,
  T,
  NUM_PIECES
};

//Defines the piece designs
constexpr uint8_t pieceDesign[NUM_PIECES][3][3] = {
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

    //Gravity to make the piece fall (natural or when pressing down)
    void fall() {
      //If at the bottom of the screen, set it to settled
      if (settled || y + height == gameHeight - 1) {
        settled = true;
        return;
      }
      //If there is a block below anywhere, set to settled
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

    void moveLeft() {
      //If at the edge of the screen, don't move
      if (x - 1 == 0) return;
      
      //TODO: Need code for collision check for blocks on the sides

      //Otherwise move
      erase();
      x--;
      print();
    }

    void moveRight() {
      //If at the edge of the screen, don't move
      if (x + 1 == gameWidth - 1) return;
      
      //TODO: Need code for collision check for blocks on the sides

      //Otherwise move
      erase();
      x++;
      print();
    }

    //Returns settled state
    bool isSettled() const {
      return settled;
    }
};

/* EOF */

//GameInput class, handles button inputs (will need to update pins but otherwise should be the same)
class GameInput {
  private:
    //Button input pins
    static constexpr uint8_t leftPin = 12, downPin = 11, rightPin = 10, rotatePin = 9;

  public:
    //GameInput constructor, sets pin modes
    GameInput() {
      //Sets pinModes
      pinMode(leftPin, INPUT);
      pinMode(downPin, INPUT);
      pinMode(rightPin, INPUT);
      pinMode(rotatePin, INPUT);
    }

    bool readInput( Piece &activePiece) {
      if(digitalRead(leftPin) == HIGH) {
        activePiece.moveLeft();
        return true;
      }
      if(digitalRead(downPin) == HIGH) {
        activePiece.fall();
        return true;
      }
      if(digitalRead(rightPin) == HIGH) {
        activePiece.moveRight();
        return true;
      }
      if(digitalRead(rotatePin) == HIGH) {
        //TODO: handle rotate input
      }
      
      return false;
    }
};

//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)
class GameOutput {
  private:
    //Temporary screen management code (mmmm thats eating a lot of ram)
    char buffer[gameHeight * (gameWidth+1) + 1]; //40 lines * (10 blocks + 1 '\n') + '\0'

  public:
    //GameOutput constructor
    GameOutput() {
      //Nothing atm
    }

    //Function to update the screen (using buffer atm)
    void updateScreen() {
      //Loop through game screen, keeping track of index
      uint16_t i = 0;
      for(uint8_t y = 0; y < gameHeight; y++) {
        for(uint8_t x = 0; x < gameWidth; x++) {
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

/* EOF */

//Global vars
Piece activePiece;
GameInput gameInput;
GameOutput gameOutput;
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
  gameInput = GameInput();
  gameOutput = GameOutput();
}


//Used to manage time in the game loop (TODO: maybe move these to respective class later, idk)
//Time in ms between ticks
constexpr uint16_t tickTime = 200;
uint16_t lastTick = 0;
constexpr uint16_t inputTime = 100;
uint16_t lastInput = 0;

//Main game loop
void loop() {
  bool screenChanged = false;

  //Get user input (has a cooldown of inputTime)
  if (millis() - lastInput >= inputTime && gameInput.readInput(activePiece)) {
    screenChanged = true;
    lastInput = millis();
  }

  //Make active game piece fall + update screen (cooldown of tickTime)
  if (millis() - lastTick >= tickTime) {
    activePiece.fall();
    lastTick = millis();
  }

  //Check if piece has hit the bottom grid
  if (activePiece.isSettled()) {
    //Create a new piece
    Serial.println("New piece");
    activePiece = Piece();
  }

  if(screenChanged) gameOutput.updateScreen();
}

//TODO: Implement scoring, finish piece movement code