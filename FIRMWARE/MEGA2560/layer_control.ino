#include <Arduino.h>

void newLayer() {
  digitalWrite(6, 0);
  digitalWrite(26, 1);
  delay(500);
  positions[0] += 20;
  positions[1] += 40;
  Serial.println(positions[0]);
  Serial.println(positions[1]);
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  digitalWrite(6, 1);
  digitalWrite(26, 0);
  digitalWrite(3, 1);
  delay(100);
  xServo.setSpeed(1000);
  xServo.runToNewPosition(-15500);
  delay(100);
  xServo.runToNewPosition(0);
  digitalWrite(3, 0);

  for (byte n = 0; n < 128; n++) {

    for (int p = 0; p < 20; p++) {
      nozzleRLE[n][p] = 0;



    }
  }
  Serial.println('A');
}
void fillBuild() {
  digitalWrite(6, 0);
  digitalWrite(26, 1);
  delay(500);
  positions[1] += 50;
  Serial.println(positions[1]);
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
  digitalWrite(6, 1);
  digitalWrite(26, 0);
  digitalWrite(3, 1);
  delay(100);
  xServo.setSpeed(1000);
  xServo.runToNewPosition(-15500);
  delay(100);
  xServo.runToNewPosition(0);
  digitalWrite(3, 0);

  for (byte n = 0; n < 128; n++) {

    for (int p = 0; p < 20; p++) {
      nozzleRLE[n][p] = 0;



    }
  }
  Serial.println('A');
}
