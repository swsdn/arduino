
int tachPin = 2;
int pwmPin = 9;
unsigned long volatile NbTopsFan = 0;
unsigned long millisMark = 0;
float actualRpm = 0;

float pwmWidth = 0.5;
int pwmMax = 1; //20kHz

void setup(){
	//start serial connection
	Serial.begin(9600);
	//configure pin2 as an input and enable the internal pull-up resistor
	pinMode(tachPin, INPUT_PULLUP);
	pinMode(pwmPin, OUTPUT);
	attachInterrupt(0, rpm, RISING);
	//digitalWrite(pwmPin, HIGH);
}

void rpm ()      //This is the function that the interupt calls 
{ 
	NbTopsFan++; 
} 

int readRpm(unsigned long millis) {
	unsigned long interval = millis - millisMark;
	if (interval > 1000) {
		//noInterrupts();
		// float intervalFactor = 1000.0 / (float)interval;
		// Serial.print("intervalFactor: ");
		// Serial.println(intervalFactor);
		actualRpm = ((float)NbTopsFan * 1000 / interval);
		NbTopsFan = 0;
		//interrupts();
		Serial.print (actualRpm, DEC); //Prints the number calculated above
		Serial.println(" rpm"); //Prints " rpm" and a new line
		Serial.println(pwmWidth);
		millisMark = millis;
	}
	return actualRpm;
}

int getPwm() {
	return pwmMax * pwmWidth;
}

void incrementPwmWidth() {
	pwmWidth += 0.01;
	if (pwmWidth > 1) {
		pwmWidth = 0;
	}
}

void loop(){
	analogWrite(pwmPin, 127);
	// int pwm = getPwm();
	// digitalWrite(pwmPin, HIGH);
	// delayMicroseconds(pwm);
	// digitalWrite(pwmPin, LOW);
	// delayMicroseconds(pwmMax - pwm);
	//Serial.println(pwmWidth);
	//incrementPwmWidth();

	int rpm = readRpm(millis());
	// if (rpm > 2000) {
	// 	pwmWidth -= 0.00001;
	// 	pwmWidth = pwmWidth <= 0 ? 0 : pwmWidth;
	// } else {
	// 	pwmWidth += 0.00001;
	// 	pwmWidth = pwmWidth >= 1 ? 1 : pwmWidth;
	// }
}



