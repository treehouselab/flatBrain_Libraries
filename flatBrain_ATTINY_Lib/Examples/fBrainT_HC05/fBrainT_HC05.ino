/*
This code will run the bluetooth as slave
pressing 1 turns on led 4
pressing 0 turns off led 4
*/
 
#include <SoftwareSerial.h>
#define TxD 3
#define RxD 2

#define DEBUG_ENABLED  1
 
SoftwareSerial blueToothSerial(RxD,TxD);

int led = 10;
 
void setup() 
{ 
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();
  
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
 
} 
 
void loop() 
{ 
  char recvChar;
  while(1){
    //check if there's any data sent from the remote bluetooth shield
    if(blueToothSerial.available()){
      recvChar = blueToothSerial.read();
      
        if(recvChar == '1')
          digitalWrite(led,HIGH);  
       
        else
          digitalWrite(led,LOW); 
    }
  }
} 
 
void setupBlueToothConnection()
{
  blueToothSerial.begin(9600); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=HC-05\r\n"); //set the bluetooth name as "HC-05"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  
  delay(2000); // This delay is required.
  //blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  blueToothSerial.print("bluetooth connected!\n");
  
  delay(2000); // This delay is required.
  blueToothSerial.flush();
}}