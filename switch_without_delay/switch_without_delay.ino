int buttonPin = 10;
int ledPin = 13;

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

long time = 0;
long debounce = 200;
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop()
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && lastButtonState == LOW && millis() - time > debounce)
  {
    if (ledState == HIGH)
    {
      ledState = LOW;
    }
    else
    {
      ledState = HIGH;
    }
    time = millis();
  }
  digitalWrite(ledPin,ledState);
  lastButtonState = buttonState;
}
