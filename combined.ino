byte FW_PWM_PIN = 3;
byte LR_PWM_PIN = 5;
byte IN1 = 2;
byte IN2 = 4;
byte IN3 = 6;
byte IN4 = 7;
byte airIN = 11;
byte pSource = 12;
byte airOUT = 13;
byte autoPIN = 8;
byte LEFT_OUTPUT_PIN = 9;
byte RIGHT_OUTPUT_PIN = 10;
 
int FW_pwm_value;
int LR_pwm_value;

void setup() {
  pinMode(FW_PWM_PIN, INPUT);
  pinMode(LR_PWM_PIN, INPUT);
  pinMode(LEFT_OUTPUT_PIN, OUTPUT);
  pinMode(RIGHT_OUTPUT_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(airOUT, OUTPUT);
  pinMode(airIN, INPUT);
  pinMode(autoPIN, INPUT);
  pinMode(pSource, OUTPUT);
  digitalWrite(pSource, HIGH);
  Serial.begin(115200);
  int autoValue = digitalRead(autoPIN);
  if (autoValue == HIGH) {
    Serial.println("Starting autonomous phase.");
    autono();
    digitalWrite(pSource, LOW);
    digitalWrite(autoValue, LOW);
  }
}

int pwm_check(int input){
  if (input > 255){
    return 255;
  }
  if (input < 0){
    return 0; 
  }
  else{
    return input;
  }
}

void loop() {
  // if auto button pressed
  double air_high_value = pulseIn(airIN, HIGH) * 100;
  double air_low_value = pulseIn(airIN, LOW);
  double air_period = air_high_value + air_low_value;
  double air_output = map(air_period, 0, 180000, 0, 100);
  air_output = pwm_check(air_output);
  
  if (air_output > 100) {
    digitalWrite(airOUT, HIGH);
    Serial.println("Turn on fan.");
  } else {
    digitalWrite(airOUT, LOW);
    Serial.println("Turn off fan.");
  }
  
  ///forward and back control, FLICK THE SWITCH BACKWARDS
  double fw_high_pwm_value = pulseIn(FW_PWM_PIN, HIGH)*100;
  double fw_low_pwm_value = pulseIn(FW_PWM_PIN, LOW);
  double fw_period = fw_high_pwm_value + fw_low_pwm_value;
  double fw_output = map(fw_period, 136000, 199000, 0, 100); 
  fw_output = pwm_check(fw_output);
  
  ///LR control, FLICK THE SWITCH BACAKWARDS
  double lr_high_pwm_value = pulseIn(LR_PWM_PIN, HIGH)*100;
  double lr_low_pwm_value = pulseIn(LR_PWM_PIN, LOW);
  double lr_period = lr_high_pwm_value + lr_low_pwm_value;
  double lr_output = map(lr_period, 134000, 207000, 0, 50); 
  lr_output = pwm_check(lr_output);

  if (fw_output > 60){ ///forwards!
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    analogWrite(RIGHT_OUTPUT_PIN, fw_output+lr_output-25);
    analogWrite(LEFT_OUTPUT_PIN, fw_output-lr_output+40);
    Serial.println("Forward");
  }
  else if (fw_output < 40){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(RIGHT_OUTPUT_PIN, 100 - fw_output+lr_output-25);
    analogWrite(LEFT_OUTPUT_PIN, 100 - fw_output-lr_output+40);
    Serial.println("Reverse"); 

  }
  else {
    if (lr_output >30) { ///round turn right
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(LEFT_OUTPUT_PIN, 25+lr_output);
      analogWrite(RIGHT_OUTPUT_PIN, 25+lr_output);
    }
    else if (lr_output < 20){ ///round turn left
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(LEFT_OUTPUT_PIN, 75-lr_output);
      analogWrite(RIGHT_OUTPUT_PIN, 75-lr_output);
    }
    else {
    analogWrite(LEFT_OUTPUT_PIN, 0);
    analogWrite(RIGHT_OUTPUT_PIN, 0);
    }
  }
} 


void autono(){
  // open claw & blow fan for first few balls
//  digitalWrite(airOUT, HIGH);
//  delay(1000);
//  digitalWrite(airOUT,LOW); 
  
  // travel back for 3 seconds
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(RIGHT_OUTPUT_PIN, 100); // left needs to be greater than right output pin due to hardware issue
  analogWrite(LEFT_OUTPUT_PIN, 115);
  delay(3000);
  
  // turn to left and knock down pillars 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(LEFT_OUTPUT_PIN, 50);
  analogWrite(RIGHT_OUTPUT_PIN, 54);
  delay(400);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(RIGHT_OUTPUT_PIN, 100);
  analogWrite(LEFT_OUTPUT_PIN, 115);
  delay(3000);

  // reverse and knock rest of pillars
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(RIGHT_OUTPUT_PIN, 100);
  analogWrite(LEFT_OUTPUT_PIN, 115);
  delay(5000);
}
