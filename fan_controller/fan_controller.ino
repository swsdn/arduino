/*
 * Blink
 *
 * The basic Arduino example.  Turns on an LED on for one second,
 * then off for one second, and so on...  We use pin 13 because,
 * depending on your Arduino board, it has either a built-in LED
 * or a built-in resistor so that you need only an LED.
 *
 * http://www.arduino.cc/en/Tutorial/Blink
 */

int fanPin = 03;                // LED connected to digital pin 13
int btnPin = 02;
bool DEBUG = false;

void setup()                    // run once, when the sketch starts
{
  pinMode(fanPin, OUTPUT);      // sets the digital pin as output
  pinMode(btnPin, INPUT);
  digitalWrite(btnPin, HIGH);
  attachInterrupt(0, updatePwm, FALLING);
  if (DEBUG) {
    Serial.begin(9600);
  }
}

volatile byte pwm = 255;


void loop()                     // run over and over again
{
  analogWrite(fanPin, pwm);
  delay(1000);
  
  if (DEBUG) {
    unsigned long readSum = 0;
    int readCount = 20;
    for (int i = 0; i < readCount; i++) {
      readSum += analogRead(0);
    }
    float temp = readSum/readCount * 5 / 1024.0;  
    temp = temp - 0.5;
    temp = temp / 0.01;  
    printLabel("temp", temp);
  }
}

void printLabel(String label, float value) {
  Serial.print(label);
  Serial.print(": ");
  Serial.println(value);
}

void updatePwm() {
  pwm += 64;
  if (DEBUG) {
    Serial.print("int0: ");
    Serial.println(pwm);
  }
}
