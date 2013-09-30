#ifndef FB_BRAIN_H
#define FB_BRAIN_H

#include "fB_Header.h"

//class fB_Pin; // pre-declare
//class fB_Card; // pre-declare
//class fB_Col; // pre-declare

class fB_Record {
	public:
		void		init();
		char		filename[MAXCHARSLINE+1]; // current filename. REFRESH FROM fat.DE.filename BUT DON'T POINT to DE record!!!!!
		char		base[MAXCHARSLINE-3]; // ditto above
		char		dateStr[MAXCHARSLINE+1];
		char		sizeStr[MAXCHARSLINE+1];
		uint16_t*	sortRay;		    // array of indexes to FAT objects
		uint8_t		fileCount;
		
		void		EEwriteTags(uint16_t base);
		void		EEloadTags(uint16_t base);
		fB_Tag*		EEloadTag(uint16_t tag, uint16_t base);
		fB_Tag*		EEgetTag(fB_Tag &bufTag, uint16_t tag, uint16_t base);
		void		EEclearTags(uint16_t offTags, uint16_t base);
		int			EEgetEAUTO();
		void		EEwriteEAUTO(uint16_t base);
		void		EEdumpTags(uint16_t base);
		bool		SDinit(uint8_t  SSpin, uint8_t  SPIspeed);

//		static int			compareFilename(const void *x1, const void *x2);
		void		buildFileRay(char *ext); // returns index count;
		void		createTagDefLog();		
		char*		fileFind(uint16_t index);
		bool		fileFind(char *fname);
		bool		fileFind();
		bool		fileCreate(char *fnameL);
		char*		logGetFilename(uint16_t fTag);
		void		logShow();
		// these methods rely in the Record filename pointer pointing to fat.DE.filename
		void		logStamp(uint16_t fTag);
		bool		logCreate(uint16_t fTag);
		void		logSetDate();
		void		logWriteHeader(uint16_t fTag);
		void		logWriteData(uint16_t fTag);
		bool		logArchive();
		void		logRemove();
		void		logGetAttributes();
		void		logDump();


	private:

};



#endif
