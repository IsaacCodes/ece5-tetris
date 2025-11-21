//Global vars
Piece activePiece;
GameInput gameInput;
GameOutput gameOutput;

//Used to manage time in the game loop
constexpr uint16_t fallTime = 500, inputTime = 150, frameRate = 20;
uint32_t lastFall, lastInput, lastFrame;
//Quit game loop early when true
bool gameOver = false;

//Runs once on startup
void setup() {
  //Init Serial
  Serial.begin(115200);
  Serial.println("\n--------------------\n");
  //Init random
  randomSeed(analogRead(A0));
  gameOutput.init();
  
  lastFall = millis();
  lastInput = millis();
  lastFrame = millis();
}

//Main game loop
void loop() {
  if(gameOver) {
    gameOutput.gameOver();
    return;
  }

  uint32_t now = millis();

  //Update screen
  if (now - lastFrame >= (1000 / frameRate)) {
    gameOutput.updateScreen();
    lastFrame = now;
  }

  //Make active game piece fall
  if (now - lastFall >= fallTime) {
    activePiece.fall();
    lastFall = now;
  }

  //Get user input
  if (now - lastInput >= inputTime && gameInput.readInput(activePiece)) {
    lastInput = now;
  }

  //Check if piece has hit the bottom grid
  if (activePiece.settled) {
    //Create a new piece, game over if no space
    if (!activePiece.reset()) {
      gameOver = true;
      return;
    }
    //Clear the line, call buzz
    if(gameGrid.lineClear()) {
      gameOutput.buzz();
    }
  }
}