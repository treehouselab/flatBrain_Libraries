#ifndef FB_BRAIN_H
#define FB_BRAIN_H

#include "fB_Header.h"

//class fB_Pin; // pre-declare
//class fB_Card; // pre-declare
//class fB_Col; // pre-declare

class fB_Record {
	public:
		char		filename[MAXCHARSLINE+1]; // current filename. REFRESH FROM fat.DE.filename BUT DON'T POINT to DE record!!!!!
		char		base[MAXCHARSLINE-3]; // ditto above
		char		dateStr[MAXCHARSLINE+1];
		char		sizeStr[MAXCHARSLINE+1];
		uint16_t*	sortRay;		    // array of indexes to FAT objects
		uint8_t	fileCount;
		
		void		EEwriteTags();
		void		EEinitTags();
		fB_Tag*		EEgetTag(uint16_t tag);
		void		init();
		bool		SDinit(uint8_t  SSpin, uint8_t  SPIspeed);

//		static int			compareFilename(const void *x1, const void *x2);
		void		buildFileRay(char *ext); // returns index count;
		void		createTagDefLog();		
		char*		fileFind(uint16_t index);
		bool		fileFind(char *fname);
		bool		fileCreate(char *fnameL);
		void		logShow();
		// these methods rely in the Record filename pointer pointing to fat.DE.filename
		void		logStamp();
		bool		logCreate(char *base);
		void		logSetDate();
		void		logWriteHeader();
		void		logWriteData();
		bool		logArchive();
		void		logRemove();
		void		logGetAttributes();
		void		logDump();


	private:

};



#endif
