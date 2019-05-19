byte FW_PWM_PIN = 3;
byte LR_PWM_PIN = 5;
byte IN1 = 2;
byte IN2 = 4;
byte IN3 = 6;
byte IN4 = 7;
byte airIN = 11;
byte airOUT = 13;
byte autoPIN = 14;
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
  Serial.begin(115200);
}

void loop() {
    // if auto button pressed
  int autoValue = digitalRead(autoPIN);
  if (autoValue == HIGH) {
    Serial.println("Starting autonomous phase.");
    auto();
  }

} 


void auto() {
  // open claw & blow fan for first few balls
  digitalWrite(airOUT, HIGH);
  delay(1000);
  digitalWrite(airOUT,LOW); 
  
  // travel back for 3 seconds
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(RIGHT_OUTPUT_PIN, 120);
  analogWrite(LEFT_OUTPUT_PIN, 120);
  delay(3000);
  
  // turn to left and knock down pillars 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(LEFT_OUTPUT_PIN, 50);
  analogWrite(RIGHT_OUTPUT_PIN, 70);
  delay(1000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(RIGHT_OUTPUT_PIN, 100);
  analogWrite(LEFT_OUTPUT_PIN, 100);
  delay(3000);

  // reverse and knock rest of pillars
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(RIGHT_OUTPUT_PIN, 120);
  analogWrite(LEFT_OUTPUT_PIN, 120);
  delay(5000);
}
