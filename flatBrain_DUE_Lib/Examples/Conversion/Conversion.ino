void setup() {

 Serial.begin(9600);
 
		double mins, dfactor;
		int    ifactor;
		unsigned long msecs;

		mins = .5;
		dfactor = 60000;
		ifactor = 60000;

		msecs = (unsigned long) (mins * 60000);
		Serial.println(msecs); // 30000  expected!

		msecs = (unsigned long) (mins * dfactor);
		Serial.println(msecs); // 30000

		msecs = (unsigned long) (mins * ifactor);
		Serial.println(msecs); // O

		msecs = (unsigned long) ( mins * double( ifactor)};
		Serial.println(msecs); // 0

		msecs = mins * (unsigned long)ifactor;
		Serial.println(msecs); // 2147480832


		msecs = (unsigned long) ( mins * (unsigned long)ifactor);
		Serial.println(msecs); //// 2147480832

}

void loop() {
  // put your main code here, to run repeatedly: 
  
  
}