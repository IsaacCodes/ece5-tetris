//Global vars
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
  gameOutput.init();
}


//Used to manage time in the game loop (TODO: maybe move these to respective class later, idk)
//Time in ms between ticks
constexpr uint16_t tickTime = 500, inputTime = 150;
uint16_t lastTick = 0, lastInput = 0;
bool gameOver = false;

//Main game loop
void loop() {
  if(gameOver) {
    gameOutput.gameOver();
    return;
  }

  bool screenChanged = false;

  //Get user input (has a cooldown of inputTime)
  if ((millis() - lastInput >= inputTime) && gameInput.readInput(activePiece)) {
    screenChanged = true;
    lastInput = millis();
  }

  //Make active game piece fall + update screen (cooldown of tickTime)
  unsigned long now = millis();
  unsigned long dtTick = now - lastTick;

  while (dtTick >= tickTime) {
      screenChanged = true;
      activePiece.fall();
      lastTick += tickTime;
      dtTick -= tickTime;
  }

  //Check if piece has hit the bottom grid
  if (activePiece.settled) {
    //Create a new piece
    Serial.println("New piece");
    if (!activePiece.reset()) {
      gameOver = true;
      return;
    }
    if(gameGrid.lineClear()) {
      Serial.println("CLEAR");
      gameOutput.buzz();
    }
  }

  if (screenChanged) gameOutput.updateScreen();
}