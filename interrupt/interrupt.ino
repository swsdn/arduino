int pin = 13;
volatile int state = LOW;

void setup()
{
  pinMode(pin, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
  attachInterrupt(0, blink, LOW);
}

void loop()
{
  digitalWrite(pin, state);
}

void blink()
{
  state = !state;
}
