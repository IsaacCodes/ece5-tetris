//Game grid info
constexpr uint8_t gameHeight = 20, gameWidth = 10;
bool gameGrid[gameHeight][gameWidth] = {0};

/*
  PIECE CODE
*/

//Enum for piece types
enum PieceType {
  L,
  T,
  O,
  NUM_PIECES
};

//Defines the piece designs
constexpr uint8_t pieceDesign[NUM_PIECES][3][3] = {
  //L
  {
    { 1, 0, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 } },
  //T
  {
    { 0, 1, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 } },
  //O
  {
    { 1, 1, 0 },
    { 1, 1, 0 },
    { 0, 0, 0 } }
};

//Piece class to manage active piece movement
class Piece {
private:
  //Piece type + pos
  PieceType type;
  //x, y relative to top left
  uint8_t x, y;
  bool settled;

public:
  //Piece constructor
  Piece() {
    //Always start at top center
    x = 3;
    y = 0;
    //Whether the piece should stop falling
    settled = false;
    //Sets a random piece type
    type = static_cast<PieceType>(random(NUM_PIECES));
  }

  //Empties the section of the gameGrid a piece occupies
  void erase() const {
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        if (pieceDesign[type][i][j] == 1) {
          gameGrid[y + i][x + j] = 0;
        }
      }
    }
  }

  //Print to the gameGrid, which is then handled by gameIO
  void print() const {
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        if (pieceDesign[type][i][j] == 1) {
          gameGrid[y + i][x + j] = 1;
        }
      }
    }
  }

  bool willCollide(int8_t dy, int8_t dx) {
    //Loops thru each position of the block
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        //If position is out of range or occupied, return true
        int8_t ny = y + i + dy, nx = x + j + dx;
        if (pieceDesign[type][i][j] == 1 && (ny >= gameHeight || nx < 0 || nx >= gameWidth || gameGrid[ny][nx] == 1)) return true;
      }
    }
    //Otherwise return false
    return false;
  }

  //Gravity to make the piece fall (natural or when pressing down)
  void fall() {
    //Erase the current position
    erase();
    //If spot is open, go down
    if (!willCollide(1, 0)) y++;
    //Otherwise set to settled
    else settled = true;
    //Then reprint
    print();
  }

  void moveLeft() {
    //Erase the current position
    erase();
    //If spot is open, go left
    if (!willCollide(0, -1)) x--;
    //Then reprint
    print();
  }

  void moveRight() {
    //Erase the current position
    erase();
    //If spot is open, go left
    if (!willCollide(0, 1)) x++;
    //Then reprint
    print();
  }

  //Returns settled state
  bool isSettled() const {
    return settled;
  }
};

/*
  GAME IO CODE
*/

//GameInput class, handles button inputs (will need to update pins but otherwise should be the same)
class GameInput {
private:
  //Button input pins
  static constexpr uint8_t leftPin = 4, rightPin = 3, downPin = 11, rotatePin = 9;

public:
  //GameInput constructor, sets pin modes
  GameInput() {
    //Sets pinModes
    pinMode(leftPin, INPUT);
    pinMode(rightPin, INPUT);
    pinMode(downPin, INPUT);
    pinMode(rotatePin, INPUT);
  }

  bool readInput(Piece &activePiece) {
    if (digitalRead(leftPin) == HIGH) {
      activePiece.moveLeft();
      Serial.println("left");
      return true;
    }
    if (digitalRead(rightPin) == HIGH) {
      activePiece.moveRight();
      Serial.println("right");
      return true;
    }

    return false;

    if (digitalRead(downPin) == HIGH) {
      activePiece.fall(); //TODO: Check if this works
      return true;
    }
    if (digitalRead(rotatePin) == HIGH) {
      //TODO: handle rotate input
    }

    return false;
  }
};

//GameOutput class, mostly temporary atm (will need to be updated with real screen functions)
class GameOutput {
private:
  //Temporary screen management code (mmmm thats eating a lot of ram)
  char buffer[gameHeight * (gameWidth + 1) + 1];  //40 lines * (10 blocks + 1 '\n') + '\0'

public:
  //GameOutput constructor
  GameOutput() {
    //Nothing atm
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
};

/*
  SETUP + LOOP CODE
*/

//Global vars
constexpr int8_t buzzerPin = 2;
Piece activePiece;
GameInput gameInput;
GameOutput gameOutput;
//Runs once on startup
void setup() {
  //Init Serial
  Serial.begin(500000);
  Serial.println("\n--------------------\n");
  //Init random
  randomSeed(analogRead(A0));
  //Init buzzer
  pinMode(buzzerPin, OUTPUT);
  //Init classes
  activePiece = Piece();
  gameInput = GameInput();
  gameOutput = GameOutput();
}


//Used to manage time in the game loop (TODO: maybe move these to respective class later, idk)
//Time in ms between ticks
constexpr uint16_t tickTime = 500;
uint16_t lastTick = 0;
constexpr uint16_t inputTime = 500;
uint16_t lastInput = 0;

//Main game loop
void loop() {
  bool screenChanged = false;

  //Get user input (has a cooldown of inputTime)
  if ((millis() - lastInput >= inputTime) && gameInput.readInput(activePiece)) {
    screenChanged = true;
    lastInput = millis();
  }

  //Make active game piece fall + update screen (cooldown of tickTime)
  if (millis() - lastTick >= tickTime) {
    screenChanged = true;
    activePiece.fall();
    lastTick = millis();
  }

  //Check if piece has hit the bottom grid
  if (activePiece.isSettled()) {
    //Create a new piece
    Serial.println("New piece");
    activePiece = Piece();
    tone(buzzerPin, 500);
    delay(500);
    noTone(buzzerPin);
  }

  if (screenChanged) gameOutput.updateScreen();
}