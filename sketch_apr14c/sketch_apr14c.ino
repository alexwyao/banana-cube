// Digital input pins go from 2-5

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  for (int i = 2; i <= 5; ++i) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  int up = digitalRead(2);
  int down = digitalRead(3);
  int left = digitalRead(4);
  int right = digitalRead(5);

  if (!up) Serial.println("Up is pressed");
  if (!down) Serial.println("Down is pressed");
  if (!left) Serial.println("Left is pressed");
  if (!right) Serial.println("Right is pressed");

  delay(100);
}
