/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
#ifndef FP_DEFINE_H
#define FP_DEFINE_H

#include <fB_SYS_Define.h>
#include <fB_USER_Define.h>

#define definePin(tag,ctag,row,side,dir,onval)		initPin(tag,F(#tag),ctag,row,side,dir,onval) 
#define defineCard(ctag,ctype,i2cAddr, aChan )		initCard(ctag,F(#ctag),ctype,i2cAddr, aChan )  
#define defineLog(fTag)								initLog(fTag,F(#fTag));
#define defineTag(tag,flags)						initTag(tag,F(#tag),NULL,flags)
#define defineRecord(tag,fTag,flags)				initTag(tag,F(#tag),fTag,flags)

//												definePage(uint16_t pTag=NULL,const __FlashStringHelper* Ptitle=NULL, uint16_t parentTag = NULL)  {
#define defineHome(tag)							initPage(HOME,F(#tag))
#define definePage(tag,parentTag)				initPage(tag,F(#tag),parentTag)
#define defineRow(tag,format,flags,tTag)		initRow(tag,F(#tag),format,flags,tTag)
#define defineSpace()							initSpace()

 //												defineRow(uint16_t mTag, const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format,uint16_t tTag, float value = NULL)
#define defineJump(tag)							initJump(tag);
#define menuSpace()								menu.defineRow(NULL,NULL,NULL,NULL,NULL, NULL);

#define P(str) (strcpy_P(Pbuffer, PSTR(str)), Pbuffer)
#define createPstr(text) PstrRay[PstrCount++] = F(text); 




#endif

