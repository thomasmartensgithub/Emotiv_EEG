//this is master

#include <SoftwareSerial.h>
#define up 7
#define down 6
SoftwareSerial BTSerial(10, 11);
char input = '0';
char previousRead = '0';


int dataUp;
int dataDown;

void setup() 
{
  BTSerial.begin(38400);
  Serial.begin(38400);
  pinMode(up, INPUT);
  pinMode(down, INPUT);
}

void loop() 
{
  dataUp = digitalRead(up);
  dataDown = digitalRead(down);
  previousRead = input;
  if(dataUp == 0 && dataDown == 0 && previousRead != '0'){
    //Serial.println("Standby");
    input = '0';
  }
  if (dataUp == 1 && previousRead != '1'){
      //Serial.println("UP");
      input = '1';
      BTSerial.write(input);
  }
  else if(dataDown == 1 && previousRead != '2'){
      //Serial.println("DOWN");
      input = '2';
      BTSerial.write(input);
    }
  delay(50);
 }
 
