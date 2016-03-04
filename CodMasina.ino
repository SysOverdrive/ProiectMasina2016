/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
*/
// la control la pornire trebe duty cicle sub 6%
/***
 *vom folosi o frecenta de 500 de Herz rescriind libraria cu un timer digital 
 */
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val,val1;    // variable to read the value from the analog pin
int duty=50;  
int dc=0;
void setup()
{
  
 myservo.attach(9);  // attaches the servo on pin 9 to the servo object
 myservo.write(0);
//analogWrite(9,0);    
pinMode(2,OUTPUT);
digitalWrite(2,LOW);
delay(3000);


}

void timerSoftware()
{
 
    val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
    val1 = map(val, 0, 500,50, 180);     // scale it to use it with the servo (value between 0 and 180) 
    val = map(val, 0, 300,0, 100); 
    duty=val;
    myservo.write(val1);
}

void loop() 
{ 

 // if (dc>duty) myservo.write(50);else myservo.write(200);
  if (dc>duty) digitalWrite(2,LOW);else digitalWrite(2,HIGH);
 dc++;
 if (dc>100){dc=0;
 timerSoftware();} 
 // myservo.write(180);
 //analogWrite(9,180);
 //delay(100);
  //delay(100);
 // myservo.write(50);
 //analogWrite(9,50);
  //delay(100);
  //analogWrite(9,50);
 // myservo.write(0);
  //delay(100);
  delayMicroseconds(15);
  // sets the servo position according to the scaled value 
                           // waits for the servo to get there 
} 
