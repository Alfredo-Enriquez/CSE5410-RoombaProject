#include <Servo.h>


#define Lpwm_pin  6     //pin of controlling speed---- ENA of motor driver board
#define Rpwm_pin  3    //pin of controlling speed---- ENB of motor driver board

#define pinRF 12     //pin of controlling turning---- IN1 of motor driver board
#define pinRB 11     //pin of controlling turning---- IN2 of motor driver board
#define pinLF 7      //pin of controlling turning---- IN3 of motor driver board
#define pinLB 8      //pin of controlling turning---- IN4 of motor driver board


Servo myservo;
volatile int DL;
volatile int DM;
volatile int DR;
int counter;

int sm, sL, sR;

float checkdistance() //ultrasonic sensor checks distnace in front of sensor
{
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  float distance = pulseIn(2, HIGH) / 25.00;
  delay(10);
  return distance;
}

float checkWall() //checks left and right to decide where to turn
{
  myservo.write(180);
  for (int i = 0; i < 3; i = i + 1) 
  {
    DL = checkdistance();
    delay(100);
  }
 
  myservo.write(0);
  for (int i = 0; i<3; i = i + 1) 
  {
    DR = checkdistance();
    delay(100);
  }

  if(DL>DR) //turns right
  {
    myservo.write(90);
    Set_Speed(200);
    turnR();
    delay(500);
    Set_Speed(90);
    advance();
  } 
  else if(DR>DL) //turns left
  {
    myservo.write(90);
    Set_Speed(200);
    turnL();
    delay(500);
    Set_Speed(90);
    advance();
  }
  else //advances foward
  {
    myservo.write(90);
    Set_Speed(90);
    advance();
  }
}



void edge() // detects an edge
{
  sm = analogRead(A0);
  sL = analogRead(A1);
  sR = analogRead(A2);
  Serial.println(sm);
  if(sm>400 && sL>400 && sR>400) //if edge isn't present
  {
    Set_Speed(90);
    advance();
  }
  else //if edge is present
  {
    Set_Speed(0);
    stopp(); 
    delay(500);
    Set_Speed(150);
    back();
    delay(500);
    Set_Speed(200);
    turnL();
    delay(500);
  }
}  


void Set_Speed(unsigned char pwm) //function of setting speed
{
  analogWrite(Lpwm_pin,pwm);
  analogWrite(Rpwm_pin,pwm);
}

void advance()    //  going forward
{
  noTone(buzzer_pin);
  digitalWrite(pinRB,LOW);  // making motor move towards right rear
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,LOW);  // making motor move towards left rear
  digitalWrite(pinLF,HIGH); 
  
}

void turnR()        //turning right(dual wheel)
{
  digitalWrite(pinRB,LOW);  //making motor move towards right rear
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,LOW);  //making motor move towards left front
}
void turnL()         //turning left(dual wheel)
{
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,LOW );   //making motor move towards right front
  digitalWrite(pinLB,LOW);   //making motor move towards left rear
  digitalWrite(pinLF,HIGH);
}    
    
void stopp()        //stop
{
  digitalWrite(pinRB,HIGH);
  digitalWrite(pinRF,HIGH);
  digitalWrite(pinLB,HIGH);
  digitalWrite(pinLF,HIGH);
}
void back()         //back up
{
  digitalWrite(pinRB,HIGH);  //making motor move towards right rear     
  digitalWrite(pinRF,LOW);
  digitalWrite(pinLB,HIGH);  //making motor move towards left rear
  digitalWrite(pinLF,LOW);
}

void setup()
{
  Serial.begin(9600);
  myservo.attach(10);
  pinMode(4, OUTPUT); //trig pin
  pinMode(2, INPUT);  //echo pin
  pinMode(pinLB,OUTPUT); // /pin 8
  pinMode(pinLF,OUTPUT); // pin 7
  pinMode(pinRB,OUTPUT); // pin 11
  pinMode(pinRF,OUTPUT);  // pin 12
  pinMode(Lpwm_pin,OUTPUT);  // pin 6 (PWM) 
  pinMode(Rpwm_pin,OUTPUT);  // pin 3 (PWM) 

  
  DL = 0;
  DM = 0;
  DR = 0;
  sm = 0;
  sL = 0;
  sR = 0;
  myservo.write(90);
}

void loop()
{
  myservo.write(90);
  DM = checkdistance();
  
  if(DM<25)
  {
    Set_Speed(0);
    stopp();
    delay(1000);
    checkWall();
    Set_Speed(90);
    advance();
  }
  else
  {
    Set_Speed(90);
    advance();
    edge();
  }
}
