#ifndef FP_CARD_H
#define FP_CARD_H

class PCF8574; // pre-declare
class MCP23017; // pre-declare

// This class manages the card-level functionality of the backplane. 
// Card-specific protocold for read/write functions s are handled at this level. 

class fB_Card {
public:
        fB_Card(uint16_t tag,const __FlashStringHelper* Ptitle, uint8_t  type,uint8_t  i2cAddr, uint8_t  aChan );

		uint16_t tag;
		const __FlashStringHelper* Ptitle;
		uint8_t  type;
		uint8_t  i2cAddr;
		uint8_t  aChan ;

		void LED(uint8_t val); // Turns HIGH/LOW pilot LED on card
		void openCDchan(uint8_t  chan);
		void AnalogGate(bool logic);
        void MCP_pinMode(uint8_t  pin, unsigned int value);
        void MCPd_pinMode(uint8_t  pin, unsigned int value);
		void MCP_digitalWrite(uint8_t  pin, unsigned int value);
		void MCPd_digitalWrite(uint8_t  pin, unsigned int value);

        void MCPd_write(uint8_t  pin, unsigned int value);
        void MCPd_analogWrite(uint8_t  pin, unsigned int value);
        void MCPd_pull(uint8_t  pin, unsigned int value);
        unsigned int MCPd_getLatches();
        unsigned int MCPd_read(uint8_t  pin);
        unsigned int MCPd_analogRead(uint8_t  pin);
        unsigned int MCPd_digitalRead(uint8_t  pin);

        void CD_write(uint8_t  pin, unsigned int value);
        void CD_analogWrite(uint8_t  pin, unsigned int value);
        void CD_digitalWrite(uint8_t  pin, unsigned int value);
        unsigned int CD_read(uint8_t  pin);
        unsigned int CD_analogRead(uint8_t  pin);
		unsigned int CD_avgAnalogRead(uint8_t  chan,uint16_t size);
        unsigned int CD_digitalRead(uint8_t  pin);
 		//PCF8574 *PCF; 
		//void PCF_openCDchan(uint8_t  chan);
       
private:
    void init();
	MCP23017 *MCP,*MCPd; 

};
 

#endif
