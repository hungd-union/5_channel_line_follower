//Line follower GPIOs
const uint8_t out1 = A1;
const uint8_t out2 = A2;
const uint8_t out3 = A3;
const uint8_t out4 = A4;
const uint8_t out5 = A5;
int sensors[] = {out1, out2, out3, out4, out5};
int digital_input[5] = {0, 0, 0, 0, 0};
int analog_input = 0; 

//Motor driver GPIOs -- TB6612FNG
const uint8_t PWMA = 11;
const uint8_t PWMB = 10;
const uint8_t AI2 = 9;
const uint8_t AI1 = 8;
const uint8_t BI1 = 7;
const uint8_t BI2 = 6;

//global variables
int default_speed = 100;      //Change the default speed for the robot throughout the maze
int speedL = default_speed;
int speedR = default_speed;
const int OffsetA = 1;        //change to -1 to reverse direction
const int OffsetB = 1;        //change to -1 to reverse direction

//Functions
void GPIO_configuration();              //Configuring GPIO Modes
void sensor_read();                     //Read inputs from the Line follower arrays
void motor_left_speed (int speedL);     //Control left motor -- connected to channel A
void motor_right_speed (int speedR);    //Control right motor -- connected to channel B
void motors_stop();                     //Stop the motor -- High impedance mode instead of short brake

//Setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GPIO_configuration();
  sensor_read();
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor_read();
  
  delay(5);
}

void GPIO_configuration()
{ //Configuring GPIO Modes
  pinMode (out1, INPUT_PULLUP);
  pinMode (out2, INPUT_PULLUP);
  pinMode (out3, INPUT_PULLUP);
  pinMode (out4, INPUT_PULLUP);
  pinMode (out5, INPUT_PULLUP);

  pinMode (PWMA, OUTPUT);
  pinMode (PWMB, OUTPUT);
  pinMode (AI1, OUTPUT);
  pinMode (AI2, OUTPUT);
  pinMode (BI1, OUTPUT);
  pinMode (BI2, OUTPUT);
  pinMode (LED_BUILTIN, OUTPUT);
}

void sensor_read()
{ //Read inputs from the Line follower arrays
  for (int i=0; i<=4; i++) 
  {
    digital_input[i] = digitalRead (sensors[i]);
  }
}

void motor_left_speed (int speedL) {
  int input1, input2;
  if (OffsetA == 1) {input1 = AI1, input2 = AI2;}
  else {input1 = AI2, input2 = AI1;}

  if (speedL == 0) {
    digitalWrite (input1, HIGH);
    digitalWrite (input2, HIGH);
  }
  else if (speedL > 0) {
    digitalWrite (input1, HIGH);
    digitalWrite (input2, LOW);
  }
  else {
    digitalWrite (input1, LOW);
    digitalWrite (input2, HIGH);
  }

  analogWrite (PWMA, abs(speedL));
  return;
}

void motor_right_speed (int speedR) {
  int input1, input2;
  if (OffsetA == 1) {input1 = BI1, input2 = BI2;}
  else {input1 = BI2, input2 = BI1;}

  if (speedL == 0) {
    digitalWrite (input1, HIGH);
    digitalWrite (input2, HIGH);
  }
  else if (speedL > 0) {
    digitalWrite (input1, HIGH);
    digitalWrite (input2, LOW);
  }
  else {
    digitalWrite (input1, LOW);
    digitalWrite (input2, HIGH);
  }

  analogWrite (PWMB, abs(speedR));
  return;
}

void motors_stop() {
  digitalWrite (AI1, LOW);
  digitalWrite (AI2, LOW);
  digitalWrite (BI1, LOW);
  digitalWrite (BI2, LOW);
}

