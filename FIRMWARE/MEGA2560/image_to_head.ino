#include <Arduino.h>

void printLayer() {
  int fails = 0;
  int steps = 0;
  xaar128.powerUp();
  xServo.setSpeed(200);
  positions[2] = -printStartPos;
  xPos = -printStartPos;
  xServo.runToNewPosition(positions[2]);

  for (int a = 0; a < 128; a++) {
    nCount[a] = nozzleRLE[a][0];
  }
  for (int x = 0; x < 735; x++) {
    hasData = false;
    for (int ca = 0; ca < 8; ca++) {
      for (int cb = 0; cb < 8; cb++) {
        int n = ca * 8 + cb;
        if (nCount[n] == 0) {
          ncPos[n]++;
          nCount[n] = nozzleRLE[n][ncPos[n]] - 1;

          if (x > 0) {

            nComb1[ca] ^= 1 << cb;


          }
        } else if (nCount[n] > 0) {
          nCount[n]--;
          hasData = true;
        }

        int n2 = n + 64;
        if (nCount[n2] == 0) {
          ncPos[n2]++;
          nCount[n2] = nozzleRLE[n2][ncPos[n2]] - 1;

          if (x > 0) {

            nComb2[ca] ^= 1 << cb;


          }
        } else if (nCount[n2] > 0) {
          nCount[n2]--;
          hasData = true;

        }
      }
    }
    if (!hasData) {
      break;
    }


    ///////DATA TO HEAD/////
    for (int r = 0; r < 4; r++) {
      xaar128.loadData(nComb1, nComb2);
      delayMicroseconds(1000);
      xaar128.fire();
    }
    xPos = xPos - 9;
    positions[2] =  (int)roundf(xPos);


    xServo.runToNewPosition(positions[2]);

  }
  delay(100);
  xaar128.powerDown();
  positions[2] = 0;
  xServo.runToNewPosition(positions[2]);
  memset(nComb1, 0, 8);
  memset(nComb2, 0, 8);
  memset(nCount, 0, 128);
  memset(ncPos, 0, 128);
  Serial.println('A');
}
