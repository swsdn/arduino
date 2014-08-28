int pin = 7;
volatile unsigned long duration = 0;

void setup()
{
  //pinMode(pin, INPUT);           // set pin to input
  //digitalWrite(pin, HIGH);       // turn on pullup resistors
  pinMode(2, INPUT);           // set pin to input
  digitalWrite(2, LOW);       // turn on pullup resistors
  Serial.begin(9600);
  attachInterrupt(0, printSerial, CHANGE);
}

void loop()
{
  duration++;

}

void printSerial() {
   Serial.println(duration);
}
