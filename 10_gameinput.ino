//GameInput class, handles button inputs (will need to update pins but otherwise should be the same)
class GameInput {
private:
  //Button input pins
  static constexpr int8_t leftPin = 4, rightPin = 3, downPin = 6, rotatePin = 7;

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

    if (digitalRead(rotatePin) == HIGH) {
      activePiece.rotate();
      Serial.println("rotate");
      return true;
    }

    //TODO: Change this to be a proper implementation?
    if (digitalRead(downPin) == HIGH) {
      activePiece.fall();
      Serial.println("down");
      return true;
    }

    return false;
  }
};