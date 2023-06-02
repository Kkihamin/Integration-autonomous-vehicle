// TCS230 or TCS3200 pins wiring to Arduino 
// MIDDLE COLOR SENSOR
#define MS0 19
#define MS1 20
#define MS2 21
#define MS3 22
#define MSOut 23
// LEFT COLOR SENSOR
#define LS0 33
#define LS1 34
#define LS2 35
#define LS3 36
#define LSOut 37
//RIGHT COLOR SENSOR
#define RS0 32
#define RS1 31
#define RS2 30
#define RS3 29
#define RSOut 28
//Gripper
#include <Servo.h>
#define PS A14 //defines the pressure sensors pin
//Ultrasonic Sensor
#define echoPin 17 // attach Echo of HC-SR04
#define trigPin 18 //attach Trig of HC-SR04

//L298 Motor Controller Pin Assignment
volatile int gripperflag = 0;
volatile int state = 0;

bool searching = false;
// Motor A pin connections
int enA = 1;
int in1 = 2;
int in2 = 3;

// Motor B pin connections
int enB = 4;
int in3 = 5;
int in4 = 6;

// Motor C pin connections
int enC = 7;
int in5 = 8;
int in6 = 9;

// Motor D pin connections
int enD = 10;
int in7 = 11;
int in8 = 12;

// Stores frequency read by the photodiodes
int RredFreq = 0;
int RgreenFreq = 0;
int RblueFreq = 0;
int LredFreq = 0;
int LgreenFreq = 0;
int LblueFreq = 0;
int MredFreq = 0;
int MgreenFreq = 0;
int MblueFreq = 0;

// Stores the red. green and blue colors
int RredCol = 0;
int RgreenCol = 0;
int RblueCol = 0;
int LredCol = 0;
int LgreenCol = 0;
int LblueCol = 0;
int MredCol = 0;
int MgreenCol = 0;
int MblueCol = 0;

//Stores color that the sensor sees
int RCol = 0;
int LCol = 0;
int MCol = 0;

//Initialize for ultrasonic
long duration;
volatile int distance = 0;

Servo Gripper;  // create servo object to control a servo

//Gripper variables
int GripperClose = 120;
int GripperOpen = 170;
int GripperPosition = 0; //initialize gripper position
float FS_Threshold = 15;
int Size = 0;

