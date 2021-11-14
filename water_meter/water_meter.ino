byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin1      = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;   

float flowRate;

unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;
unsigned long pTime = 30000;


void setup(){
  Serial.begin(9600);
  pinMode(sensorPin1, INPUT);
  digitalWrite(sensorPin1, 1);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime2          = 0;
  
}


void loop(){
  measureAndSend();
}


void measureAndSend() {
  if(millis() - oldTime2 >= 5000)
  {
    int value = totalMilliLitres/1000;
    String value_water = String(value);
    Serial.println(value);
     
    oldTime2 = millis();
    value_water = "";
  }
  if((millis() - oldTime) > 1000)    // Only process counters once per second
  {
    detachInterrupt(sensorInterrupt);

    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

    oldTime = millis();

    flowMilliLitres = (flowRate / 60) * 1000;
    
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    
    pulseCount = 0;
    
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}


void pulseCounter()
{
  pulseCount++;
}