// Digital input pins go from 2-5
int buttonPin1 = 10;
int buttonPin2 = 8;
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(28, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  
}

void loop() {
  // put your main code here, to run repeatedly:
 
  
  int up = digitalRead(28);
  int down = digitalRead(30);
  int left = digitalRead(26);
  int right = digitalRead(34);

  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  if (!up) Serial.println("Up is pressed");
  if (!down) Serial.println("Down is pressed");
  if (!left) Serial.println("Left is pressed");
  if (!right) Serial.println("Right is pressed");

  if(!buttonState1) Serial.println("ZUP");
  if(!buttonState2) Serial.println("ZDOWN");

  delay(300);
}
