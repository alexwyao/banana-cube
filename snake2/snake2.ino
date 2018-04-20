class Coord {
public:
  Coord () {
    x = 255;
    y = 255; 
    z = 255;
  }
  Coord (byte pX, byte pY, byte pZ) {
    x = pX;
    y = pY; 
    z = pZ;
  }
  byte x;
  byte y;
  byte z;
};

byte xSpeed = 1;
byte ySpeed = 0;
byte zSpeed = 0;
byte snakeLength = 1; 
int initialSpeed = 600;
Coord snakePosition, foodPosition;
Coord * snakeSegments = new Coord[64];
long mspm = 1000, lastMove = 0; //milliseconds per move
boolean gameRunning = true;


void(* resetFunc) (void) = 0; //declare reset function @ address 0

int buttonPin1 = 10;
int buttonPin2 = 8;

int ledButDown = 22;
int ledButUp = 24;

void setup() {
  // put your setup code here, to run once:

  //LEDS
  Serial.begin(9600);

  randomSeed(analogRead(0));

  for (int i = 23; i <= 53; i += 2) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }

  for (int i = 2; i <= 5; ++i) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  pinMode(28, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);

  pinMode(22, OUTPUT);
  pinMode(24, OUTPUT);

  
  //BUTTONS --> Change pin values!

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  snakePosition.x = random(4);
  snakePosition.y = random(4);
  snakePosition.z = random(4);
  //SETUP 
  snakeSegments[0].x = snakePosition.x;
  snakeSegments[0].y = snakePosition.y;
  snakeSegments[0].z = snakePosition.z;
  
  zeroLedArray(); 
  spawnFood();
  digitalWrite(ledButDown, HIGH);
  digitalWrite(ledButUp, HIGH);

  while (digitalRead(buttonPin1) && digitalRead(buttonPin2)) {delay(100);}


  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      //if ( i == 1 && j == 1 || i == 1 && j == 2 ||
      //i == 2 && j == 1 || i == 2 && j == 2) continue;
      for (int k = 0; k < 4; ++k) {
        turnOn(i,j,k);
      }
      delay(70);
      for (int k = 0; k < 4; ++k) {
        turnOff(i,j,k);
      }
    }
  }
  
  zeroLedArray();
  digitalWrite(ledButDown, LOW);
  digitalWrite(ledButUp, LOW);
  delay(500);
  digitalWrite(ledButDown, HIGH);
  digitalWrite(ledButUp, HIGH);
}

void loop() {

  
  
  boolean moved = false;
  for(int i = 0; i < 300; i++){ 
    if (!(i % 25)) updateDirection();
    for(int j = 0; j < snakeLength; j++){
      //toggle(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z, 1);
      /*if (j == snakeLength - 1) {
        if ((i > 75 && i < 150) || i > 225) {
          turnOn(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
          delayMicroseconds(500-130*sqrt(snakeLength-1));
      
          turnOff(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
        }
        continue;
      }*/
      turnOn(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
      delayMicroseconds(max(500-130*sqrt(snakeLength-1), 50));
      
      turnOff(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
    }
    if ((i > 75 && i < 150) || i > 225) {
    turnOn(foodPosition.x, foodPosition.y, foodPosition.z);

    delayMicroseconds(max(1000-260*sqrt(snakeLength-1), 100));
    
    turnOff(foodPosition.x, foodPosition.y, foodPosition.z);
    //Serial.println(i);
    } else {
      delayMicroseconds(max(1000-260*sqrt(snakeLength-1), 100));
    }
  }
  runGame();
}

void runGame(){

  
  if (/*mspm + lastMove < millis() && */gameRunning) { // time to move the snake
    //lastMove = millis();
    //mspm = analogRead(mspmPin);
    //totalRefreshes++;

    //Update snake position
    if(snakePosition.x + xSpeed == -1)
    {
      snakePosition.x = 3;
    }
    else
    {
      snakePosition.x = (snakePosition.x + xSpeed) % 4;
    }
    if(snakePosition.y + ySpeed == -1)
    {
      snakePosition.y = 3;
    }
    else
    {
      snakePosition.y = (snakePosition.y + ySpeed) % 4;
    }
    if(snakePosition.z + zSpeed == -1)
    {
      snakePosition.z = 3;
    }
    else
    {
      snakePosition.z = (snakePosition.z + zSpeed) % 4;
    }

    if (zSpeed == 0) {
      digitalWrite(ledButUp, HIGH);
      digitalWrite(ledButDown, HIGH);
    }
    else if (zSpeed == 1) {
      //Serial.println(zSpeed);
      digitalWrite(ledButUp, LOW);
      digitalWrite(ledButDown, HIGH);
    }
    else if (zSpeed == 255) {
      //Serial.println();
      digitalWrite(ledButUp, HIGH);
      digitalWrite(ledButDown, LOW);
    }

    // check if snake eats itself
    if (segmentExists(snakePosition.x, snakePosition.y, snakePosition.z) && snakeLength!=4){
      Serial.println(snakeLength);
      gameRunning = false; 
    }

    // check if snake eats food
    if (snakePosition.x != foodPosition.x || snakePosition.y != foodPosition.y || snakePosition.z != foodPosition.z) 
      removeLastSegment();
    else 
      spawnFood();

    addNewSegment();

    zeroLedArray();
    //refreshLedArray();

    // Serial.println(String(snakeDirection) + " (" + snakePosition.x + "|" + snakePosition.y + ")");
  }

//  if (gameRunning) {  
//    // refresh led matrix
//    for (int i = 0; i < 8; i++) {
//      row.setAll(led[i]);
//      col.set(i, LOW);
//      delayMicroseconds(50);
//      col.setAllHigh();
//    }
//  }

  // check if game is over
  if (!gameRunning) {
    //Serial.println("DEAD");
    zeroLedArray();
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 4; ++k) {
          turnOn(i, j, k);
        }
      }
    }
  
    delay(500);
  
    for (int k = 3; k >= 0; --k) {
      delay(200);
      digitalWrite(2 + k, LOW);
    }
    zeroLedArray();
    delay(500);
    for (int tf = 0; tf < 500; ++tf) {
    for (int sc = 0; sc < snakeLength; ++sc) {
      turnOn( (sc % 16) % 4, (sc % 16) / 4, sc / 16);
      delayMicroseconds(800);
      turnOff( (sc % 16) % 4, (sc % 16) / 4, sc / 16);
    }
    }
    delay(500);
    zeroLedArray();
    resetFunc();
//    if(updateDirection()){
//      reset();
//    }
  }
}


