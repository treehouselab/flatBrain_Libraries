/* Attiny4313 example (G.Small, 24Mar2012)
Example of Attiny4313 on Arduino Tiny core, with serial comms test

ATMEL 6 PIN ISP CONNECTOR TOP VIEW:
                        +---+
                MISO  1 |o o| 2  +Vcc
(SCL, to UNO A5) SCK  3 |o o| 4  MOSI (SDA, to UNO A4)
              /RESET  5 |o o| 6  GND
                        +---+

ATMEL AttinyX313/ARDUINO:

                        +-\/-+
           (D 17) PA2  1|    |29  VCC
      RX   (D  0) PD0  2|    |19  PB7 (D  16)
      TX   (D  1) PD1  3|    |18  PB6 (D  15)
           (D  2) PA1  4|    |17  PB5 (D  14)
           (D  3) PA0  5|    |16  PB4 (D  13)*
      INT0 (D  4) PD2  6|    |15  PB3 (D  12)*
      INT1 (D  5) PD3  7|    |14  PB2 (D  11)*
           (D  6) PD4  8|    |13  PB1 (D  10)
          *(D  7) PD5  9|    |12  PB0 (D  9)
                  GND 10|    |11  PD6 (D  8)
                        +----+

 * indicates PWM port

 These arrays map port names (e.g. port B) to the appropriate addresses
 for various functions (e.g. reading and writing)

*/ 

#define LED_PIN           2                    // ATtiny Pin 4 (D2)
//#define LED_PIN           6                     // ATtiny Pin 8 (D6)

void setup(){
  pinMode(LED_PIN,OUTPUT);                      // for general DEBUG use
  LED_Blink(LED_PIN,2);                         // show it's alive
  Serial.begin(9600);
}

void loop() {
  int test = 31;
  Serial.print("test");
  LED_Blink(LED_PIN,1);                         // show it's alive
}

// LED blinker for status
void LED_Blink(byte pin, byte times){
  for (byte i=0; i< times; i++){
    digitalWrite(pin,LOW);
    delay (1000);
    digitalWrite(pin,HIGH);
    delay (1000);
  }
}