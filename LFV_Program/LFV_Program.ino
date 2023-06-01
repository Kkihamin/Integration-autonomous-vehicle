// Time-Step Degree of Vehicle Rotation
// Written by: David Ki
#define TimeStep_ms 10
// LEFT and RIGHT TCS230 Color Sensor Pin Assignments

#define LS0 32
#define LS1 31
#define LS2 30
#define LS3 29
#define L_SensorOut 28

#define MS0 19
#define MS1 20
#define MS2 21
#define MS3 22
#define M_SensorOut 23

#define RS0 33
#define RS1 34
#define RS2 35
#define RS3 36
#define R_SensorOut 37

// LEFT and RIGHT Color Sensor Parameters

int LredFrequency = 0;
int LgreenFrequency = 0;
int LblueFrequency = 0;
int LredColor = 0;
int LgreenColor = 0;
int LblueColor = 0;
int Lcolor;
int Rcolor;
int Mcolor;
bool searching = true;
int LstopColor = 0;

int RredFrequency = 0;
int RgreenFrequency = 0;
int RblueFrequency = 0;
int RredColor = 0;
int RgreenColor = 0;
int RblueColor = 0;
int RstopColor = 0;
// middle color sensors or the third color sensors on the gripper
int MredFrequency = 0;
int MgreenFrequency = 0;
int MblueFrequency = 0;
int MredColor = 0;
int MgreenColor = 0;
int MblueColor = 0;
int MstopColor = 0;

//L298 Motor Controller Pin Assignment

// motor A
int EnableENA = 1;
int A_in1 = 2;
int A_in2 = 3;

// motor B
int EnableENB = 4;
int B_in3 = 5;
int B_in4 = 6;

// motor C
int EnableENC = 7;
int C_in5 = 8;
int C_in6 = 9;

// motor D
int EnableEND = 10;
int D_in7 = 11;
int D_in8 = 12;

void setup()
{
  // Set all motor controller pins as outputs
	
  pinMode(EnableENA, OUTPUT);
  pinMode(EnableENB, OUTPUT);
  pinMode(EnableENC, OUTPUT);
  pinMode(EnableEND, OUTPUT);
  pinMode(A_in1, OUTPUT);
  pinMode(A_in2, OUTPUT);
  pinMode(B_in3, OUTPUT);
  pinMode(B_in4, OUTPUT);
  pinMode(C_in5, OUTPUT);
  pinMode(C_in6, OUTPUT);
  pinMode(D_in7, OUTPUT);
  pinMode(D_in8, OUTPUT);
 
	
  // Initial state - Turn off all the motors
	
  digitalWrite(A_in1, LOW);
  digitalWrite(A_in2, LOW);
  digitalWrite(B_in3, LOW);
  digitalWrite(B_in4, LOW);
  digitalWrite(C_in5, LOW);
  digitalWrite(C_in6, LOW);
  digitalWrite(D_in7, LOW);
  digitalWrite(D_in8, LOW);

  // Port setup for LEFT color sensor pins
  
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(L_SensorOut, INPUT);
  
  // Setting LEFT sensor frequency scaling to 20%
  
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);
  
  // Port setup for RIGHT color sensor pins
  
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(R_SensorOut, INPUT);
  
  // Setting RIGHT sensor frequency scaling to 20%
  
  digitalWrite(MS0,HIGH);
  digitalWrite(MS1,LOW);

  pinMode(MS0, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(M_SensorOut, INPUT);
  
  // // Setting Middle sensor frequency scaling to 20%
  
  digitalWrite(MS0,HIGH);
  digitalWrite(MS1,LOW);
  
  // Begins serial communication
  Serial.begin(115200);
}

void loop() 
{
  //////////////// PUT LFV CONTROL PROGRAM HERE ////////////////
  ReadColorSensors();
  // if(searching){
  //   Search();
  // }else{
  //   follow();
  // }
  Serial.println("Red: ");
  Serial.println(RredFrequency);
  Serial.println("Green: ");
  Serial.println(RgreenFrequency);
  Serial.println("Blue: ");
  Serial.println(RblueFrequency);
  // Serial.println(Lcolor);
  // Serial.print("left color: ");
  // Serial.println(Lcolor);
  // if(Lcolor = WHITE){
  //   Serial.println(WHITE);
  // }
  Serial.print("right color: ");
  Serial.println(Rcolor);
}

