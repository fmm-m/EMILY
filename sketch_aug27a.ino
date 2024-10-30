

#include <QTRSensors.h>;
QTRSensors qtr;

const uint8_t SensorCount = 4;

uint16_t sensorValues[SensorCount];
const int motorOut1 = 4;
const int motorOut2 = 5;
const int motorOut3 = 6;
const int motorOut4 = 7;
const int correctionTime = 5;
uint16_t lastSensors[4] = {0, 0, 0, 0};
bool lastOnBlack = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(motorOut1, OUTPUT);
  pinMode(motorOut2, OUTPUT);
  pinMode(motorOut3, OUTPUT);
  pinMode(motorOut4, OUTPUT);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]) {
    A0, A1, A2, A3
  }, SensorCount);
  Serial.begin(9600);


}

void leftBack() {
  digitalWrite(motorOut3, HIGH);
  digitalWrite(motorOut4, LOW);
}

void leftForward() {
  digitalWrite(motorOut3, LOW);
  digitalWrite(motorOut4 , HIGH);
}

void leftOff() {
  digitalWrite(motorOut3, 0);
  digitalWrite(motorOut4, 0);
}

void rightOff() {
  digitalWrite(motorOut1, 0);
  digitalWrite(motorOut2, 0);
}

void rightForward() {
  digitalWrite(motorOut1, LOW);
  digitalWrite(motorOut2, HIGH);
}

void rightBack() {
  digitalWrite(motorOut1, HIGH);
  digitalWrite(motorOut2, LOW);
}

void turnLeft(int len) {
  rightForward();
  leftBack();
  delay(len);
  rightOff();
  leftOff();
}

void turnRight(int len) {
  rightBack();
  leftForward();
  delay(len);
  rightOff();
  leftOff();
}

void testMotors() {
  delay(250);
  rightForward();
  leftForward();
  delay(250);
  rightOff();
  leftOff();
  delay(250);
  rightForward();
  delay(250);
  rightOff();
  leftForward();
  delay(250);
  rightOff();
  leftOff();
}



void loop() {
  test();
}






// print the sensor values as numbers from 0 to 1023, where 0 means maximum

// reflectance and 1023 means minimum reflectance
// 0, 1, 2, 3
// 0, 1, 3, 2
void test() {
  bool onBlack = false;
  qtr.read(sensorValues);

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(sensorValues[i]        );
    Serial.print("\t");
    if (!onBlack)
    {
      if (sensorValues[i] / 10 >= 70)
      {
        onBlack = true;
      }
    }

  } Serial.println();

  if (!onBlack and lastOnBlack) {
    while (!onBlack) {
      rightForward();
      leftBack();
      delay(50);
      rightOff();
      leftOff();
      onBlack = false;
      qtr.read(sensorValues);

      for (uint8_t i = 0; i < SensorCount; i++)
      {
        Serial.print(sensorValues[i]        );
        Serial.print("\t");
        if (!onBlack)
        {
          if (sensorValues[i] / 10 >= 70)
          {
            onBlack = true;
          }
        }

      } Serial.println();
    }
  }
  if (sensorValues[1] >= 700 or sensorValues[3] >= 700) {


    Serial.println("OnBlack");
    rightForward();
    leftForward();
    delay(correctionTime);
    rightOff();
    leftOff();
    //delay(150);


  } else if (sensorValues[2] >= 700) {

    turnRight(correctionTime);
    //delay(150);

  } else if (sensorValues[0] >= 700) {

    turnLeft(correctionTime);
    //delay(150);

  }


  else {

    rightBack();
    leftBack();
    delay(correctionTime);
    rightOff();
    leftOff();


  }

  //Serial.println();
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    lastSensors[i] =   sensorValues[i];
  }
  lastOnBlack = onBlack;


  //delay(50);
}
