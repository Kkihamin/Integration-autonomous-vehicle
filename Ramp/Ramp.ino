// Motor A pin connections
int enA = 1;
int A_in1 = 2;
int A_in2 = 3;
// Motor B pin connections
int enB = 4;
int B_in3 = 5;
int B_in4 = 6;
// Motor C pin connections
int enC = 7;
int C_in5 = 8;
int C_in6 = 9;
// Motor D pin connections
int enD = 10;
int D_in7 = 11;
int D_in8 = 12;



void setup() {
 	// Set all motor controller pins as outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
  pinMode(enC, OUTPUT);
  pinMode(enD, OUTPUT);
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
  // initialize serial communication for debugging
  Serial.begin(115200);
}

void loop() {
  
  stop();
  delay(1000);
  movFW(100);
  delay(5000);
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
  digitalWrite(A_in1, LOW);
	digitalWrite(A_in2, HIGH);
	digitalWrite(B_in3, LOW);
	digitalWrite(B_in4, HIGH);
  digitalWrite(C_in5, HIGH);
	digitalWrite(C_in6, LOW);
	digitalWrite(D_in7, HIGH);
	digitalWrite(D_in8, LOW);
}

void stop()
{
  // move forward with 0% speed - For PWM value 0
	analogWrite(enA, 0);
	analogWrite(enB, 0);
  analogWrite(enC, 0);
	analogWrite(enD, 0);
   
	// Turn off all the motors
	digitalWrite(A_in1, LOW);
	digitalWrite(A_in2, LOW);
	digitalWrite(B_in3, LOW);
	digitalWrite(B_in4, LOW);
  digitalWrite(C_in5, LOW);
  digitalWrite(C_in6, LOW);
  digitalWrite(D_in7, LOW);
  digitalWrite(D_in8, LOW);  
}