void Search(){
  Serial.println("search");
  RotateRt(17);
  ReadColorSensors();
  if(Lcolor == 1 || Lcolor == 2 || Lcolor == 3){
    searching = false;
  }
}
  
void follow(){
  ReadColorSensors();
  Serial.println("follow");
  if(Lcolor == 1 || Rcolor == 1){
    searching = true;
  }
  else if((Lcolor == 1 && Rcolor == 1)){
    searching = true;
  } else {
    if((Rcolor == 1 || Rcolor == 2 || Rcolor == 3)){
      RotateRt(25);
      Serial.println("turn right");
      ReadColorSensors();
      
    }
    else if((Lcolor == 1 || Lcolor == 2 || Lcolor == 3) ){
      RotateLft(25);
      Serial.println(" turn left");
      ReadColorSensors();

    } else if(Lcolor == 4 && Rcolor == 4) {
      MovFWD(17);
      Serial.println("Move forward");
      ReadColorSensors();
    }
  }
 
  
}

//////////////////////////////////////////////////////////////////////////////
// ReadColorSensors():  Read RIGHT and LEFT color sensors.
//////////////////////////////////////////////////////////////////////////////

void ReadColorSensors()
{
  // Setting RED (R) filter photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,LOW);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,LOW);
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,LOW);
  
  // Reading the output frequency
  
  RredFrequency = pulseIn(R_SensorOut, LOW);
  LredFrequency = pulseIn(L_SensorOut, LOW);
  MredFrequency = pulseIn(M_SensorOut, LOW);
  
  // Remaping the value of the RED (R) frequency from 0 to 255
  
  RredColor = map(RredFrequency, 0, 40, 0, 255);
  LredColor = map(LredFrequency, 0, 40, 0, 255);
  MredColor = map(MredFrequency, 0, 40, 0, 255);
 
  // Setting GREEN (G) filtered photodiodes to be read
  
  digitalWrite(RS2,HIGH);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,HIGH);
  digitalWrite(LS3,HIGH);
  digitalWrite(MS2,HIGH);
  digitalWrite(MS3,HIGH);
  
  // Reading the output frequency
  
  RgreenFrequency = pulseIn(R_SensorOut, LOW);
  LgreenFrequency = pulseIn(L_SensorOut, LOW);
  MgreenFrequency = pulseIn(M_SensorOut, LOW);
  
  // Remaping the value of the GREEN (G) frequency from 0 to 255
  
  RgreenColor = map(RgreenFrequency, 0, 80, 0, 255);
  LgreenColor = map(LgreenFrequency, 0, 80, 0, 255);
  MgreenColor = map(MgreenFrequency, 0, 80, 0, 255);

  // Setting BLUE (B) filtered photodiodes to be read
  
  digitalWrite(RS2,LOW);
  digitalWrite(RS3,HIGH);
  digitalWrite(LS2,LOW);
  digitalWrite(LS3,HIGH);
  digitalWrite(MS2,LOW);
  digitalWrite(MS3,HIGH);
  
  // Reading the output frequency
  
  RblueFrequency = pulseIn(R_SensorOut, LOW);
  LblueFrequency = pulseIn(L_SensorOut, LOW);
  MblueFrequency = pulseIn(M_SensorOut, LOW);
  
  // Remaping the value of the BLUE (B) frequency from 0 to 255
  
  RblueColor = map(RblueFrequency, 0, 100, 0, 255);
  LblueColor = map(LblueFrequency, 0, 100, 0, 255);
  MblueColor = map(MblueFrequency, 0, 100, 0, 255);

  if((LblueFrequency >= 104 && LblueFrequency <= 190) && (LredFrequency >= 145 && LredFrequency <= 295) && (LgreenFrequency >= 120 && LgreenFrequency <= 190)){
    Lcolor = 1; // green detected in the left sensor is the right RGB color sensor on the actual vehicle
  }
  else if((LblueFrequency >= 94 && LblueFrequency <= 175) && (LredFrequency >= 30 && LredFrequency <= 80) && (LgreenFrequency >= 128 && LgreenFrequency <= 250)){ //have upper and lower bounds for all colors
    Lcolor = 2; // red detected in the left sensor
  }
    else if((LblueFrequency >= 40 && LblueFrequency <= 65) && (LredFrequency >= 114 && LredFrequency <= 230) && (LgreenFrequency >= 76 && LgreenFrequency <= 125)){
    Lcolor = 3; // blue detected in the left sensor
  }
  else if((LblueFrequency >= 23 && LblueFrequency <= 38 ) && (LredFrequency >= 23 && LredFrequency <= 39) && (LgreenFrequency >= 26 && LgreenFrequency <= 42)){
    Lcolor = 0; // white is detected
  }
  else if((LblueFrequency >= 110 && LblueFrequency <= 255) && (LredFrequency >= 115 && LredFrequency <= 255) && (LgreenFrequency >= 135 && LgreenFrequency <= 300)){
    Lcolor = 4; // black is detected
  }

  if((RblueFrequency >= 84 && RblueFrequency <= 102) && (RredFrequency >= 125 && RredFrequency <= 146) && (RgreenFrequency >= 75 && RgreenFrequency <= 93 )){
    Rcolor = 1; // green is detected
  }
  else if((RblueFrequency >= 80 && RblueFrequency <= 96) && (RredFrequency >= 27 && RredFrequency <= 44) && (RgreenFrequency >= 105 && RgreenFrequency <= 120)){
    Rcolor = 2; // red is detected
  }
  else if((RblueFrequency >= 74 && RblueFrequency <= 88) && (RredFrequency >= 126 && RredFrequency <= 143) && (RgreenFrequency >= 124 && RgreenFrequency <= 138 )){
    Rcolor = 3; // blue is detected
  }

  else if((RblueFrequency >= 20 && RblueFrequency <= 24) && (RredFrequency >= 22 && RredFrequency <= 27) && (RgreenFrequency >= 25 && RgreenFrequency <= 30)){
    Rcolor = 0; // white is detected
  }
  else if((RblueFrequency >= 110 && RblueFrequency <= 200) && (RredFrequency >= 110 && RredFrequency <= 255) && (RgreenFrequency >= 120 && RgreenFrequency <= 255)){
    Rcolor = 4; // black is detected
  }

  if((MblueFrequency >= 194 && MblueFrequency <= 340) && (MredFrequency >= 121 && MredFrequency <= 310) && (MgreenFrequency >= 175 && MgreenFrequency <= 410 )){
    Mcolor = 1; // green is detected
  }
  else if((MblueFrequency >= 120 && MblueFrequency <= 210) && (MredFrequency >= 27 && MredFrequency <= 134) && (MgreenFrequency >= 115 && MgreenFrequency <= 220)){
    Mcolor = 2; // red is detected
  }
  else if((MblueFrequency >= 274 && MblueFrequency <= 340) && (MredFrequency >= 226 && MredFrequency <= 300) && (MgreenFrequency >= 324 && MgreenFrequency <= 400)){
    Mcolor = 3; // blue is detected
  }

  else if((MblueFrequency >= 20 && MblueFrequency <= 1) && (MredFrequency >= 22 && MredFrequency <= 27) && (MgreenFrequency >= 25 && MgreenFrequency <= 245)){
    Mcolor = 0; // white is detected
  }
  else if((MblueFrequency >= 110 && MblueFrequency <= 215) && (MredFrequency >= 110 && MredFrequency <= 135) && (MgreenFrequency >= 120 && MgreenFrequency <= 215)){
    Mcolor = 4; // black is detected
  }
}

