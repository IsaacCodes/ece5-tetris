//Game grid info
const uint8_t game_grid_height = 20; //Should be 40 for the real game
const uint8_t game_grid_width = 10;
int game_grid[game_grid_height][game_grid_width] = {0};

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
class GamePiece {
  private:
    //Piece type + pos
    PieceType type;
    uint8_t x, y;

  public:
    //Constructor, pawns piece at center
    GamePiece() {
      type = static_cast<PieceType>(random(NUM_PIECES));
      x = 3; y = 0;
    }

    //Empties the section of the game_grid a piece occupies
    void zeroPiece() {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          game_grid[y + i][x + j] = 0;
        }
      }
    }

    //Temp print to Serial
    void printPiece() {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          game_grid[y + i][x + j] = pieceDesign[type][i][j];
        }
      }
    }

    //Simple gravity, can add collision later
    void fall() {
      if (y < game_grid_height - 3) { //Minus 3 since it is of size 3, need to make this much better in the future
        zeroPiece();
        y++;
        printPiece();
      }
    }
};



//Temporary screen management code (mmmm thats eating a lot of ram)
char game_screen[game_grid_height * (game_grid_width+1) + 1]; //40 lines * (10 blocks + 1 '\n') + '\0'
void update_screen() {
  uint16_t game_screen_i = 0;
  for(uint8_t y = 0; y < game_grid_height; y++) {
    for(uint8_t x = 0; x < game_grid_width; x++) {
      game_screen[game_screen_i] = '0' + game_grid[y][x]; //Convert it to a char and put it on the screen string
      game_screen_i++;
    }
    game_screen[game_screen_i] = '\n';
    game_screen_i++;
  }
  game_screen[game_screen_i] = '\0';

  Serial.println(game_screen);
}


//Init
GamePiece gp0;
void setup() {
  Serial.begin(115200);
  Serial.println("\n--------------------\n");
  gp0 = GamePiece();
}


//Used to manage time in the game loop
//Time in ms between ticks
const uint16_t tick_time = 1000;
uint16_t last_tick = 0;

//Main game loop
void loop() {

  //Every tick_time ms, fall + update the screen
  if (millis() - last_tick >= tick_time) {
    //Make pieces fall + update screen
    gp0.fall();
    update_screen();

    last_tick = millis();

  }
}