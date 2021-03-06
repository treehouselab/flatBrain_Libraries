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
#define defineTag(tag,flags)						initTag(tag,F(#tag),flags)
#define defineAlias(tag,alias)						initAlias(tag,F(#alias))
#define defineLog(tag,fTag)						initLog(tag,fTag,F(#fTag))
#define defineHome(tag)							initPage(HOME,F(#tag),NULL)
#define definePage(tag,parentTag)				initPage(tag,F(#tag),parentTag)
#define defineRow(tag,flags)					initRow(tag,F(#tag),flags)
#define defineRowList(tag,parentTag,flags)		initRowList(tag,F(#tag),parentTag,flags)
#define defineSpace()							initSpace()
#define defineJump(tag)							initJump(tag);

//#define _MOUNTED								_sysStatus & _SD

#define P(str) (strcpy_P(Pbuffer, PSTR(str)), Pbuffer)
#define createPstr(index,text) PstrRay[index] = F(text); 

#endif

