#ifndef FB_BRAIN_H
#define FB_BRAIN_H

#include "fB_Header.h"

//class fB_Pin; // pre-declare
//class fB_Card; // pre-declare
//class fB_Col; // pre-declare


class fB_Log {
	public:
		uint8_t	fTag;
		char		filename[14];
		//uint8_t 		enabled;
		//char	fileext[4];
		//char		dateStr[MAXCHARSTEXT];
		//char		sizeStr[MAXCHARSTEXT];
		void		show();
		bool		create(char *fname=NULL);
		void		setDate();
		void		writeHeader();
		void		writeData();
		bool		archive();
		void		remove();
		void		getAttributes();
		void		dump();
		fB_Log(uint8_t	fTag, char * filename) ;
};

typedef struct		logStruc {	
	uint8_t	tag;	 
	char		name[13];
	fB_Log*		pLog;			
	};

class fB_Record {
	public:
		logStruc   *Logs;
		void		createTagDefLog();		
		//void		defineSystemGlobals();
		//void		defineElements();
		void		EEwriteTags();
		void		EEinitTags();
		fB_Tag*		EEgetTag(uint16_t tag);
		char*		getLogName(uint8_t fTag);
		void		init();
		bool		SDinit(uint8_t  SSpin, uint8_t  SPIspeed);
		void		initLog(uint8_t fTag, const __FlashStringHelper* Ptitle);

	private:

};



#endif
