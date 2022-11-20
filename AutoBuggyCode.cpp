#include <Servo.h>

int pos = 0;
int fm_c=5;  // Front motor
int fm_ac=9;
int bm_c=13; // Back motor
int bm_ac=2;
int f_ir_r=8;
int f_ir_l=3;
int med= 0;
int med_work=0;
int patient=1;
int turned=0;
int inside=0;
int ir=7;
int go=10;
int pingPin=11;
int u_stop=0;
long remember;
int taken=0;
int only_work=0;

Servo servo_med;
void setup()
{ Serial.begin(9600);
  
  servo_med.attach(6);
  servo_med.write(0);
  pinMode(fm_c, OUTPUT);
  pinMode(fm_ac, OUTPUT);
  pinMode(bm_c, OUTPUT);
  pinMode(bm_ac, OUTPUT);   
  pinMode(12, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(ir, INPUT);
  pinMode(10, INPUT);
  pinMode(f_ir_r,INPUT);
  pinMode(f_ir_l,INPUT);
}



void forward() // move forward
{ 
  digitalWrite(fm_c,HIGH);
  digitalWrite(bm_c,HIGH);
}


void backward() // move backward
{ 
  digitalWrite(fm_ac,HIGH);
  digitalWrite(bm_ac,HIGH);
}

void stop()  // to stop
{
  digitalWrite(fm_ac,LOW);
  digitalWrite(bm_ac,LOW);
  digitalWrite(fm_c,LOW);
  digitalWrite(bm_c,LOW);
}

void turn(char d)  //to turn
{ if(d=='r')

{ stop();
  digitalWrite(bm_c,HIGH); //left tyre will rotate
}

 else if(d=='l')
 {
  stop();
  digitalWrite(fm_c,HIGH); //right tyre will rotate
   
 }
 
}

long ultrasonic()
{
  long duration, cm;
  pinMode (pingPin, OUTPUT);
  digitalWrite(pingPin, HIGH);
  digitalWrite(pingPin, LOW);
  
  pinMode (pingPin, INPUT);
  duration= pulseIn(pingPin, HIGH);
  cm= microsecondsToCentimeters(duration);
  if(cm<100)
  {
   Serial.println("Obstacle");
   stop();
   digitalWrite(12,HIGH);
  }
  
  
  else
  {
    digitalWrite(12,LOW);
  }
 
  return cm;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds /29 /2;
}

int check()
{
if(digitalRead(ir)== HIGH)
  return 1;
 
}

int front=1;
int turning=0;
void movement()
{ int val_f_r = digitalRead(f_ir_r);
  int val_f_l = digitalRead(f_ir_l);
  int go = digitalRead(10);
 if(go==HIGH)
 { 
   inside++;
   Serial.println(" Stopped near patient ");
 }
 
 if(inside>0)
 { if (taken==0)
 { Serial.println(" Waiting for patient to take med ");
   stop();
 } 
  
    else if(taken>0)
    {  
       if (val_f_r == 1 && val_f_l == 1)
       {
        if(turned==0)
        { backward();
        
         Serial.println(" Going Back ");
        }
   
        else if(turned != 0)
        { stop();
          delay(500); 
          forward();
         Serial.println(" Going Forward to cross the junction so dont turn again ");
          delay(1000);//So it can pass the jusction and dont turn again
          inside=0; 
          turned=0;
          taken=0;
          return ;
         }
        }
    
     else if(val_f_r==1 && val_f_l == 0)
     { 
     turn('l');
     Serial.println(" Back Turning left ");
     turned++;
    
     }
     
     else if (val_f_r==0 && val_f_l == 1)
     {
      turn('r');
      Serial.println(" Back Turning Right ");
      turned++;
     } 
    }
 }
 
 
if (val_f_r == 1 && val_f_l == 1 && inside==0)
{
  forward();
  only_work=0;
 Serial.println(" Going Forward ");
  }

 else if(val_f_r==0 && val_f_l == 1 && inside==0 && only_work==0 ) //turning right
 { only_work=1;
   turn('r');    
 Serial.println(" Turning Right ");
  }
 
else if(val_f_r==1 && val_f_l == 0 && inside==0 && only_work==0 ) //turning left
 { only_work=1;
   turn('l'); 
  Serial.println(" Turning Left ");
 
  } 
 
}
   


void loop()
{ while(ultrasonic()>100)
   { movement();
   
    if(digitalRead(ir)== HIGH)
  {  med = med+20;
   servo_med.write(med);
   Serial.print("Patient ");
   Serial.print(patient);
   Serial.print(" has taken the med");
   Serial.println();
   patient++;
   delay(1000);
   taken=1;
   return;}
   
  else 
  { servo_med.write(med);
 }
   }
}









































