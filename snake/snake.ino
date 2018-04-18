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
  
}

void loop() {

  
  updateDirection();
  boolean moved = false;
  for(int i = 0; i < initialSpeed / (snakeLength + 1); i++){ 
    for(int j = 0; j < snakeLength; j++){
      //toggle(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z, 1);
      turnOn(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
      delayMicroseconds(500);
      
//      int runTime = 0;
//      for(int alex = 0; alex < 100; alex++){
////        moved = updateDirection();
////        if(moved){
////          runTime = 100000;
////          Serial.println("HI");
////        }
////        //runGame();
//        Serial.println("PRINTLN");
//        delayMicroseconds(5);
//        runTime ++;
//      }
//      Serial.println("STOP");
      
      turnOff(snakeSegments[j].x, snakeSegments[j].y, snakeSegments[j].z);
    }
    turnOn(foodPosition.x, foodPosition.y, foodPosition.z);

    delayMicroseconds(500);
    
//    int runTime = 0;
//      for(int alex = 0; alex < 100; alex++){
////        if(updateDirection())
////          runTime = 100000;
//        //runGame();
//        delayMicroseconds(5);
//        Serial.println("WHAT");
//        runTime ++;
//      }

      
      turnOff(foodPosition.x, foodPosition.y, foodPosition.z);
    //toggle(foodPosition.x, foodPosition.y, foodPosition.z, 1);
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
//    col.setAllLow();
//    row.setAllHigh();
    //Serial.println("DEAD");
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

//void generateNewCandy()
//{
//  candy = Location(random(4), random(4), random(4));
//  while(snake.contains(candy))
//  {
//    candy = Location(random(4), random(4), random(4));
//  }
//}
