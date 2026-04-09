//Line follower GPIOs
const uint8_t out1 = A1;
const uint8_t out2 = A2;
const uint8_t out3 = A3;
const uint8_t out4 = A4;
const uint8_t out5 = A5;
int sensors[] = {out1, out2, out3, out4, out5};
float digital_input[5] = {0, 0, 0, 0, 0};
float weight[5] = {75, 40, 0, -40, -75};
float analog_input = 0; 
const float target_input = 0;

//Motor driver GPIOs -- TB6612FNG
const uint8_t PWMA = 11;
const uint8_t PWMB = 10;
const uint8_t AI2 = 9;
const uint8_t AI1 = 8;
const uint8_t BI1 = 7;
const uint8_t BI2 = 6;

//PID Constants
float kP = 1.15;
float kI = 0.00;
float kD = 0.017;
float error = 0;
float past_error = 0;
float diff_error = 0;
float int_error = 0;

//global variables
int default_speed = 90;      //Change the default speed (0-256) for the robot throughout the maze
float  speedL = default_speed;
float  speedR = default_speed*0.975;
const int OffsetA = -1;        //change to -1 to reverse direction
const int OffsetB = -1;        //change to -1 to reverse direction

//Functions
void GPIO_configuration();              //Configuring GPIO Modes
void sensor_read();                     //Read inputs from the Line follower arrays
void motor_left_speed (int speedL);     //Control left motor -- connected to channel A
void motor_right_speed (int speedR);    //Control right motor -- connected to channel B
void motors_stop();                     //Stop the motor -- High impedance mode instead of short brake
bool zero_sensor();


//Setup
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GPIO_configuration();
  sensor_read();
  delay(500);
}

float current_time = 0;
float past_time = 0;
float time_elapsed;
float correction = 0;

//Main function in loop
void loop() {
  // put your main code here, to run repeatedly:
  sensor_read();
  current_time = millis();
  float time_elapsed = current_time - past_time;

  if (time_elapsed >= 4 && !zero_sensor()) {
    past_time = current_time;
    error =  analog_input - target_input;
    diff_error = (error - past_error)/ (time_elapsed/1000);
    int_error += error * time_elapsed/1000;
    past_error = error;
  
    correction = (kP * error + kI * int_error + kD * diff_error) * default_speed/100;
    motor_left_speed(speedL - correction);
    motor_right_speed(speedR + correction);
  }

  if (zero_sensor()) {
    motors_stop();
    while (1) {delay(1000);}
    }
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
  analog_input = 0;
  for (int i=0; i<=4; i++) 
  {
    digital_input[i] = digitalRead (sensors[i]);
    analog_input += (1-digital_input[i]) * weight[i];
  }
}

void motor_left_speed (int speedL) {
  int input1, input2;
  if (OffsetA == 1) {input1 = AI1, input2 = AI2;}
  else {input1 = AI2, input2 = AI1;}

  if (speedL == 0) {
    digitalWrite (input1, LOW);
    digitalWrite (input2, LOW);
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
  if (OffsetB == 1) {input1 = BI1, input2 = BI2;}
  else {input1 = BI2, input2 = BI1;}

  if (speedR == 0) {
    digitalWrite (input1, LOW);
    digitalWrite (input2, LOW);
  }
  else if (speedR > 0) {
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
  digitalWrite (AI1, HIGH);
  digitalWrite (AI2, HIGH);
  digitalWrite (BI1, HIGH);
  digitalWrite (BI2, HIGH);
}

bool zero_sensor() {
  bool check = true;
  for (int i=0; i<=4; i++) 
  {
    if (digital_input[i] == 1) check = false; 
  }
  return check;
}

