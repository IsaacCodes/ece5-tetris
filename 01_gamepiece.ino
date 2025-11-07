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
public:
  bool settled;
private:
  //x, y relative to top left
  uint8_t x, y;
  PieceType type;

public:
  //Piece constructor
  Piece() {
    //Calls the reset function to set all variables to defaults
    reset();
  }

  //Returns whether it can reset (!willCollide)
  bool reset() {
    //Always start at top center
    x = 3;
    y = 0;
    //Whether the piece should stop falling
    settled = false;
    //Sets a random piece type
    type = static_cast<PieceType>(random(NUM_PIECES));

    return !willCollide(0, 0);
  }

  //Empties the section of the gameGrid a piece occupies
  void erase() const {
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        if (pieceDesign[type][i][j] == 1) {
          gameGrid.set(y + i, x + j, 0);
        }
      }
    }
  }

  //Print to the gameGrid, which is then handled by gameIO
  void print() const {
    for (int8_t i = 0; i < 3; i++) {
      for (int8_t j = 0; j < 3; j++) {
        if (pieceDesign[type][i][j] == 1) {
          gameGrid.set(y + i, x + j, 1);
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
        if (pieceDesign[type][i][j] == 1 && (ny >= gameGrid.height || nx < 0 || nx >= gameGrid.width || gameGrid.get(ny, nx))) return true;
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
};