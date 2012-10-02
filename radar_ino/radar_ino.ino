/*
 * Radar
 *
 * ----------------------------------
 * Author: Joseph Cheng<9Lab.co> 
 * Date: 2012-10-02 11:37:12
 * Revision: 2
 * ----------------------------------
 */

#include <Servo.h>
#include <Ultrasonic.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     13

Servo myservo;  // create servo object to control a servo 
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

// this constant won't change.  It's the pin number
// of the sensor's output:
int angle = 179;
int dypOutputPin = 2; // TRIG
int dypInputPin = 3;  // ECHO
long distance;
long cm;
int val;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
}

void loop()
{
  float cmMsec;
  long microsec = ultrasonic.timing();   

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  if(angle < 0) {
    angle = 180;
  }
  
  Serial.print("{d:");
  Serial.print(cmMsec);
  Serial.print(",r:");
  Serial.print(angle);
  Serial.print("}");
  
  val = map(angle, 0, 180, 0, 179);
  
  myservo.write(val);
  
  angle -= 15;
  
  delay(100);
}
