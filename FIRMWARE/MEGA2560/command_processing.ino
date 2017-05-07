#include <Arduino.h>

void processCommand(char nData[]) {
  if (nData[0] == 'N') {
    temp[0] = nData[1];
    temp[1] = nData[2];
    temp[2] = nData[3];
    nozzleNum = atoi(temp);
    memset(&temp, 0, sizeof(temp));
    cA = 0;
    cAn = 0;
    for (int i = 5; i < commandSize; i++) {
      if (nData[i] != ',' && nData[i] != ']') {
        temp[cA] = nData[i];
        cA++;
      } else {
        nozzleRLE[nozzleNum][cAn] = atoi(temp);
        cAn++;
        cA = 0;
        memset(&temp[0], 0, sizeof(temp));
      }
    }

  }
  if (nData[0] == 'P') {
    for (int n = 0; n < 128; n++) {
      for (int p = 0; p < 20; p++) {
        Serial.print(nozzleRLE[n][p]);
        Serial.print(':');

      }


      Serial.println(n);
    }
  }
  if (nData[0] == 'C') {
    for (byte n = 0; n < 128; n++) {

      for (int p = 0; p < 20; p++) {
        nozzleRLE[n][p] = 0;


      }
    }
  }

  if (nData[0] == 'S') {
    printLayer();
    for (byte n = 0; n < 128; n++) {

      for (int p = 0; p < 20; p++) {
        nozzleRLE[n][p] = 0;



      }
    }
  }
  if (nData[0] == 'L') {
    newLayer();
  }
  if (nData[0] == 'W') {
    byte w[8] = {255, 255, 255, 255, 255, 255, 255, 255};
    xaar128.powerUp();
    delay(1000);
    for (int r = 0; r < 1000; r++) {
      Serial.println(r);
      xaar128.loadData(w, w);
      delay(1);
      xaar128.fire();
      delay(10);
    }
    xaar128.powerDown();

  }
  if (nData[0] == 'F') {
    fillBuild();
  }
  if (nData[0] == 'A') {
    temp[0] = nData[1];
    temp[1] = nData[2];
    temp[2] = nData[3];
    temp[3] = nData[4];
    temp[4] = nData[5];
    temp[5] = nData[6];
    positions[1] = atoi(temp);
    memset(&temp[0], 0, sizeof(temp));
    Serial.println(positions[1]);
    digitalWrite(6, 0);
    delay(500);
    stepperFeed.runToNewPosition(positions[1]);
    digitalWrite(6, 1);
  }
  if (nData[0] == 'B') {
    temp[0] = nData[1];
    temp[1] = nData[2];
    temp[2] = nData[3];
    temp[3] = nData[4];
    temp[4] = nData[5];
    temp[5] = nData[6];
    positions[0] = atoi(temp);
    memset(&temp[0], 0, sizeof(temp));
    Serial.println(positions[0]);
    digitalWrite(26, 1);
    delay(500);
    stepperBuild.runToNewPosition(positions[0]);
    digitalWrite(26, 0);
  }
  if (nData[0] == 'Y') {
    temp[0] = nData[1];
    temp[1] = nData[2];
    temp[2] = nData[3];
    temp[3] = nData[4];
    temp[4] = nData[5];
    temp[5] = nData[6];
    positions[3] = atoi(temp);
    memset(&temp[0], 0, sizeof(temp));
    Serial.println(positions[3]);

    delay(500);
    stepperY.runToNewPosition(-positions[3]);
    Serial.println('A');
  }



}
