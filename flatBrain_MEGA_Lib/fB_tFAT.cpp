/*
	fB_tFAT.cpp - Arduino library support FAT16 on SD cards
	Copyright (C)2010-2011 Henning Karlsen. All right reserved

	You can find the latest version of the library at 
	http://www.henningkarlsen.com/electronics

	This library has been made to easily use SD card with the Arduino.

	If you make any modifications or improvements to the code, I would appreciate
	that you share the code with me so that I might include it in the next release.
	I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "fB_Include.h"


/* Public */

fB_tFAT::fB_tFAT()
{
	_inited=false;
}

uint8_t fB_tFAT::initFAT(uint8_t  speed)
{
	uint8_t res;
	//mmc::setSSpin(SSpin); // SPI chip select pin, this call sets PORT, DDR, and SS bit values
	res = mmc::initialize(speed);
	if(res != RES_OK) return res;
// Read MBR
	if (RES_OK == mmc::readSector(buffer, 0))
	{
		if ((buffer[0x01FE]==0x55) && (buffer[0x01FF]==0xAA))
		{
			MBR.part1Type=buffer[450];
			MBR.part1Start = uint16_t(buffer[454])+(uint16_t(buffer[455])<<8)+(uint32_t(buffer[456])<<16)+(uint32_t(buffer[457])<<24);
			MBR.part1Size = uint16_t(buffer[458])+(uint16_t(buffer[459])<<8)+(uint32_t(buffer[460])<<16)+(uint32_t(buffer[461])<<24);
		}
		else ERROR_MBR_SIGNATURE;
	}
	else return ERROR_MBR_READ_ERROR;

	if ((MBR.part1Type!=0x04) && (MBR.part1Type!=0x06) && (MBR.part1Type!=0x86)) return ERROR_MBR_INVALID_FS;

// Read Boot Sector
	if (RES_OK == mmc::readSector(buffer, MBR.part1Start))
    {
		if ((buffer[0x01FE]==0x55) && (buffer[0x01FF]==0xAA))
		{
			BS.sectorsPerCluster = buffer[0x0D];
			BS.reservedSectors = uint16_t(buffer[0x0E])+(uint16_t(buffer[0x0F])<<8);
			BS.fatCopies = buffer[0x10];
			BS.rootDirectoryEntries = uint16_t(buffer[0x11])+(uint16_t(buffer[0x12])<<8);
			BS.totalFilesystemSectors = uint16_t(buffer[0x13])+(uint16_t(buffer[0x14])<<8);
			if (BS.totalFilesystemSectors==0)
				BS.totalFilesystemSectors = uint16_t(buffer[0x20])+(uint16_t(buffer[0x21])<<8)+(uint32_t(buffer[0x22])<<16)+(uint32_t(buffer[0x23])<<24);
			BS.sectorsPerFAT = uint16_t(buffer[0x16])+(uint16_t(buffer[0x17])<<8);
			BS.hiddenSectors = uint16_t(buffer[0x1C])+(uint16_t(buffer[0x1D])<<8)+(uint32_t(buffer[0x1E])<<16)+(uint32_t(buffer[0x1F])<<24);
			BS.partitionSerialNum = uint16_t(buffer[0x27])+(uint16_t(buffer[0x28])<<8)+(uint32_t(buffer[0x29])<<16)+(uint32_t(buffer[0x2A])<<24);
			firstDirSector = MBR.part1Start + BS.reservedSectors + (BS.fatCopies * BS.sectorsPerFAT);
			BS.fat1Start = MBR.part1Start + BS.reservedSectors;
			BS.fat2Start = BS.fat1Start + BS.sectorsPerFAT;
			BS.partitionSize = float((MBR.part1Size*512)/float(1048576));
		}
		else
			return ERROR_BOOTSEC_SIGNATURE;
	}
	else
		return ERROR_BOOTSEC_READ_ERROR;

	_inited=true;
	return 0x00;
}