void reset(){
  
  gameRunning = true;

    snakePosition.x = random(4);
    snakePosition.y = random(4);
    snakePosition.z = random(4);

  xSpeed = 1;
  ySpeed = 0;
  zSpeed = 0;
  snakeLength = 1;
  //SETUP 
  snakeSegments[0].x = snakePosition.x;
  snakeSegments[0].y = snakePosition.y;
  snakeSegments[0].z = snakePosition.z;
  
  
  zeroLedArray(); 
  spawnFood();
  
}

void turnOn(int x, int y, int z) {
  digitalWrite(23 + (x + 4*y)*2, LOW);
  digitalWrite(2 + z, HIGH);
}

void turnOff(int x, int y, int z) {
  digitalWrite(23 + (x + 4*y)*2, HIGH);
  digitalWrite(2 + z, LOW);
}

void toggle(int x, int y, int z, byte t) {
  turnOn(x,y,z);
  delay(t);
  turnOff(x,y,z);
}

boolean updateDirection(){
  int left = digitalRead(28);
  int right = digitalRead(30);
  int down = digitalRead(26);
  int up = digitalRead(34);

  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);

  if (!up) Serial.println("Up is pressed");
  if (!down) Serial.println("Down is pressed");
  if (!left) Serial.println("Left is pressed");
  if (!right) Serial.println("Right is pressed");

  boolean moved = false;
  if(!up && ySpeed != -1)
  {
    xSpeed = 0;
    ySpeed = 1;
    zSpeed = 0;
    moved = true;
  }
  else if(!down && ySpeed != 1)
  {
    xSpeed = 0;
    ySpeed = -1;
    zSpeed = 0;
    moved = true;
  }
  else if(!left && xSpeed != 1)
  {
    xSpeed = -1;
    ySpeed = 0;
    zSpeed = 0;
    moved = true;
  }
  else if(!right && xSpeed != -1)
  {
    xSpeed = 1;
    ySpeed = 0;
    zSpeed = 0;
    moved = true;
  }
  else if(buttonState1 == LOW && zSpeed != -1)
  {
    xSpeed = 0;
    ySpeed = 0;
    zSpeed = 1;
    moved = true;
  }
  else if(buttonState2 == LOW && zSpeed != 1)
  {
    xSpeed = 0;
    ySpeed = 0;
    zSpeed = -1;
    moved = true;
  }
  return moved;
}

void zeroLedArray() {
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        turnOff(i,j,k);
      }
    }
  }
}

void spawnFood() {
  byte x, y, z;
  
  do {
    x = random(4);
    y = random(4);
    z = random(4);
  } 
  while (segmentExists(x, y, z));
  foodPosition.x = x;
  foodPosition.y = y;
  foodPosition.z = z;
}

boolean segmentExists(byte x, byte y, byte z) {
  //Serial.println(x);
  for (int i = 0; i < snakeLength; i++) 
    if (snakeSegments[i].x == x && snakeSegments[i].y == y && snakeSegments[i].z == z)
      return true;
  return false;
}

void addNewSegment() {
  snakeSegments[snakeLength].x = snakePosition.x;
  snakeSegments[snakeLength].y = snakePosition.y;
  snakeSegments[snakeLength].z = snakePosition.z;

  snakeLength++;
}


void removeLastSegment() {
  int i = -1;
  for (i = 1; i < snakeLength; i++) {
    snakeSegments[i-1].x = snakeSegments[i].x; 
    snakeSegments[i-1].y = snakeSegments[i].y;
    snakeSegments[i-1].z = snakeSegments[i].z;
  }
  if (i != -1) {
    snakeSegments[i].x = 255;
    snakeSegments[i].y = 255;
    snakeSegments[i].z = 255;
  }

  snakeLength--; 
}
