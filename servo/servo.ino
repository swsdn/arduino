#include <Servo.h>

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 90;    // variable to store the servo position 
int pos2 = 0;
 
void setup() 
{ 
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 


   // send data only when you receive data:
        while (Serial.available() > 0) {
                // read the incoming byte:
                int ch = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(ch -'0', DEC);
                
                switch(ch) {
                  case '0'...'9':
                    pos2 = pos2 * 10 + ch - '0';
                    break;
                  case 's':
                    myservo.write(pos2);
                    delay(1000);
                    myservo.write(pos);
                    pos2 = 0;
                    break;
                }
                
                 Serial.print("pos is: ");
                Serial.println(pos);   
        }
}
  
/*  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } */

