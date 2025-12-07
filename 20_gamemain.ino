//Global vars
Piece activePiece;
GameInput gameInput;
GameOutput gameOutput;

//Used to manage timed events in the game loop
constexpr uint16_t fallTime = 500, inputTime = 150, buzzTime = 500, frameRate = 20;
uint32_t lastFall, lastInput, lastBuzz, lastFrame;
bool isBuzzing = false;

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
  lastBuzz = millis();
  lastFrame = millis();
}

//Main game loop
void loop() {
  //Stop main loop if gameOver
  if(gameOver) {
    gameOutput.gameOver();
    return;
  }

  //Check if piece has hit the bottom grid
  if (activePiece.settled) {
    //Create a new piece, game over if no space
    if (!activePiece.reset()) {
      gameOver = true;
      return;
    }
    //Check for line clear, call buzz on succeess
    if(gameGrid.lineClear()) {
      gameOutput.startBuzz();
      lastBuzz = millis();
      isBuzzing = true;
    }
  }

  //Current time
  uint32_t now = millis();

  //Stops buzzer when done
  if (isBuzzing && now - lastBuzz >= buzzTime) {    
    gameOutput.endBuzz();
    isBuzzing = false;
  }

  //Get user input
  if (now - lastInput >= inputTime && gameInput.readInput(activePiece)) {
    lastInput = now;
  }

  //Make active game piece fall
  if (now - lastFall >= fallTime) {
    activePiece.fall();
    lastFall = now;
  }

  //Update screen
  if (now - lastFrame >= (1000 / frameRate)) {
    gameOutput.updateScreen();
    lastFrame = now;
  }
}