void setup() {
  ////////////Setting the color sensors///////////
  // Setting the outputs
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(MS0, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(RSOut, INPUT);
  pinMode(LSOut, INPUT);
  pinMode(MSOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);
  digitalWrite(MS0, HIGH);
  digitalWrite(MS1,LOW);

  ///////////Setting the motors///////////
  // Set all motor controller pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(enD, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(in5, OUTPUT);
  pinMode(in6, OUTPUT);
  pinMode(in7, OUTPUT);
  pinMode(in8, OUTPUT);

  // Initial state - Turn off all the motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);

  //Gripper
  Gripper.attach(39);  // attaches the servo on pin 39 to the servo object
  pinMode(PS, INPUT);
  Gripper.write(GripperOpen); //Initially have gripper open

  //Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Begins serial communication
  Serial.begin(115200);
}

//For the main code whatever color the different sensors see, a different action is implemented
//For reference Black = 0, Red = 1, Blue = 2, Green = 3, White = 4
void loop(){
  // grab();
  // search();
 
 
readColorSensors();
readUltrasonic(); 
  //Serial.println(distance);
 // movFW(100); //move forward until the ultrasonic sensor detects the platform
  //if the ultrasonic sensor reads 10 then the vehicle is in place to grab the box
  if(state == 0){
    if(distance <= 10){
      movFW(25);
      delay(3000);  
      stop();
      grab(); //grab the box
      movBW(100); //move back so the ultrasonic sensor no longer sees 10
      delay(500);
      movCW(100);
      delay(4200);
      stop();
    }
    state = 1;
  }
  if(state == 1){
    readColorSensors();
    if(searching == true){
      search();
    }else {
      searching = false;
      follow();
    }
  }
  

/*
  if(LCol == 3 && RCol == 3){
    search();
  }
  follow();
  */
}

void readColorSensors() 
{
  ///////////////////// FOR RIGHT COLOR SENSOR /////////////////////////////
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,LOW);
  // Reading the output frequency
  RredFreq = pulseIn(RSOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  RredCol = map(RredFreq, 47, 172, 255, 0);
  // Printing the RED (R) value
  // Serial.print("RR = ");
  // Serial.print(RredFreq);
  // delay(10);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(RS2,HIGH);
  digitalWrite(RS3,HIGH);
  // Reading the output frequency
  RgreenFreq = pulseIn(RSOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  RgreenCol = map(RgreenFreq, 47, 176, 255, 0);
  // Printing the GREEN (G) value  
  // Serial.print(" RG = ");
  // Serial.print(RgreenFreq);
  // delay(10);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,HIGH);
  // Reading the output frequency
  RblueFreq = pulseIn(RSOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  RblueCol = map(RblueFreq, 35, 133, 255, 0);
  // Printing the BLUE (B) value 
  // Serial.print(" RB = ");
  // Serial.print(RblueFreq);
  // delay(10);
  /////////////////////////////////////////////////////////////////////////

  ///////////////////// FOR LEFT COLOR SENSOR /////////////////////////////
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,LOW);
  // Reading the output frequency
  LredFreq = pulseIn(LSOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  LredCol = map(LredFreq, 34, 149, 255, 0);
  // Printing the RED (R) value
  // Serial.print(" LR = ");
  // Serial.print(LredFreq);
  // delay(10);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(LS2,HIGH);
  digitalWrite(LS3,HIGH);
  // Reading the output frequency
  LgreenFreq = pulseIn(LSOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  LgreenCol = map(LgreenFreq, 35, 167, 255, 0);
  // Printing the GREEN (G) value  
  // Serial.print(" LG = ");
  // Serial.print(LgreenFreq);
  // delay(10);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,HIGH);
  // Reading the output frequency
  LblueFreq = pulseIn(LSOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  LblueCol = map(LblueFreq, 29, 138, 255, 0);
  // Printing the BLUE (B) value 
  // Serial.print(" LB = ");
  // Serial.println(LblueFreq);
  // delay(10);
  ////////////////////////////////////////////////////////////////////////////

  ///////////////////// FOR MIDDLE COLOR SENSOR /////////////////////////////
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,LOW);
  // Reading the output frequency
  MredFreq = pulseIn(MSOut, LOW);
  // Remaping the value of the RED (R) frequency from 0 to 255
  MredCol = map(MredFreq, 18, 118, 255, 0);
  // Printing the RED (R) value
  // Serial.print("MR = ");
  // Serial.print(MredFreq);
  // delay(10);

  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(MS2,HIGH);
  digitalWrite(MS3,HIGH);
  // Reading the output frequency
  MgreenFreq = pulseIn(MSOut, LOW);
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  MgreenCol = map(MgreenFreq, 20, 176, 255, 0);
  // Printing the GREEN (G) value  
  // Serial.print(" MG = ");
  // Serial.print(MgreenFreq);
  // delay(10);

  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,HIGH);
  // Reading the output frequency
  MblueFreq = pulseIn(MSOut, LOW);
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  MblueCol = map(MblueFreq, 17, 165, 255, 0);
  // Printing the BLUE (B) value 
  // Serial.print(" MB = ");
  // Serial.print(MblueFreq);
  // delay(10);

  ///////////////Translating frequency to colors///////////////
  //read black color for all sensors
  if((LblueCol <= 70) && (LredCol <= 70) && (LgreenCol <= 70))
  {
       Serial.println("Left: Black");
      LCol = 0;
  }
  if((RblueCol <= 0) && (RredCol <= 0) && (RgreenCol <= 0))
  {
    Serial.println("Right: Black");
      RCol = 0;
  }

  //read red color for all sensors
  if((LblueCol <= 115) && (LredCol >= 200 && LredCol <= 250) && (LgreenCol <= 105))
  {
    //  Serial.println("Left: Red");
      LCol = 1;
  }
  if((RblueCol <= 0) && (RredCol >= 150 && RredCol <= 250) && (RgreenCol <= 0))
  {
    //  Serial.println("Right: Red");
      RCol = 1;
  }
  if((MblueCol <= 120) && (MredCol >= 160 && MredCol <= 250) && (MgreenCol <= 100))
  {
    //  Serial.println("Middle: Red");
      MCol = 1;
  }

  //read blue color for all sensors
  if((LblueCol >= 150 && LblueCol <= 220) && (LredCol <= 30) && (LgreenCol <= 160))
  {
    // Serial.println("Left: Blue");
      LCol = 2;
  }
  if((RblueCol >= 150 && RblueCol <= 250) && (RredCol <= 40) && (RgreenCol <= 150))
  {
      // Serial.println("Right: Blue");
      RCol = 2;
  }
  if((MblueCol >= 170 && MblueCol <= 250) && (MredCol <= 70) && (MgreenCol <= 150))
  {
      // Serial.println("Middle: Blue");
      MCol = 2;
  }

  //read green color for all sensors
  if((LblueCol <= 90) && (LredCol <= 70) && (LgreenCol >= 50 && LgreenCol <= 150))
  {
      Serial.println("Left: Green");
      LCol = 3;
  }
  if((RblueCol <= 30) && (RredCol <= 30) && (RgreenCol >= -30 && RgreenCol <= 100))
  {
      Serial.println("Right: Green");
      RCol = 3;
  }

  //read white color for all sensors
  if((LblueCol >= 240) && (LredCol >= 240) && (LgreenCol >= 240))
  {
    //  Serial.println("Left: White");
      LCol = 4;
  }
  if((RblueCol >= 240) && (RredCol >= 240) && (RgreenCol >= 240))
  {
      // Serial.println("Right: White");
      RCol = 4;
  }

  //else print frequency to tune
  else 
  {
    //Middle Sensor
    // Serial.print("Middle Blue: ");
    // Serial.println(MblueCol);
    // Serial.print("Middle Red: ");
    // Serial.println(MredCol);
    // Serial.print("Middle Green: ");
    // Serial.println(MgreenCol);
    //Left Sensor
    // Serial.print("Left Blue: ");
    // Serial.println(LblueCol);
    // Serial.print("Left Red: ");
    // Serial.println(LredCol);
    // Serial.print("Left Green: ");
    // Serial.println(LgreenCol);
    //Right Sensor
    // Serial.print("Right Blue: ");
    // Serial.println(RblueCol);
    // Serial.print("Right Red: ");
    // Serial.println(RredCol);
    // Serial.print("Right Green: ");
    // Serial.println(RgreenCol);
  }
}

// stop any movement
void stop(){
  // move forward with 0% speed - For PWM value 0
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  analogWrite(enC, 0);
  analogWrite(enD, 0);
  
  // Turn off all the motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);  
}

// move forward
void movFW(int speed)
{
  int PWMval = 255*speed/100;
  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);
  // Rotate left and right wheels in clockwise direction - Move forward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
}

// move backward
void movBW(int speed)
{
  int PWMval = 255*speed/100;
  // move backward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);
  // Rotate left and right wheels in anti-clockwise direction - Move backward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH); 
}

// turn left
void movCW(int speed)
{
  int PWMval = 255*speed/100;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);
  // Rotate left wheel clockwise and right wheel in anti-clockwise direction - Spin clockwise/Turn right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
}

// turn right
void movCCW(int speed)
{
  int PWMval = 255*speed/100;
  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);
  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
}

