#define echoPin 17
#define trigPin 18
#define RS0 32
#define RS1 31
#define RS2 30
#define RS3 29
#define RSOut 28
#define LS0 33
#define LS1 34
#define LS2 35
#define LS3 36
#define LSOut 37
#define left 1
#define right 2
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

int RredFreq = 0;
int RgreenFreq = 0;
int RblueFreq = 0;
int LredFreq = 0;
int LgreenFreq = 0;
int LblueFreq = 0;
int RredCol = 0;
int RgreenCol = 0;
int RblueCol = 0;
int LredCol = 0;
int LgreenCol = 0;
int LblueCol = 0;
int RCol = 0;
int LCol = 0;
// Initialization for ultrasonic sensor
long duration;
int distance;
int direction = right;
// bool detect = true;
bool gap = false;
void setup() {
  pinMode(RS0, OUTPUT);
  pinMode(RS1, OUTPUT);
  pinMode(RS2, OUTPUT);
  pinMode(RS3, OUTPUT);
  pinMode(LS0, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(RSOut, INPUT);
  pinMode(LSOut, INPUT);
  digitalWrite(RS0,HIGH);
  digitalWrite(RS1,LOW);
  digitalWrite(LS0,HIGH);
  digitalWrite(LS1,LOW);
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
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  digitalWrite(in5, LOW);
  digitalWrite(in6, LOW);
  digitalWrite(in7, LOW);
  digitalWrite(in8, LOW);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  readUltrasonic(); // read the Ultrasonic sensors
  readColorSensors(); // read the rgb color sensors
  if(RCol == 4){ //if the right sensor detects black
      stop(); // stop
      delay(100); // delay for 0.01 seconds
      direction = left; // set direction to left
    }else if(LCol == 4){ // if the left sensor detects black 
      stop(); // stop
      delay(100); // delay for 0.01 seconds
      direction = right; // set direction to right
    }
  if(direction == right){
      if (distance <= 15) { // if the distance of the ultrasonic is less than equal to 15
        movBW(100); // move backward
      }else if(distance >= 20 && distance < 24){ // if the distance is greater than 20 and the distance is less than 24
        movFW(100); // move forward
      }else if(distance >= 24){ // if the ultrasonic distance is greater than equal to 24
        movRT(100); // move right
        delay(700); // delay for 0.07 seconds
        while(distance >= 24){ // while distance is greater and equal to 24 read ultrasonic sensor and move forward
          readUltrasonic();
          readColorSensors();
          if(RCol == 3 || LCol == 3){
            stop();
          } else {
            movFW(100);            
          }
        }
      }
    
      else{ // otherwise move right
        movRT(100);
      }
  } else {
      if (distance <= 15) { // distance is less than equal to 15
        movBW(100);
      }else if(distance >= 20 && distance < 24){ // if distance is greater than equal to 20 and distance is less than 24
        movFW(100); // move Forward
        
      }else if(distance >= 24){ // distance is greater than equal to 24
        movLFT(100); // move left
        delay(700); // delay for 0.07 seconds
        while(distance >= 24){ // distance is greater than equal to 24 read ultrasonic and move forward
          readUltrasonic();
          readColorSensors();
          if(RCol == 3 || LCol == 3){
            finish();
          } else {
            movFW(100);            
          }
        }
      }
      else{ // otherwise move left
        movLFT(100);
      }
  }         
   

}

void readColorSensors(){
   // Setting BLUE (B) filtered photodiodes to be read
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
   //read black color for all sensors
  if((LblueCol <= -110) && (LredCol <= -120) && (LgreenCol <= -130))
  {
      // Serial.println("Left: Black");
      LCol = 0;
  }
  if((RblueCol <= -180) && (RredCol <= -175) && (RgreenCol <= -178))
  {
    //  Serial.println("Right: Black");
      RCol = 0;
  }

   if((LblueCol >= 240) && (LredCol >= 240) && (LgreenCol >= 240))
  {
    //  Serial.println("Left: White");
      LCol = 4;
  }
  if((RblueCol >= 240) && (RredCol >= 240) && (RgreenCol >= 240))
  {
    //  Serial.println("Right: White");
      RCol = 4;
  }
  if((LblueCol >= 150 && LblueCol <= 220) && (LredCol <= 30) && (LgreenCol <= 160))
  {
    //  Serial.println("Left: Blue");
      LCol = 2;
  }
  if((RblueCol >= 150 && RblueCol <= 250) && (RredCol <= 40) && (RgreenCol <= 150))
  {
    //  Serial.println("Right: Blue");
      RCol = 2;
  }
    if((LblueCol <= 60 && LblueCol >= 10) && (LredCol <= 10 && LredCol >= -20) && (LgreenCol >= 70 && LgreenCol <= 100))
  {
    Serial.println("Left: Green");
      LCol = 3;
  }
  if((RblueCol <= -60 && RblueCol >= -150 ) && (RredCol <= -70 && RredCol >= -165) && (RgreenCol <= -10 && RgreenCol >= -30))
  {
     Serial.println("Right: Green");
      RCol = 3;
  }


}
void movFW(int speed)
{
  int PWMval = 255*speed/100;
  // move forward with 60% speed - For PWM values varies between 0 (no speed) and 255 (full speed)
	analogWrite(enA, PWMval);
	analogWrite(enB, PWMval);
  analogWrite(enC, PWMval);
	analogWrite(enD, PWMval);
  //Rotate left and right wheels in clockwise direction - Move forward
  // delay(500);
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  digitalWrite(in5, HIGH);
	digitalWrite(in6, LOW);
	digitalWrite(in7, HIGH);
	digitalWrite(in8, LOW);
}
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

void readUltrasonic() {
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
  //Serial.println(distance);
  delay(100);
}
void finish(){
  stop();
}