// VEHICLE MOVEMENT SUBROUTINES

//////////////////////////////////////////////////////////////////////////////
// Stop():  Stop vehicle movement by stopping wheel rotation.
//////////////////////////////////////////////////////////////////////////////

void Stop() 
{
  // The PWM value is 0 for a modulation of 0%
  
  analogWrite(EnableENA, 0);
  analogWrite(EnableENB, 0);
  analogWrite(EnableENC, 0);
  analogWrite(EnableEND, 0);
  
  // Turn off all motors

  digitalWrite(A_in1, LOW);
  digitalWrite(A_in2, LOW);
  digitalWrite(B_in3, LOW);
  digitalWrite(B_in4, LOW); 
  digitalWrite(C_in5, LOW);
  digitalWrite(C_in6, LOW);
  digitalWrite(D_in7, LOW);
  digitalWrite(D_in8, LOW); 
}

//////////////////////////////////////////////////////////////////////////////
// MovFWD( int speed ):  Right and Left wheels will rotate in the CW direction.
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void MovFWD( int speed )
{
  int  PWMval = 255*speed/100;
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableENA, PWMval);
  analogWrite(EnableENB, PWMval);
  analogWrite(EnableENC, PWMval);
  analogWrite(EnableEND, PWMval);

  // Rotate Right and Left wheels clockwise (CW) 

  digitalWrite(A_in1, LOW);
  digitalWrite(A_in2, HIGH);
  digitalWrite(B_in3, LOW);
  digitalWrite(B_in4, HIGH);
  digitalWrite(C_in5, HIGH);
  digitalWrite(C_in6, LOW);
  digitalWrite(D_in7, HIGH);
  digitalWrite(D_in8, LOW); 
}