//move left
void movLFT(int speed){
  int PWMval = 255*speed/100;

  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);

  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in5, HIGH);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, HIGH);
}

//move right
void movRT(int speed){
   int PWMval = 255*speed/100;

  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);

  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
}

//move diagonally right
void movDiagRT(int speed){
  int PWMval = 255*speed/100;

  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);

  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, HIGH);
  digitalWrite(in8, LOW);
}

//move diagonally left
void movDiagLFT(int speed){
  int PWMval = 255*speed/100;

  // turn with 30% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
  analogWrite(enA, PWMval);
  analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
  analogWrite(enD, PWMval);

  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, HIGH);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
}

void grab(){
  for(int i = 0; i <= 50; i++)
  {
    readUltrasonic();
    int Press = analogRead(PS); //Reads force on sensor //Testing purposes
    float Force = Press/1.5;

    //  Serial.println(Force);
    Gripper.write(GripperOpen-i); //Decreases the gripper's position from open one step at a time
    GripperPosition = GripperOpen-i; //Set gripper position
    delay(50);
    if(Force >= FS_Threshold) //if the force sensor reads a pressure
    {
      gripperflag = 1;
      Gripper.write(GripperPosition); //setting the gripper position
      i = 60; //in case break doesn't break out of the loop set i = 60
      break; //Exit the for loop and set gripper position in place
      
      //Sets size based on the gripper position to be used later
      if((GripperPosition <= 120 && GripperPosition >= 131)) //if the gripper position is more closed
      {
        Size = 0; // box is small
      }

      if((GripperPosition <= 140 && GripperPosition >= 150))
      {
        Size = 1; // box is large
      }
    }
  }
}

void readUltrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (send & receive)
  Serial.println(distance);
  delay(100);
}

void search(){
  Serial.println("Searching");
  readColorSensors();
  movFW(100);
  delay(4000);
  stop();
  if(MCol == 1){ //if the box is red look for the red line
    readColorSensors();
    if (LCol == 3 && RCol == 3) {
      stop();
      movLFT(100); // align car to the center of the line
      delay(100);
    } 
    if(LCol == 1 && RCol == 1){
      movFW(100);

    }
  }

  if(MCol == 2){
    readColorSensors();
    if (RCol == 2) {
      stop();
      movRT(100); // align car to the center of the line
      stop();
      movRT(100);
      delay(100);
    }
  }
}
void follow() {
  //if the ground sensors (left and right) read black the vehicle will go forward
  // if (RCol == 0 && LCol == 0) {
  //   stop();
  //   movFW(100);
  // }
  movFW(100);
  delay(500);
  //if the ground sensors read white the vehicle will have to correct left or right depending on which sensor is white
  if (RCol == 4) {
    stop();
    movLFT(100);
  }
  if (LCol == 4) {
    stop();
    movRT(100);
  }
  //if the middle sensor detects red follow the red line
  if (MCol == 1 && RCol == 1) {
    stop();
    movDiagRT(100);
    correct();
    delay(2000);
    if (RCol == 1) {
      stop();
      movRT(100);
      if (RCol == 0 && LCol == 0) {
        search();
      }
    }
  }
  //if the middle sensor detects blue follow the blue line
  if (MCol == 2 && LCol == 2) {
    stop();
    movDiagLFT(100);
    delay(500);
    correct();
  }
  //if all sensors read red and the size is big turn left
  if (MCol == 1 && RCol == 1 && LCol == 1 && Size == 1) { //placeholder for Size 
    stop();
    movLFT(100);
    //if the left sensor sees black and the right sees red go forward
    if (RCol == 1 && LCol == 0) {
      stop();
      movFW(100);
      //if the ultrasonic sensor detects a platform open the gripper
      if (distance == 10 || distance == 9) {
        stop();
        Gripper.write(GripperOpen - 1);
      }
    }
  }
  //if all sensors read red and the size is small turn right
  if (MCol == 1 && RCol == 1 && LCol == 1 && Size == 0) { //placeholder for Size 
    stop();
    movRT(100);
    //if the right sensor sees black and the left sees red go forward
    if (RCol == 0 && LCol == 1) {
      stop();
      movFW(100);
      //if the ultrasonic sensor detects a platform open the gripper
      if (distance <= 10) {
        stop();
        Gripper.write(GripperOpen - 1);
      }
    }
  }
  //if all sensors read blue and the size is big turn left
  if (MCol == 2 && RCol == 2 && LCol == 2 && Size == 1) { //placeholder for Size 
    stop();
    movLFT(100);
    //if the left sensor sees black and the right sees blue go forward
    if (RCol == 2 && LCol == 0) {
      stop();
      movFW(100);
      //if the ultrasonic sensor detects a platform open the gripper
      if (distance <= 10) {
        stop();
        Gripper.write(GripperOpen - 1);
      }
    }
  }
  //if all sensors read blue and the size is small turn right
  if (MCol == 2 && RCol == 2 && LCol == 2 && Size == 0) {
    stop();
    movRT(100);
    if(RCol == 2 && LCol == 2){
      stop();
      movFW(100);
      if(distance <= 10){
        stop();
        Gripper.write(GripperOpen - 1);
      }
    }
  }
}
void correct(){
  if(RCol == 1 || RCol == 2){
    stop();
    movRT(100);
  }
  if(LCol == 1 || LCol == 2){
    stop();
    movLFT(100);
  }
}