uint8_t fB_tFAT::findNextFile(){

	unsigned long currSec = firstDirSector;
	int i;
	word offset = DEcnt*32;
	while (offset>=512)	{
		currSec++;
		offset-=512;
	}
	mmc::readSector(buffer, currSec);

	if (buffer[0]==0x00) return ERROR_NO_MORE_FILES;
	else {
		while ((buffer[offset + 0x0B] & 0x08) || (buffer[offset + 0x0B] & 0x10) || (buffer[offset]==0xE5))	{
			offset+=32;
			DEcnt++;
			if (offset==512){
			  currSec++;
			  mmc::readSector(buffer, currSec);
			  offset = 0;
			} 		
			

			if (buffer[offset]==0x00) return ERROR_NO_MORE_FILES;
		}
		DE.sec = currSec;
		DE.offset = offset;
		for ( i=0; i<8 && (buffer[i+offset]!=0x20); i++) DE.basename[i] = buffer[i+offset];
		DE.basename[i] = 0;
		if(!DE.basename[0]) return ERROR_NO_MORE_FILES;
		for ( i=0; i<3 && (buffer[i+0x08+offset]!=0x20); i++)	DE.fileext[i] = buffer[i+0x08+offset];
		DE.fileext[i] = 0;
		sprintf(DE.filename,"%s.%s",DE.basename,DE.fileext);
		DE.attributes = buffer[0x0B + offset];
		DE.time = uint16_t(buffer[0x0E + offset]) + (uint16_t(buffer[0x0F + offset])<<8);
		DE.date = uint16_t(buffer[0x10 + offset]) + (uint16_t(buffer[0x11 + offset])<<8);
		DE.startCluster = uint16_t(buffer[0x1A + offset]) + (uint16_t(buffer[0x1B + offset])<<8);
		DE.fileSize = uint16_t(buffer[offset + 0x1C]) | (uint16_t(buffer[offset + 0x1D])<<8) | (uint32_t(buffer[offset + 0x1E])<<16) | (uint32_t(buffer[offset + 0x1F])<<24);
		DEcnt++;
		return NO_ERROR;
	}
}

uint8_t fB_tFAT::findIndex(uint16_t index) // resets fat pointer to file at index position ( count from zero)
{	
	uint8_t res = NO_ERROR;
	uint16_t count = 0;
	restartDir();
	while(findNextFile() == NO_ERROR) {
		if(count++ == index)  return NO_ERROR;
	}
	return ERROR_FILE_NOT_FOUND;
}
uint16_t  fB_tFAT::fileCount() {
	uint8_t res = NO_ERROR;
	int count=0;
	restartDir();
	while ( findNextFile()==NO_ERROR) 	count++;
	return count;
}

uint16_t  fB_tFAT::fileCountExt(char* ext) {
	if(!ext) return fileCount();
	uint16_t count=0,i;
	restartDir();
	while(findNextFile() == NO_ERROR) {
		if(ext) {
			for( i =0;i<3 && i < strlen(ext);i++) if(DE.fileext[i] != ext[i]) break;
			if(i!=3 && i!=strlen(ext)) continue;
		}
		count++;
	}
	return count;
}

uint8_t fB_tFAT::findNextExt(char *ext, uint16_t &index) // finds next file that matches ext substr, passes back position 
{	
	uint8_t res = NO_ERROR;
	int i,skip;

	while( findNextFile()== NO_ERROR) {
		skip = 0;
		if(ext) {
			for(int i =0;i<3 && i < strlen(ext);i++) if(DE.fileext[i]  != ext[i]) { skip = 1; break; }
			if(!skip) {
				return NO_ERROR;
			}
			index++;
		}else return NO_ERROR;
	}
	return ERROR_FILE_NOT_FOUND;
}
uint8_t fB_tFAT::findNextBase(char *base)
{	
	uint8_t res = NO_ERROR;
	while( findNextFile()==NO_ERROR) if(!strcmp(DE.basename,base)) return NO_ERROR;
	return ERROR_FILE_NOT_FOUND;
}

uint8_t fB_tFAT::getFile(char *fn)
{
	int i, j;
	for (i=0; i<strlen(fn); i++) fn[i]=uCase(fn[i]);
	restartDir();
	while (findNextFile() ==NO_ERROR) if (!strcmp(DE.filename,fn))	return NO_ERROR;
	return ERROR_FILE_NOT_FOUND;
}


uint8_t fB_tFAT::openFile(char *fn, uint8_t mode)
{
	uint8_t res;
	int i, j;

	if (currFile.filename[0]!=0x00)	return ERROR_ANOTHER_FILE_OPEN;

	for (i=0; i<strlen(fn); i++)	fn[i]=uCase(fn[i]);

	restartDir();
	if (getFile(fn)==NO_ERROR)
	{
		if (!strcmp(DE.filename,fn)){
			sprintf(currFile.filename,"%s",DE.filename);
			currFile.currentCluster=DE.startCluster;
			currFile.fileSize=DE.fileSize;
			currFile.currentPos=0;
			currFile.fileMode=mode;
			return NO_ERROR;
		}
	}
	else ERROR_FILE_NOT_FOUND;
}

