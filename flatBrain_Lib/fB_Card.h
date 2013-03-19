#ifndef FP_CARD_H
#define FP_CARD_H

class PCF8574; // pre-declare
class MCP23017; // pre-declare

// This class manages the card-level functionality of the backplane. 
// Card-specific protocold for read/write functions s are handled at this level. 

class fB_Card {
public:
        fB_Card(uint16_t cTag,char* tagStr, uint8_t  index, uint8_t  cType,uint8_t  i2cAddr, uint8_t  aChan );
        ~fB_Card();
		uint16_t cTag;
		char*    tagStr;
		uint8_t  index;
		uint8_t  cType;
		uint8_t  i2cAddr;
		uint8_t  aChan ;
        void MCP_pull(uint8_t  pin, unsigned int value);
        void MCP_pinMode(uint8_t  pin, unsigned int value);
        void MCP_write(uint8_t  pin, unsigned int value);
        void MCP_analogWrite(uint8_t  pin, unsigned int value);
		void MCP_digitalWrite(uint8_t  pin, unsigned int value);
        unsigned int MCP_getLatches();
        unsigned int MCP_read(uint8_t  pin);
        unsigned int MCP_analogRead(uint8_t  pin);
        unsigned int MCP_digitalRead(uint8_t  pin);
		void openCDchan(uint8_t  chan);
		void VDselectR(uint8_t  chan);
		void LED(uint8_t val); // Turns HIGH/LOW pilot LED on card


        unsigned int PCF_digitalRead(uint8_t  pin);
        
		void VDgate(bool logic);
		//void CD8inhibit(bool logic);
		//void CD16inhibit(bool logic);

        void CD_write(uint8_t  pin, unsigned int value);
        void CD_analogWrite(uint8_t  pin, unsigned int value);
        void CD_digitalWrite(uint8_t  pin, unsigned int value);
        unsigned int CD_read(uint8_t  pin);
        unsigned int CD_analogRead(uint8_t  pin);
        unsigned int CD_digitalRead(uint8_t  pin);
 		PCF8574 *PCF; 
		void PCF_openCDchan(uint8_t  chan);
       
private:
    void init();
	MCP23017 *MCP; 

};
 

#endif
