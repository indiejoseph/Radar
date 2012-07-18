
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

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
  
  pinMode(dypOutputPin, OUTPUT);
  pinMode(dypInputPin,INPUT);
}

void loop()
{
  // The DYP-ME007 pings on the low-high flank...
  digitalWrite(dypOutputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(dypOutputPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(dypOutputPin, LOW);   

  // the distance is proportional to the time interval
  // between HIGH and LOW
  distance = pulseIn(dypInputPin, HIGH, 500000); 
  cm= distance/58;     
  
  if(angle < 0) {
    angle = 180;
  }
  
  Serial.print("{d:");
  Serial.print(cm);
  Serial.print(",r:");
  Serial.print(angle);
  Serial.print("}");
  
  val = map(angle, 0, 180, 0, 179);
  
  myservo.write(val);
  
  angle -= 10;
  
  delay(100);
}
