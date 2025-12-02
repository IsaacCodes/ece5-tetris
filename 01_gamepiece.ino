//Enum for piece types
enum PieceType : uint8_t {
  L,
  J,
  T,
  O,
  S,
  Z,
  I,
  NUM_PIECES
};

enum PieceRotation : uint8_t {
  ROT_0,
  ROT_90,
  ROT_180,
  ROT_270,
  NUM_ROTATIONS
};

//Defines the piece designs
constexpr uint8_t pieceDesignWidth = 4;
constexpr uint8_t pieceDesignHeight = 4;
constexpr uint8_t pieceDesign[NUM_PIECES][NUM_ROTATIONS][pieceDesignHeight][pieceDesignWidth] PROGMEM = {
  //L
  {
    {
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 1, 0 },
      { 1, 0, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 1, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //J
  {
    {
      { 0, 0, 1, 0 },
      { 0, 0, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 0, 0, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 0, 0 },
      { 1, 0, 0, 0 },
      { 1, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 1, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //T
  {
    {
      { 0, 1, 0, 0 },
      { 1, 1, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 1, 1, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //O
  {
    {
      { 1, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //S
  {
    {
      { 0, 1, 1, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 1, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 0, 1, 1, 0 },
      { 1, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 1, 0, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //Z
  {
    {
      { 1, 1, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 1, 0 },
      { 0, 1, 1, 0 },
      { 0, 1, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 1, 1, 0, 0 },
      { 0, 1, 1, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 1, 1, 0, 0 },
      { 1, 0, 0, 0 },
      { 0, 0, 0, 0 }
    }
  },
  //I
  {
    {
      { 0, 0, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 }
    },
    {
      { 0, 0, 0, 0 },
      { 1, 1, 1, 1 },
      { 0, 0, 0, 0 },
      { 0, 0, 0, 0 }
    },
    {
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 },
      { 0, 1, 0, 0 }
    }
  }
};


//Piece class to manage active piece movement
class Piece {
public:
  bool settled;
private:
  //x, y relative to top left
  uint8_t x, y;
  PieceRotation rotation;
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
    //Sets a random piece type + rotation
    type = static_cast<PieceType>(random(NUM_PIECES));
    rotation = static_cast<PieceRotation>(random(NUM_ROTATIONS));

    Serial.println("New Piece?");
    return !willCollide(0, 0);
  }

  //Empties the section of the gameGrid a piece occupies
  void erase() const {
    for (int8_t i = 0; i < pieceDesignHeight; i++) {
      for (int8_t j = 0; j < pieceDesignWidth; j++) {
        if (pgm_read_byte(&pieceDesign[type][rotation][i][j])) {
          gameGrid.set(y + i, x + j, 0);
        }
      }
    }
  }

  //Print to the gameGrid, which is then handled by gameIO
  void print() const {
    for (int8_t i = 0; i < pieceDesignHeight; i++) {
      for (int8_t j = 0; j < pieceDesignWidth; j++) {
        if (pgm_read_byte(&pieceDesign[type][rotation][i][j])) {
          gameGrid.set(y + i, x + j, 1);
        }
      }
    }
  }

  //Returns whether will collide with new location
  bool willCollide(int8_t dy, int8_t dx) {
    //Loops thru each position of the block
    for (int8_t i = 0; i < pieceDesignHeight; i++) {
      for (int8_t j = 0; j < pieceDesignWidth; j++) {
        //If position is out of range or occupied, return true
        int8_t ny = y + i + dy, nx = x + j + dx;
        if (pgm_read_byte(&pieceDesign[type][rotation][i][j]) && (ny >= gameGrid.height || nx < 0 || nx >= gameGrid.width || gameGrid.get(ny, nx))) return true;
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

  //Movement directions
  void moveLeft() {
    //Erase the current position
    erase();
    //If spot is open, go left
    if (!willCollide(0, -1)) x--;
    //Then reprint
    print();
    Serial.println("left");
  }

  void moveRight() {
    //Erase the current position
    erase();
    //If spot is open, go left
    if (!willCollide(0, 1)) x++;
    //Then reprint
    print();
    Serial.println("right");
  }

  void moveDown() {
    fall();
    gameGrid.score++;
    Serial.println("down");
  }

  void rotate() {
    //Erase the current position
    erase();
    //Try rotation
    rotation = static_cast<PieceRotation>((rotation + 1) % NUM_ROTATIONS);
    //Rotate back if spot is full
    if (willCollide(0, 0))
      rotation = static_cast<PieceRotation>((rotation + NUM_ROTATIONS - 1) % NUM_ROTATIONS);
    //Then reprint
    print();
    Serial.println("rotate");
  }
};