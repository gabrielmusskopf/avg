// C++ code

// Pins
#define LED_DEBUG               13
#define IT_LINE_COUNTER          3
#define IT_OBSTACLE              2
#define IN_LINE_FOLLOWER_RIGHT  11
#define IN_LINE_FOLLOWER_LEFT   12
#define OUT_PWM_MOTOR_RIGHT      5
#define OUT_PWM_MOTOR_LEFT       6
#define OUT_MOTOR_RIGHT_IN_0     8
#define OUT_MOTOR_RIGHT_IN_1     7
#define OUT_MOTOR_LEFT_IN_0     10
#define OUT_MOTOR_LEFT_IN_1      9

// Machone states
#define MOVING			    1
#define TURNING_LEFT	  2
#define TURNING_RIGHT	  3
#define STOPPED         4
#define ARRIVED			    5

//
#define LEFT	0
#define RIGHT	1 

int steps = 6;
int turnAfter = 3;
int turn = RIGHT;

int hasObstacle = 0;
int lines = 0;
int state = MOVING;

int isDebug = 0;

void debug(char* msg) {
    if (isDebug) {
        Serial.println(msg);
    }
}

void debug(int msg) {
    if (isDebug) {
        Serial.println(msg);
    }
}

void setup() {
  // Debug configurations
  Serial.begin(9600);
  pinMode(LED_DEBUG, OUTPUT);
  
  // Configurations of IOs
  pinMode(IT_LINE_COUNTER, INPUT);
  pinMode(IT_OBSTACLE, INPUT);
  pinMode(IN_LINE_FOLLOWER_RIGHT, INPUT);
  pinMode(IN_LINE_FOLLOWER_LEFT, INPUT);
  pinMode(OUT_PWM_MOTOR_RIGHT, OUTPUT);
  pinMode(OUT_PWM_MOTOR_LEFT, OUTPUT);
  pinMode(OUT_MOTOR_RIGHT_IN_0, OUTPUT);
  pinMode(OUT_MOTOR_RIGHT_IN_1, OUTPUT);
  pinMode(OUT_MOTOR_LEFT_IN_0, OUTPUT);
  pinMode(OUT_MOTOR_LEFT_IN_1, OUTPUT);

  // Configurations of Interruptions 
  attachInterrupt(digitalPinToInterrupt(IT_LINE_COUNTER), handleCrossLine, RISING);
  attachInterrupt(digitalPinToInterrupt(IT_OBSTACLE), handleObstacle, CHANGE);
}

// Interruption Callbacks

const long debounceDelay = 100;
long lastLineInterruptTime = 0;
long lastObstacleInterruptTime = 0;

void handleCrossLine() {
  long interruptTime = millis();

  if (interruptTime - lastLineInterruptTime > debounceDelay) {
    lastLineInterruptTime = interruptTime;

    debug("Crossed");
    lines++;
  
    if (lines == turnAfter) {
      state = (turn == RIGHT) ? TURNING_RIGHT : TURNING_LEFT;
    }
  
    if (lines == steps) {
      state = ARRIVED;
    }
  }
 
}

void handleObstacle() {
  long interruptTime = millis();
  if (interruptTime - lastObstacleInterruptTime <= debounceDelay) {
      return;
  }

  lastObstacleInterruptTime = interruptTime;
  hasObstacle = !hasObstacle;
}

void loop() {

  int x = digitalRead(IT_LINE_COUNTER);
  debug(x);

  // Machine state
  switch(state) {
  	case MOVING:
      // Check if there is an obstacle
      //if(hasObstacle){
      //  state = STOPPED;
      //  break;
      //}

      // Controls the AGV's movement
      move();
      break;
    
    case TURNING_LEFT:
      turnLeft();
      break;
    
    case TURNING_RIGHT:
      turnRight();
      break;

    case STOPPED:
      // Stop the AGV
      stop();

      // Check if there isn't an obstacle
      //if(!hasObstacle){
      //  state = MOVING;
      //}

      break;
    
    case ARRIVED:
      //Serial.println("Finally done!");
      delay(1000);
      break;
  }
  
}

void move(){
  debug("Moving...");
  delay(1000);
}


void stop() {
  debug("Stoped");

  // Turn off RIGHT motor
  analogWrite(OUT_PWM_MOTOR_RIGHT, 0);
  digitalWrite(OUT_MOTOR_RIGHT_IN_0, LOW);
  digitalWrite(OUT_MOTOR_RIGHT_IN_1, LOW);
  // Turn off LEFT motor
  analogWrite(OUT_PWM_MOTOR_LEFT, 0);
  digitalWrite(OUT_MOTOR_LEFT_IN_0, LOW);
  digitalWrite(OUT_MOTOR_LEFT_IN_1, LOW);
}

void turnLeft() { 
  debug("Turning left");
  delay(1000);
  state = MOVING;
}

void turnRight() { 
  debug("Turning right");
  delay(1000);
  state = MOVING;
}
