#include "ACS712.h"
int count = 0;
int interval = 60;
int myArray [60];
double sumP = 0;

ACS712 sensor(ACS712_30A, A0);
void setup() {
  Serial.begin(9600);
  sensor.calibrate();
}

void loop() {
  float U = 230;
  float I = sensor.getCurrentAC();
  if (I >= 0.06 && I <= 0.1) I = 0;
  float P = U * I;
  Serial.println(P);
  count += 1;
  myArray[count -1] = P;
  if (count == interval)
  {
    for (int i = 0; i <= interval-1; i++)
    {
      sumP += myArray[i];
    }
    Serial.println(String("P = ") + sumP + " Watts");  
    count = 0;
    sumP = 0;
  }
  delay(1000);
}