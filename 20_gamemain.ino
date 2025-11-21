//Global vars
Piece activePiece;
GameInput gameInput;
GameOutput gameOutput;

//TODO: Fix main timings to be a little less janky
//Used to manage time in the game loop (TODO: maybe move these to respective class later, idk)
//Time in ms between ticks
constexpr uint16_t fallTime = 500;
constexpr uint16_t inputTime = 150;
constexpr uint16_t frameRate = 20;

uint32_t lastFall, lastInput, lastFrame; // FIX: made these 32 bit to match the millis() value being 32 bit; no more speeding up !!!
bool gameOver = false;

//Runs once on startup
void setup() {
  //Init Serial
  Serial.begin(500000);
  Serial.println("\n--------------------\n");
  //Init random
  randomSeed(analogRead(A0));
  gameOutput.init();
  
  lastFall = millis();
  lastInput = millis();
}

//Main game loop
void loop() {
  if(gameOver) {
    gameOutput.gameOver();
    return;
  }
  //Get user input (has a cooldown of inputTime)
  //Make active game piece fall + update screen (cooldown of tickTime)

  uint32_t now = millis();

  if (now - lastFrame >= (1000 / frameRate)) {
    gameOutput.updateScreen();
    lastFrame = now;
  }

  if (now - lastFall >= fallTime) {
    activePiece.fall();
    lastFall = now;
  }

  if (now - lastInput >= inputTime) {
    if (gameInput.readInput(activePiece)) {
      lastInput = now;
    }
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
}