uint8_t fB_tFAT::stampFile(char *fn,uint8_t  year,uint8_t  month,uint8_t  day,uint8_t  hour,uint8_t  min){
	unsigned long currSec;
	word offset;
	uint16_t dbuf = 0;
	uint16_t tbuf = 0;
	uint8_t 	 dH=0,dL=0,tH=0,tL=0;

	dbuf |= (year-1980) << 9;
	dbuf |= month << 5;
	dbuf |= day;

	tbuf |= hour << 11;
	tbuf |= min << 5;

	dH  = dbuf >> 8;
	dL  = dbuf & 0xFF;
	tH  = tbuf >> 8;
	tL  = tbuf & 0xFF;

	if (getFile(fn)==NO_ERROR)	{
		currSec = DE.sec;
		offset = DE.offset;
		mmc::readSector(buffer, currSec);
		buffer[0x11 + offset] = dH;
		buffer[0x10 + offset] = dL;
		buffer[0x0F + offset] = tH;
		buffer[0x0E + offset] = tL;
		mmc::writeSector(buffer, currSec);
		closeFile();
		return NO_ERROR;
	}
	return NO_ERROR;
}



uint8_t fB_tFAT::renameFile(char *fn1, char *fn2)
{
	unsigned long currSec;
	word offset;
	int i, j;
	for (i=0; i<strlen(fn1); i++)	fn1[i]=uCase(fn1[i]);

	for (i=0; i<strlen(fn2); i++) {
		fn2[i]=uCase(fn2[i]);
		if (!validChar(fn2[i]))return ERROR_RENAME_ERROR;
	}

	if (getFile(fn1)==NO_ERROR)	{
		currSec = DE.sec;
		offset = DE.offset;
		mmc::readSector(buffer, currSec);

		for (int i=0; i<11; i++) buffer[i+offset]=0x20;
		j=0;
		for (int i=0; i<strlen(fn2); i++)
		{
			if (fn2[i]=='.') j=8;
			else {
				buffer[j+offset]=fn2[i];
				j++;
			}
		}
		mmc::writeSector(buffer, currSec);
		return NO_ERROR;
	}
	else return ERROR_RENAME_ERROR;
}

uint8_t fB_tFAT::deleteFile(char *fn)
{
	unsigned long currSec = firstDirSector;
	uint16_t firstCluster, currCluster, nextCluster;
	word offset = -32;
	int j;
		dbug(F("fatdf %s"),fn);
	for (int i=0; i<strlen(fn); i++)	fn[i]=uCase(fn[i]);

	if (getFile(fn)==NO_ERROR)	{
		dbug(F("fatdf2 %s"),fn);
		currSec = DE.sec;
		offset = DE.offset;
		mmc::readSector(buffer, currSec);

		buffer[offset]=0xE5;
		firstCluster = uint16_t(buffer[0x1A + offset]) + (uint16_t(buffer[0x1B + offset])<<8);
		mmc::writeSector(buffer, currSec);
		
		if (firstCluster!=0)
		{
			currSec=firstCluster/256;
			mmc::readSector(buffer, BS.fat1Start+currSec);
			currCluster=firstCluster;
			nextCluster=0;
			while (nextCluster!=0xFFFF)
			{
				nextCluster = buffer[(currCluster % 256)*2] + (buffer[((currCluster % 256)*2)+1]<<8);
				buffer[(currCluster % 256)*2]=0;
				buffer[((currCluster % 256)*2)+1]=0;
				if (((currCluster/256) != (nextCluster/256)) and (nextCluster!=0xFFFF))
				{
					mmc::writeSector(buffer, BS.fat1Start+currSec);
					currSec=nextCluster/256;
					mmc::readSector(buffer, BS.fat1Start+currSec);

				}
				currCluster=nextCluster;
			}
			mmc::writeSector(buffer, BS.fat1Start+currSec);

			currSec=firstCluster/256;
			mmc::readSector(buffer, BS.fat2Start+currSec);
			currCluster=firstCluster;
			nextCluster=0;
			while (nextCluster!=0xFFFF)
			{
				nextCluster = buffer[(currCluster % 256)*2] + (buffer[((currCluster % 256)*2)+1]<<8);
				buffer[(currCluster % 256)*2]=0;
				buffer[((currCluster % 256)*2)+1]=0;
				if (((currCluster/256) != (nextCluster/256)) and (nextCluster!=0xFFFF))
				{
					mmc::writeSector(buffer, BS.fat2Start+currSec);
					currSec=nextCluster/256;
					mmc::readSector(buffer, BS.fat2Start+currSec);

				}
				currCluster=nextCluster;
			}
			mmc::writeSector(buffer, BS.fat2Start+currSec);
		}
		return NO_ERROR;
	}
	else return ERROR_DELETE_ERROR;
}


