#include<Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial s(11,12);
const int op = 5;
const int button = A1;
const int ecg = A2;
const int buzzer = 9;
const int output = 10;
int bpm;
int adc;
boolean counter;
int count;
unsigned long millisBefore;
unsigned long beatTime=20000;
const int threshold=360;
const int MPU6050_addr=0x68;
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;
int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
void setup(){
  s.begin(9600);
  pinMode(buzzer,OUTPUT);
  pinMode(button, INPUT);
  pinMode(op,OUTPUT);
  analogWrite(op,255);
   counter = true;
  millisBefore = millis();
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  {
  int ecgdata = analogRead(ecg);
  Serial.println(ecgdata);
  }
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  double  Sum;
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr,14,true);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
   Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  Sum = sqrt(pow(Ax,2)+pow(Ay,2)+pow(Az,2));
 // T = (double)Temperature/340+36.53; //temperature formula
  //Gx = (double)GyroX/GyroScaleFactor;
  //Gy = (double)GyroY/GyroScaleFactor;
  //Gz = (double)GyroZ/GyroScaleFactor;
  // Serial.print("Ax: "); Serial.print(Ax);
  //Serial.print(" Ay: "); Serial.print(Ay);
  //Serial.print(" Az: "); Serial.print(Az);
 //Serial.print(" Sum vector ");Serial.println(Sum);
  adc = analogRead(A0);
  //Serial.println(adc);
  delay(10);
  if ((millis() - millisBefore) < beatTime) {
    if (counter == true) {
      if (adc >= threshold) {
        count++;
        counter = false;
        //Serial.print("Beat : ");
        //Serial.println(count);
      }
    }
    if (adc < threshold) {
      counter = true;
    }
  } else {
    bpm = count*(60000/beatTime);
    
   if(s.available()>0)
{
 s.write(bpm);
}
    //Serial.print(bpm);
    //Serial.println(" BPM");
    count=0;
    millisBefore = millis();
  }
  while(Sum>=3.3)
  { 
    int cnt=0;
    int input;
    while(cnt<=15)
    {
    tone(buzzer,500);
    delay(200);
    noTone(buzzer);
    delay(200);
    input = analogRead(button);
    cnt++;
    if(input>680)
    break;
    }
    //Serial.print(input);
    if(input>=680)
    {
      Sum = 0;
      break;
      }
      else{
       // Serial.print("high output");
         digitalWrite(op,LOW);
   //analogWrite(op,255);
    delay(3000);
   digitalWrite(op,HIGH);
     //Serial.print("low output");
     Sum = 0;
    }}
    }
      
    
    
    
  //Serial.print(" T: "); Serial.print(T);
  //Serial.print(" Gx: "); Serial.print(Gx);
 // Serial.print(" Gy: "); Serial.print(Gy);
 // Serial.print(" Gz: "); Serial.println(Gz);

 
  
