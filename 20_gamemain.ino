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
}


//Used to manage time in the game loop (TODO: maybe move these to respective class later, idk)
//Time in ms between ticks
constexpr uint16_t tickTime = 500, inputTime = 500;
uint16_t lastTick = 0, lastInput = 0;

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
  if (activePiece.settled) {
    //Create a new piece
    Serial.println("New piece");
    activePiece.reset();
    gameOutput.buzz();
  }

  if (screenChanged) gameOutput.updateScreen();
}