

 

#include <SD.h>

#include "TimerOne.h"

 

//motor pins

int motorLeft = 3;

int motorRight = 5;

 

//Initializing Sensor

const int dSensor = A0;

const int lSensor = A1;

 

int dValue = 0;            // value read from pot  

double sensorFloat = 0.0;     

double distance = 0.0;         // Measured in cm

int START_SPEED = 210;

 

 

// data storage initialization

File myFile;

int buffToWrite = 1; //which buffer to write to SD card

int index = 0;

int done = 0;

char buff1[256];

char buff2[256];

 

void setup(){

  //Initializing Motors

  pinMode(motorLeft,OUTPUT);

  pinMode(motorRight,OUTPUT);

 

  // Open serial communications and wait for port to open:

  Serial.begin(9600);

  while (!Serial) {

    ;

  }

  Serial.print("Initializing SD card...");

  pinMode(10, OUTPUT);

  if (!SD.begin()) {

    Serial.println("initialization failed!");

    return;

  }

  Serial.println("initialization done.");

 

  Timer1.initialize(30000);           //timer's period in us     

  Timer1.attachInterrupt(callback);   // attaches callback() as a timer overflow interrupt

 

  // start moving

  setBMotor(START_SPEED);

}

void setBMotor(int newSpeed){

  analogWrite(motorLeft,newSpeed);

  analogWrite(motorRight,newSpeed);

}

 

void turnRight(int angle){

  analogWrite(motorRight,0);

  analogWrite(motorLeft,angle);

}

 

// check for objects and either continue forward or turn to avoid

void checkDistance(int d){

  if (d<100){

    setBMotor(START_SPEED);

  }

  else{

    turnRight(START_SPEED);

  }

}

 

 

// interrupt function called to take data

void callback() //called 100 times/s

{

  dValue = analogRead(dSensor);

  checkDistance(dValue);  

 

  //write data to the correct buffer

  if (buffToWrite == 1) {

    buff2[index] = (char)(analogRead(A5)/4);

  }

  else {

    buff1[index] = (char)(analogRead(lSensor)/4);

  }

 

  // if the buffer is now full, switch the buffers and change done

  if (index >= 255) {

    index = 0;

    if (buffToWrite == 1) buffToWrite = 2;

    else buffToWrite = 1;

    done = 1;

  }

  else {

    index++;

    done = 0;

  }

 

}

 

void loop()

{

  if (done) {  //buffer is ready to be written

    myFile = SD.open("test.txt", FILE_WRITE);

    if (myFile) {

      // save correct buffer to SD card

      if (buffToWrite == 1) { //write buff1

        myFile.println(buff1);

      }

      else if (buffToWrite == 2) { //write buff2

        myFile.println(buff2);

      }

      // close the SD card everytime to ensure data is not lost if system crashes

      myFile.close();

 

 

    }

    else {

      Serial.println("error opening test.txt");

    }

  }

}