uint16_t fB_tFAT::readBinary()
{
	uint32_t sec;
	uint8_t  status_;

	if (currFile.fileMode==FILEMODE_BINARY)
	{
		if ((currFile.currentPos==0) and (currFile.currentCluster==0))
			return FILE_IS_EMPTY;
		if (((currFile.currentPos % BS.sectorsPerCluster)==0) and (currFile.currentPos>0))
			currFile.currentCluster=findNextCluster(currFile.currentCluster);
		sec=BS.hiddenSectors + (uint32_t)BS.reservedSectors + ((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+(((uint32_t)BS.rootDirectoryEntries*32)/512)+((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster+((uint32_t)currFile.currentPos % (uint32_t)BS.sectorsPerCluster);
		status_=mmc::readSector(buffer, sec);
		if (status_)
		{
			return status_;
		}
		currFile.currentPos++;
		if ((currFile.currentPos*512)>currFile.fileSize)
		{
				return (currFile.fileSize-((currFile.currentPos-1)*512));
		}
		else
		{
				return 512;
		}
	}
	else
		if (currFile.fileMode==0x00)
			return ERROR_NO_FILE_OPEN;
		else
			return ERROR_WRONG_FILEMODE;
}

uint16_t fB_tFAT::lineCount(char *fn){
	uint32_t sec;
	uint16_t lineCount = 0;

	if(openFile(fn,FILEMODE_TEXT_READ) == NO_ERROR) {
		if ((currFile.currentPos==0) and (currFile.currentCluster==0))	return lineCount;
		do {
			sec=((uint32_t)BS.reservedSectors+((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+(((uint32_t)BS.rootDirectoryEntries*32)/512)+(((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster)+BS.hiddenSectors)+(((uint32_t)currFile.currentPos/512) % (uint32_t)BS.sectorsPerCluster);
			mmc::readSector(buffer, sec);

			while ((currFile.currentPos<currFile.fileSize) and (buffer[currFile.currentPos % 512]!=10) and (buffer[currFile.currentPos % 512]!=13) )	{
				currFile.currentPos++;
				if ((currFile.currentPos % 512) == 0)
				{
					sec++;
					if (((currFile.currentPos/512) % BS.sectorsPerCluster)==0)
					{
						currFile.currentCluster=findNextCluster(currFile.currentCluster);
						sec=((uint32_t)BS.reservedSectors+((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+(((uint32_t)BS.rootDirectoryEntries*32)/512)+(((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster)+BS.hiddenSectors);
					}
					mmc::readSector(buffer, sec);
				}
			}
			if (currFile.currentPos>=currFile.fileSize) return lineCount;
			else if ((buffer[(currFile.currentPos % 512)]==13) and (buffer[(currFile.currentPos % 512)+1]==10))
			{
				currFile.currentPos+=2;
				lineCount++;;
			}
			else if ((buffer[(currFile.currentPos % 512)]==13) or (buffer[(currFile.currentPos % 512)]==10))
			{
				currFile.currentPos++;
				lineCount++;
			}
			else lineCount++;
		} while(true);
	}
	else return lineCount;
}
boolean fB_tFAT::readLn(char *st, int bufSize)
{
	uint32_t sec;
	int bufIndex=0;
	//dbug(F("TFAT  bs: %d  "),bufSize);

	for (int i=0; i<=bufSize; i++)	st[i]=0;

	if (currFile.fileMode==FILEMODE_TEXT_READ)
	{
		if ((currFile.currentPos==0) and (currFile.currentCluster==0))
			return FILE_IS_EMPTY;
		sec=((uint32_t)BS.reservedSectors+((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+(((uint32_t)BS.rootDirectoryEntries*32)/512)+(((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster)+BS.hiddenSectors)+(((uint32_t)currFile.currentPos/512) % (uint32_t)BS.sectorsPerCluster);
		mmc::readSector(buffer, sec);
		while ((currFile.currentPos<currFile.fileSize) and (buffer[currFile.currentPos % 512]!=10) and (buffer[currFile.currentPos % 512]!=13) and (bufIndex<bufSize))
		{
			st[bufIndex]=buffer[currFile.currentPos % 512];
			bufIndex++;
			currFile.currentPos++;
			if ((currFile.currentPos % 512) == 0)
			{
				sec++;
				if (((currFile.currentPos/512) % BS.sectorsPerCluster)==0)
				{
					currFile.currentCluster=findNextCluster(currFile.currentCluster);
					sec=((uint32_t)BS.reservedSectors+((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+(((uint32_t)BS.rootDirectoryEntries*32)/512)+(((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster)+BS.hiddenSectors);
				}
				mmc::readSector(buffer, sec);
			}
		}
		if (currFile.currentPos>=currFile.fileSize)	return false;	//return EOF;
		else if ((buffer[(currFile.currentPos % 512)]==13) and (buffer[(currFile.currentPos % 512)+1]==10))
		{
			currFile.currentPos+=2;
			return true;	//return bufIndex;
		}
		else if ((buffer[(currFile.currentPos % 512)]==13) or (buffer[(currFile.currentPos % 512)]==10))
		{
			return true;//return bufIndex;
		}
		else return true;	//return BUFFER_OVERFLOW;
	}
	else
		if (currFile.fileMode==0x00)return false; // return ERROR_NO_FILE_OPEN;
		else return false; //return ERROR_WRONG_FILEMODE;
}

uint16_t fB_tFAT::writeLn(char *st)
{
	unsigned long currSec = firstDirSector;
	uint16_t nextCluster = 0;
	word offset = -32;
	uint32_t sec;
	char tmpFN[13];
	int i, j;
	int bufIndex=0;
	boolean done=false;

	if (currFile.fileMode==FILEMODE_TEXT_WRITE)
	{
		if (currFile.currentCluster==0)
		{
			currFile.currentCluster=findFreeCluster();

			mmc::readSector(buffer, currSec);

			while (!done)
			{
				offset+=32;
				if (offset==512)
				{
				  currSec++;
				  mmc::readSector(buffer, currSec);
				  offset = 0;
				} 
				j=0;
				for (int i=0; i<8; i++)
				{
					if (buffer[i+offset]!=0x20)
					{
						tmpFN[j]=buffer[i+offset];
						j++;
					}
				}
				tmpFN[j]='.';
				j++;
				for (int i=0; i<3; i++)
				{
					if (buffer[i+0x08+offset]!=0x20)
					{
						tmpFN[j]=buffer[i+0x08+offset];
						j++;
					}
				}
				tmpFN[j]=0x00;
				
				if (!strcmp(tmpFN, currFile.filename))
				{
					buffer[offset+0x1A]=currFile.currentCluster & 0xFF;
					buffer[offset+0x1B]=currFile.currentCluster>>8;

					mmc::writeSector(buffer, currSec);

					mmc::readSector(buffer, BS.fat1Start+(currFile.currentCluster>>8));
					buffer[(currFile.currentCluster & 0xFF)*2]=0xFF;
					buffer[((currFile.currentCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat1Start+(currFile.currentCluster>>8));

					mmc::readSector(buffer, BS.fat2Start+(currFile.currentCluster>>8));
					buffer[(currFile.currentCluster & 0xFF)*2]=0xFF;
					buffer[((currFile.currentCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat2Start+(currFile.currentCluster>>8));

					done=true;
				}
			}

		}

		if ((((currFile.fileSize % 512)+strlen(st))<=510) and ((currFile.fileSize % (long(BS.sectorsPerCluster)*512)!=0) or (currFile.fileSize==0)))
		{
			currSec=(BS.reservedSectors+(BS.fatCopies*BS.sectorsPerFAT)+((BS.rootDirectoryEntries*32)/512)+((currFile.currentCluster-2)*BS.sectorsPerCluster)+BS.hiddenSectors)+((currFile.fileSize/512) % BS.sectorsPerCluster);
			mmc::readSector(buffer, currSec);
			for (int i=0; i<strlen(st); i++)
				buffer[(currFile.fileSize%512)+i]=st[i];
			buffer[(currFile.fileSize%512)+strlen(st)]=0x0D;
			buffer[(currFile.fileSize%512)+strlen(st)+1]=0x0A;
			mmc::writeSector(buffer, currSec);
		}
		else
		{
			currSec=(BS.reservedSectors+(BS.fatCopies*BS.sectorsPerFAT)+((BS.rootDirectoryEntries*32)/512)+((currFile.currentCluster-2)*BS.sectorsPerCluster)+BS.hiddenSectors)+((currFile.fileSize/512) % BS.sectorsPerCluster);

			if ((currFile.fileSize%512)!=0)
			{
				mmc::readSector(buffer, currSec);
				for (int i=0; i<(512-(currFile.fileSize%512)); i++)
				{
					buffer[(currFile.fileSize%512)+i]=st[i];
					bufIndex++;
				}
				mmc::writeSector(buffer, currSec);
				currSec++;
			}
			else
				bufIndex=0;

			if (((currSec-(BS.reservedSectors+(BS.fatCopies*BS.sectorsPerFAT)+((BS.rootDirectoryEntries*32)/512)+BS.hiddenSectors)) % BS.sectorsPerCluster)==0)
			{
				nextCluster=findFreeCluster();

				mmc::readSector(buffer, BS.fat1Start+(currFile.currentCluster>>8));
				buffer[(currFile.currentCluster & 0xFF)*2]=nextCluster & 0xFF;
				buffer[((currFile.currentCluster & 0xFF)*2)+1]=nextCluster>>8;
				if ((nextCluster>>8)==(currFile.currentCluster>>8))
				{
					buffer[(nextCluster & 0xFF)*2]=0xFF;
					buffer[((nextCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat1Start+(currFile.currentCluster>>8));
				}
				else
				{
					mmc::writeSector(buffer, BS.fat1Start+(currFile.currentCluster>>8));
					mmc::readSector(buffer, BS.fat1Start+(nextCluster>>8));
					buffer[(nextCluster & 0xFF)*2]=0xFF;
					buffer[((nextCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat1Start+(nextCluster>>8));
				}

				mmc::readSector(buffer, BS.fat2Start+(currFile.currentCluster>>8));
				buffer[(currFile.currentCluster & 0xFF)*2]=nextCluster & 0xFF;
				buffer[((currFile.currentCluster & 0xFF)*2)+1]=nextCluster>>8;
				if ((nextCluster>>8)==(currFile.currentCluster>>8))
				{
					buffer[(nextCluster & 0xFF)*2]=0xFF;
					buffer[((nextCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat2Start+(currFile.currentCluster>>8));
				}
				else
				{
					mmc::writeSector(buffer, BS.fat2Start+(currFile.currentCluster>>8));
					mmc::readSector(buffer, BS.fat2Start+(nextCluster>>8));
					buffer[(nextCluster & 0xFF)*2]=0xFF;
					buffer[((nextCluster & 0xFF)*2)+1]=0xFF;
					mmc::writeSector(buffer, BS.fat2Start+(nextCluster>>8));
				}

				currFile.currentCluster=nextCluster;

				currSec=(BS.reservedSectors+(BS.fatCopies*BS.sectorsPerFAT)+((BS.rootDirectoryEntries*32)/512)+((currFile.currentCluster-2)*BS.sectorsPerCluster)+BS.hiddenSectors);
			}
			mmc::readSector(buffer, currSec);
			for (int i=0; i<strlen(st)-bufIndex; i++)
				buffer[i]=st[i+bufIndex];
			buffer[strlen(st)-bufIndex]=0x0D;
			buffer[strlen(st)-bufIndex+1]=0x0A;
			mmc::writeSector(buffer, currSec);


		}

		currFile.fileSize+=(strlen(st)+2);

		currSec=firstDirSector;
		offset=-32;
		done=false;
		mmc::readSector(buffer, currSec);
		while (!done)
		{
			offset+=32;
			if (offset==512)
			{
			  currSec++;
			  mmc::readSector(buffer, currSec);
			  offset = 0;
			} 

			j=0;
			for (int i=0; i<8; i++)
			{
				if (buffer[i+offset]!=0x20)
				{
					tmpFN[j]=buffer[i+offset];
					j++;
				}
			}
			tmpFN[j]='.';
			j++;
			for (int i=0; i<3; i++)
			{
				if (buffer[i+0x08+offset]!=0x20)
				{
					tmpFN[j]=buffer[i+0x08+offset];
					j++;
				}
			}
			tmpFN[j]=0x00;

			if (!strcmp(tmpFN, currFile.filename))
			{
				buffer[offset+0x1C]=currFile.fileSize & 0xFF;
				buffer[offset+0x1D]=(currFile.fileSize & 0xFF00)>>8;
				buffer[offset+0x1E]=(currFile.fileSize & 0xFF0000)>>16;
				buffer[offset+0x1F]=currFile.fileSize>>24;

				mmc::writeSector(buffer, currSec);

				done=true;
			}
		}
		return NO_ERROR;
	}
	else
		if (currFile.fileMode==0x00)
			return ERROR_NO_FILE_OPEN;
		else
			return ERROR_WRONG_FILEMODE;
}

void fB_tFAT::closeFile()
{
	currFile.filename[0]=0x00;
	currFile.fileMode=0x00;
}


uint8_t  fB_tFAT::createFile(char *fn)
{
	unsigned long currSec;
	word offset = 0;
	boolean done=false;
	int j;

	for (int i=0; i<strlen(fn); i++)
	{
		fn[i]=uCase(fn[i]);
		if (!validChar(fn[i]))
		return ERROR_INVALID_NAME;
	}

	if (!(getFile(fn)==NO_ERROR))
	{

		currSec = firstDirSector;
		mmc::readSector(buffer, currSec);
		offset = -32;
		while (!done)
		{
			offset+=32;
			if (offset==512)
			{
			  currSec++;
			  mmc::readSector(buffer, currSec);
			  offset = 0;
			} 

			if ((buffer[offset]==0x00) or (buffer[offset]==0xE5))
			{
				for (int i=0; i<11; i++)
				{
					buffer[i+offset]=0x20;
				}
				j=0;
				for (int i=0; i<strlen(fn); i++)
				{
					if (fn[i]=='.')
						j=8;
					else
					{
						buffer[j+offset]=fn[i];
						j++;
					}
				}
				
				for (int i=0x0b; i<0x20; i++)
					buffer[offset+i]=0;
				buffer[offset+0x0b]=0x20;
				buffer[offset+0x0f]=0x60;
				buffer[offset+0x10]=0x21;
				buffer[offset+0x11]=0x3E;
				buffer[offset+0x12]=0x21;
				buffer[offset+0x13]=0x3E;
				buffer[offset+0x17]=0x60;
				buffer[offset+0x18]=0x21;
				buffer[offset+0x19]=0x3E;

				mmc::writeSector(buffer, currSec);
				done=true;
			}
		}
		getFile(fn); // to reset fat.DE records
		return NO_ERROR;
	}
	else return ERROR_CREATE_ERROR;
}

/* Private */

uint16_t fB_tFAT::findNextCluster(uint16_t cc)
{
	uint16_t nc;
	mmc::readSector(buffer, BS.fat1Start+int(cc/256));
	nc = buffer[(cc % 256)*2] + (buffer[((cc % 256)*2)+1]<<8);
	return nc;
}

char fB_tFAT::uCase(char c)
{
	if ((c>='a') && (c<='z'))
		return (c-0x20);
	else
		return c;
}

boolean fB_tFAT::validChar(char c)
{
	//char valid[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$&'()-@^_`{}~.";
	//for (int i=0; i<strlen(valid); i++)
	//	if (c==valid[i])
	//		return true;

	// saves heap
	uint8_t test;
	test= (uint8_t) c;
	if(test >63 && test < 91) return true;
	if(test >47 && test < 58) return true;
	switch(test) {
		case 33:
		case 35:
		case 36:
		case 38:
		case 39:
		case 40:
		case 41:
		case 45:
		case 46:
		case 94:
		case 96:
		case 123:
		case 125:
		case 126: return true;
	}
	return false;
}

uint16_t fB_tFAT::findFreeCluster()
{
	unsigned long currSec=0;
	word firstFreeCluster=0;
	word offset=0;

	while ((firstFreeCluster==0) and (currSec<=BS.sectorsPerFAT))
	{
		mmc::readSector(buffer, BS.fat1Start+currSec);
		while ((firstFreeCluster==0) and (offset<=512))
		{
			if ((buffer[offset] + (buffer[offset+1]<<8))==0)
				firstFreeCluster=(currSec<<8)+(offset/2);
			else
				offset+=2;
		}
		offset=0;
		currSec++;
	}
}

