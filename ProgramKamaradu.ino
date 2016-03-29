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
Dioada skotty ii ca sa nu alimentam din gresala de la laptop
///!!!!!!!!!!!!! TOT TIMPUL SCOTI BATERIA CAND PROGRAMEZI?????????????????/
//POTENTIOMETRUL PE PINUL A0 daca este necesar
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int potpin = A0;  // analog pin used to connect the potentiometer
int val, val1;    // variable to read the value from the analog pin
//ANDROID CONTROL PART---------------------------------------------------------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ATENTIE AICI E SUFLETU 2.0 ---------------------------------------------------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//MUIE LA DUSMANI---------------------------------------------------------------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 int directieServo = 65;//DREAPTA 5,6% == MYSERVO 70-----------------------MIJLOC 7,5 % == MYSERVO 90-----------------------------------------STANGA 9, 8 % == MYSERVO 110
 int startMotor = true;
 int dc = 0;
//AICI II SUFLETU--------------------------------------------------------------------------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//ISR Part
volatile unsigned long timp2,timp3;
volatile  float frecventa;
volatile int duty = 30;
int interuptPin=3;// we set this for platform switching purpose //REACTIA DE LA MOTOR PRIN FREQ DIV
unsigned long timp1;
 char buffer[32];
 

void setup()
{
	timp1 = millis();
	myservo.attach(9);  // attaches the servo on pin 9 to the servo object
	myservo.write(90);
	Serial.begin(115200);
	//analogWrite(9,0);    
	pinMode(2, OUTPUT);
	pinMode(8, OUTPUT);//PWM OUT
	analogWrite(8, 15);
	digitalWrite(2, LOW);
	//ISR part
	pinMode(interuptPin, INPUT);
	attachInterrupt(digitalPinToInterrupt(interuptPin), ISRFunction, RISING);
	timp2 = micros();
	frecventa = 0;
	//Frequency reset of timer 1

	int myEraser = 7;             // this is 111 in binary and is used as an eraser
	TCCR4B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0

	int myPrescaler = 4;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
	TCCR4B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
	//end of timer 1 frequency rescaling

	delay(3000); //delay

	timp3 = millis();
	//analogWrite(8, 80);
}
//Task 01
// configureaza un timer la frecventa de 50 de kHz si sa pun o intrerupere la ea
// comuta un pin sus jos si vezi daca am 50 de kHz pe osciloscop
/*

*/
//valori control servo ---
/*
mijlocul e la 6 % pwm 
STANGA 
MIJLOC 7,5 % == MYSERVO 90
DREAPTA 5,6% == MYSERVO 70
DREAPTA 9,8 % == MYSERVO 110

*/

void timerSoftware()
{
	
	/*-----------------TESTARE VALORI PENTRU CONTROL --------------------------
	val = map(val, 0, 700, 0, 255);				//motor
	//duty = val;
	val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1223)
	val = map(val, 0, 500, 35, 180);    // scale it to use it with the servo (value between 0 and 180)


-------------------FOLOSIND POTENTIOMETRUL---------------------------------*/
	myservo.write(directieServo);
	analogWrite(8, duty);
	Frecventiometer();


}
//---ISR PART 
//Functia din ISR
void ISRFunction()
{	
	frecventa = 1000000/(float)(micros() - timp2);
	timp2 = micros();
	//Serial.print(frecventa);


}
//Frecventiometer

void Frecventiometer()
{

	unsigned int frec1 = (int)frecventa;						// pentru functia snprint care NU accepta double
	unsigned int frec2 = (int)(frecventa * 10) % 10;

	snprintf(buffer, sizeof(buffer), "%d.%d", frec1, frec2);		// metoda antica toString NU merge cu double
	//Serial.println(buffer);
	//Serial.println(duty);
	if (frec1 < 180) duty++; else duty--;

	if (duty < 30)duty = 30;
	if (duty > 180) duty = 180;

}
//timer software


void loop()
{	
	
	//folosim milis sa facem ceva la intervale de timp predefinite
	//----------------------------------------------ASTA !! Impreuna cu funtia timer software 
	if (millis() - timp3 < 3000) // timp de mers pentru test

	{


		if (startMotor) {
			if ((millis() - timp1) > 150)
			{
				timp1 = millis();
				timerSoftware();

			}
		}
	}
	else analogWrite(8, 0);
	//-----------------------------------------------
}
/*
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
OLD CODE	

vechiul timer software
if (dc>100) {
dc = 0;
timerSoftware();
}

delayMicroseconds(150);
--sfarsit timer
vechiul pwm softare
if (dc>duty) digitalWrite(2, LOW); else digitalWrite(2, HIGH);
dc++;
--sfarsit

*/