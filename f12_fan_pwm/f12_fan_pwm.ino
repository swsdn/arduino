// Analog output (i.e PWM) pins. These must be chosen so that we can change the PWM frequency without affecting the millis()
// function or the MsTimer2 library. So don't use timer/counter 1 or 2. See comment in setup() function.
// THESE PIN NUMBERS MUST NOT BE CHANGED UNLESS THE CODE IN setup() AND setFanSpeed() IS CHANGED TO MATCH!
// On the Uno we can only use the OC1B pin

const int fanPin = 10;     // OC1B
const unsigned char maxFanSpeed = 80;   // this is calculated as 16MHz divided by 8 (prescaler), divided by 25KHz (target PWM frequency from Intel specification)

void setup()
{
  // Set up the PWM pins for a PWM frequency close to the recommended 25KHz for the Intel fan spec.
  // We can't get this frequency using the default TOP count of 255, so we have to use a custom TOP value. 
  // Only timer/counter 1 is free because TC0 is used for system timekeeping (i.e. millis() function),
  // and TC2 is used for our 1-millisecond tick. TC1 controls the PWM on Arduino pins 9 and 10.
  // However, we can only get PWM on pin 10 (controlled by OCR1B) because we are using OCR1A to define the TOP value.
  // Using a prescaler of 8 and a TOP value of 80 gives us a frequency of 16000/(8 * 80) = 25KHz exactly.
  // TCCR1A = (1 << COM1B1) | (1 << COM1B0) | (1 << WGM11) | (1 << WGM10);  // OC1A (pin 9) disconnected, OC1B (pin 10) = inverted fast PWM  
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);  // OC1A (pin 9) disconnected, OC1B (pin 10) = inverted fast PWM  
 #ifdef FAN_AUDIO_TEST
  // test code to get 440Hz output (= concert A) to test the logic 
  OCR1AH = 0;
  OCR1BL = 71;  // 50% duty cycle 
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);  // TOP = OCRA, prescaler = 256

  OCR1AL = 141; // TOP = 141, 16000000 / (256 * 142) = 440.014
  OCR1BH = 0;
 #else
  OCR1AH = 0;
  OCR1AL = 79;  // TOP = 79
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // TOP = OCR0A, prescaler = 8

  OCR1BH = 0;
  OCR1BL = maxFanSpeed;  // max fan speed 
 #endif

  TCNT1H = 0;
  TCNT1L = 0; 

  // We have to enable the ports as outputs before PWM will work.
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);
}

// Set the fan speed, where 0 <= fanSpeed <= maxFanSpeed
void setTransistorFanSpeed(unsigned char fanSpeed)
{
  if (fanSpeed > maxFanSpeed) {
    fanSpeed = maxFanSpeed;
  } else if (fanSpeed < 0) {
    fanSpeed = 0;
  }
  OCR1BH = 0;
  OCR1BL = fanSpeed;
}

float measureTemperature(int analogPin) {
  float temp = 0;
  int readCount = 10;
  for (int count = 0; count < readCount; count++) {
    float read = analogRead(analogPin) * 5/1024.0;
    read = read - 0.5;
    temp += read / 0.01;
  }
  return temp / readCount;
}

void loop() {
  unsigned char speed = 0;
  float prevTemp0 = 0;
  printLabels();
  while(true) {
    float temp0 = measureTemperature(0);
    speed = nextSpeed(temp0, prevTemp0, speed);
    setTransistorFanSpeed(speed);
    prevTemp0 = temp0;
    log(temp0, speed);
    delay(5000);
  }
}

unsigned char nextSpeed(float temp, float prevTemp, unsigned char previousSpeed) {
  float nextSpeed = previousSpeed;
  if (temp > 40.0) {
    return maxFanSpeed;
  } else if (temp > 26.5) {
    if (previousSpeed == 0) {
      nextSpeed += 15;
    }
    nextSpeed *= temp > prevTemp ? 1.1f : 1.0f;
    return nextSpeed > maxFanSpeed ? maxFanSpeed : nextSpeed;
  } 
  return 0;
}

void printLabels() {
  Serial.print("TEMP\t");
  Serial.println("SPEED");
}

void log(float t, int s) {
  Serial.print(t);
  Serial.print("\t");
  Serial.println(s);
}
