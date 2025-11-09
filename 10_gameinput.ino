//GameInput class, handles button inputs (will need to update pins but otherwise should be the same)
class GameInput {
private:
  //Button input pins
  static constexpr int8_t leftPin = 4, rightPin = 3, downPin = -1, rotatePin = -1;

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
    //TODO: remove this when below is implemented
    return false;

    if (digitalRead(downPin) == HIGH) {
      activePiece.fall();  //TODO: Check if this works
      return true;
    }
    if (digitalRead(rotatePin) == HIGH) {
      //TODO: handle rotate input
    }

    return false;
  }
};