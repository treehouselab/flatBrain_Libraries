long int count;
void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
    Serial.println("hello world"); 

}

void loop() {
  if(Serial.available()) {
     Serial.read(); 
     Serial.println(count++,DEC);
   }


}