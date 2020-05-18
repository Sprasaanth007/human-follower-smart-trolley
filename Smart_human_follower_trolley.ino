#include <Servo.h>  //Servo motor for rotating the Ultrasonic sensor
Servo myservo;     

int Echo = 4;  
int Trig = 5; 

//motor controller pins         //EN pins for speed and IN pins for direction
int ENA = 5
int ENB = 6
int IN1 = 7
int IN2 = 8
int IN3 = 9
int IN4 = 11
int carSpeed = 150              //Ranges from 0 to 255

int rightDistance = 0, leftDistance = 0;     //The distance of human to the right and left of the ultrasonic sensor

void forward(){                             //Function to move the trolley forward 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {                             //Function to move the trolley backward 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {                             //Function to move the trolley left
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {                             //Function to move the trolley right
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

//Ultrasonic distance measurement 
int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float distance = pulseIn(Echo, HIGH);  
  distance= distance / 58;       
  return (int)distance;
}

void setup() { 
  myservo.attach(3);  // attach servo on pin 3 to servo object
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();                 //To initially stop the motors from rotating
}


void loop() { 
  
    myservo.write(60);                 //set servo position to right side
    delay(200); 
    rightDistance = Distance_test();

    myservo.write(120);               //set servo position to left side
    delay(200); 
    leftDistance = Distance_test();

        if((rightDistance > 70)&&(leftDistance > 70)){          //This is for the trolley to avoid going towards farther objects like a wall
      stop();
    }else if((rightDistance >= 20) && (leftDistance >= 20)) {     
      forward();
    }else if((rightDistance <= 10) && (leftDistance <= 10)) {     //This is for the trolley to stay at a minimum distance of 10cms away from us
        back();
        delay(100);
    }else if(rightDistance - 3 > leftDistance) {
        left();
        delay(100);
    }else if(rightDistance + 3 < leftDistance) {
        right();
        delay(100);
    }else{
      stop();
    }
    
}
