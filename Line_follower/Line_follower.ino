//Line follower GPIOs
const uint8_t out1 = A1;
const uint8_t out2 = A2;
const uint8_t out3 = A3;
const uint8_t out4 = A4;
const uint8_t out5 = A5;
int sensors[] = {out1, out2, out3, out4, out5};

//Motor driver GPIOs -- TB6612FNG
const uint8_t PWMA = 11;
const uint8_t PWMB = 10;
const uint8_t AI2 = 9;
const uint8_t AI1 = 8;
const int OffsetA = 1;    //change to -1 to reverse direction
const uint8_t BI1 = 7;
const uint8_t BI2 = 6;
const int OffsetB = 1;    //change to -1 to reverse direction

//global variables
int digital_input[5] = {0, 0, 0, 0, 0};
int analog_input = 1000; 

//Functions
void GPIO_configuration();
void sensor_read();
void motor_left_speed (int speedL);
void motor_right_speed (int speedR);
void motor_stop();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  GPIO_configuration();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor_read();
  for (int i=0; i<=4; i++) 
  {
    Serial.print(digital_input[i]);
    Serial.print(", ");
  }
  Serial.println();
  delay(10);
}

void GPIO_configuration()
{
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
{
  for (int i=0; i<=4; i++) 
  {
    digital_input[i] = digitalRead (sensors[i]);
  }
}