//////////////////////////////////////////////////////////////////////////////
// MovBKWD( int speed ):  Right and left wheels will rotate in the CCW direction.
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void MovBKWD( int speed )
{
  int  PWMval = 255*speed/100;
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableENA, PWMval);
  analogWrite(EnableENB, PWMval);
   analogWrite(EnableENC, PWMval);
  analogWrite(EnableEND, PWMval);


  // Rotate Right and Left wheels counter-clockwise (CCW)  

  digitalWrite(A_in1, HIGH);
  digitalWrite(A_in2, LOW);
  digitalWrite(B_in3, HIGH);
  digitalWrite(B_in4, LOW);
  digitalWrite(C_in5, LOW);
  digitalWrite(C_in6, HIGH);
  digitalWrite(D_in7, LOW);
  digitalWrite(D_in8, HIGH); 
}

//////////////////////////////////////////////////////////////////////////////
// RotateRt( int speed ):  
//    CW rotation of vehicle -- Left wheel rotates forward; Right wheel
//    rotates backwards.  Each time this subroutine is called, causes the
//    vehicle to rotate in time-step increments.  The predefined value for
//    "TimeStep_ms" is the number of milli-seconds of rotation.  The angle
//    the vehicle will be facing is determined by how fast the vehicle is 
//    rotating and the duration of rotation (how long the vehicle has been)
//    rotating.
//
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void RotateRt( int speed )
{
  int  PWMval = 255*speed/100;
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableENA, PWMval);
  analogWrite(EnableENB, PWMval);
  analogWrite(EnableENC, PWMval);
  analogWrite(EnableEND, PWMval);
  // Rotate left wheel anti-clockwise and right wheel in clockwise direction - Spin anti-clockwise/Turn left
  digitalWrite(A_in1, LOW);
	digitalWrite(A_in2, HIGH);
	digitalWrite(B_in3, LOW);
	digitalWrite(B_in4, HIGH);
  digitalWrite(C_in5, LOW);
	digitalWrite(C_in6, HIGH);
	digitalWrite(D_in7, LOW);
	digitalWrite(D_in8, HIGH);
  delay(TimeStep_ms);
}

//////////////////////////////////////////////////////////////////////////////
// RotateLft( int speed ):  
//    CCW rotation of vehicle -- Left wheel rotates backward; Right wheel
//    rotates forward.  Each time this subroutine is called, causes the
//    vehicle to rotate in time-step increments.  The predefined value for
//    "TimeStep_ms" is the number of milli-seconds of rotation.  The angle
//    the vehicle will be facing is determined by how fast the vehicle is 
//    rotating and the duration of rotation (how long the vehicle has been
//    rotating).
//
//    "speed" parameter {0 - 100} is the modulation percentage of the PWM signal.
//////////////////////////////////////////////////////////////////////////////

void RotateLft( int speed)
{
  int  PWMval = 255*speed/100;
  // The PWMval varies between 0 and 255
  
  analogWrite(EnableENA, PWMval);
  analogWrite(EnableENB, PWMval);
  analogWrite(EnableENC, PWMval);
  analogWrite(EnableEND, PWMval);
  // Rotate Left wheel backward; Rotate Right wheel forward  
  
  digitalWrite(A_in1, HIGH);
	digitalWrite(A_in2, LOW);
	digitalWrite(B_in3, HIGH);
	digitalWrite(B_in4, LOW);
  digitalWrite(C_in5, HIGH);
	digitalWrite(C_in6, LOW);
	digitalWrite(D_in7, HIGH);
	digitalWrite(D_in8, LOW);
  delay(TimeStep_ms); 
}

// void pickup(){
//   ReadColorSensors();
//   if(Mcolor == 1 || Mcolor == 2 || Mcolor == 3){
    
//   }
// }


