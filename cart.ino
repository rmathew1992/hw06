/*Code for driving the cart with timer interrupts*/
#import <math.h>


int motorLeft = 3;
int motorRight = 5;

//Start speed of both motors
int START_SPEED;

//Initializing Sensor
const int dSensor = A0;
const int lSensor = A1;

int dValue = 0;            // value read from pot   
double sensorFloat = 0.0;      
double distance = 0.0;         // Measured in cm
int lValue = 0;
boolean toggle0 = 0;

void setup()
{ 
 //Initializing Motors
 pinMode(motorLeft,OUTPUT);  
 pinMode(motorRight,OUTPUT);
 Serial.begin(9600);

 //Start speed for both motors
 START_SPEED = 210;
 //START_SPEED = 0;
 analogWrite(motorLeft,START_SPEED);
 analogWrite(motorRight,START_SPEED);
 cli();//stop interrupts

//set timer0 interrupt at 2kHz
 TCCR0A = 0;// set entire TCCR0A register to 0
 TCCR0B = 0;// same for TCCR0B
 TCNT0  = 0;//initialize counter value to 0
 // set compare match register for 2khz increments
 // Change this value to adjust frequency of interrupt
 OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
 // turn on CTC mode
 TCCR0A |= (1 << WGM01);
 // Set CS01 and CS00 bits for 64 prescaler
 TCCR0B |= (1 << CS01) | (1 << CS00);   
 // enable timer compare interrupt
 TIMSK0 |= (1 << OCIE0A);

 sei();  
}

//timer0 interrupt
ISR(TIMER0_COMPA_vect){
 //dValue = analogRead(dSensor);
 //distance = calculateDistance(dValue);


 if(toggle0){   
     dValue = analogRead(dSensor);
 lValue = analogRead(lSensor); 
   checkDistance(dValue);   
   toggle0 = 0;
 }
 else{
   toggle0 = 1;
 }
}

void setBMotor(int newSpeed){
 analogWrite(motorLeft,newSpeed);
 analogWrite(motorRight,newSpeed);
}

void turnRight(int angle){
 analogWrite(motorRight,0);
 analogWrite(motorLeft,angle);
}

void checkDistance(int p){
 distance = calculateDistance(p);
 if (p<100){
   setBMotor(210);
 }
 else{
   turnRight(210);
   //setBMotor(0);
 }
}

double calculateDistance(int sensorValue)
{
 sensorFloat = (double) sensorValue;
 distance = 370.3 * exp(-0.01676 * sensorFloat) + 102.8 * exp(-0.002933 * sensorFloat);

 return distance;
}

void lightMode(int photocell)
{
Serial.print("light = ");
 // Based on the reading the photocell gives,
 // the lcd displays a qualitative interpretation of
 // how bright the room is.
 if (photocell < 10) {
   Serial.print("Dark");
 } 
 else if (photocell < 200) {
   Serial.print("Dim");
 } 
 else if (photocell< 500) {
   Serial.print("Light");
 } 
 else if (photocell < 800) {
   Serial.print("Bright");
 } 
 else {
   Serial.print("Very bright");
 }
}

void loop(){  

   Serial.println(dValue); 
   /*Serial.print(dValue);
   Serial.print(" ");
   Serial.println(lValue);   */
}
