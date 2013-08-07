/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
#ifndef FP_DEFINE_H
#define FP_DEFINE_H

#include <fB_SYS_Define.h>
#include <fB_USER_Define.h>

#define definePin(tag,ctag,row,side,dir,onval)	initPin(tag,F(#tag),ctag,row,side,dir,onval) 
#define defineCard(ctag,ctype,i2cAddr, aChan )  initCard(ctag,F(#ctag),ctype,i2cAddr, aChan )  
#define defineLog(fTag)							initLog(fTag,F(#fTag));
#define defineTag(tag,format,fTag,flags)		initTag(tag,F(#fTag),format,fTag,flags)
#define defineValue(tag,value,factor,offset)	initValue(tag,value,factor,offset)

//												definePage(uint16_t pTag=NULL,const __FlashStringHelper* Ptitle=NULL, uint16_t parentTag = NULL)  {
#define menuHome(pTag)							definePage(HOME,F(#pTag));
#define menuPage(pTag,parentTag)				definePage(pTag,F(#pTag),parentTag);

 //												defineRow(uint16_t mTag, const __FlashStringHelper* Ptitle,uint8_t  type,uint8_t  format,uint16_t tTag, float value = NULL)
#define menuJump(pTag)							defineRow(NULL,F(#pTag),JPAGE,NULL,pTag, NULL);
#define menuRowSpace()							defineRow(NULL,NULL,NULL,NULL,NULL, NULL);
#define menuRow(rTag,type,format,tTag,value)	defineRow(rTag,F(#rTag),type,format,tTag,value);

#define P(str) (strcpy_P(Pbuffer, PSTR(str)), Pbuffer)
#define createPstr(text) PstrRay[PstrCount++] = F(text); 




#endif

