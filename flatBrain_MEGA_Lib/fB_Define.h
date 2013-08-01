/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
#ifndef FP_DEFINE_H
#define FP_DEFINE_H

#include <fB_SYS_Define.h>
#include <fB_USER_Define.h>

#define createPin(tag,cardTag,row,side,iodir,onval) definePin(tag,F(#tag),cardTag,row,side,iodir,onval)  
#define createCard(ctag,ctype,i2cAddr, aChan )  defineCard(ctag,F(#ctag),ctype,i2cAddr, aChan )  

#define createGlobal(gTag,value,format,fTag,flags) defineGlobal(gTag,value,F(#gTag),format,fTag,flags);
#define createGlobalPin(gTag,format,fTag,flags) defineGlobalPin(gTag,format,fTag,flags);

#define createLog(fTag) defineLog(fTag,F(#fTag));
#define createPstr(text) PstrRay[PstrCount++] = F(text);  // make sure sTag never exceeds MAXPSTRCOUNT

//#define createSystem(mTag)   defineSystem(mTag,F(#mTag));
//#define createGlobals(mTag)  defineGlobals(mTag,F(#mTag)); 
#define createStacks(sTag)	 defineStacks(sTag,F(#sTag));

#define createRow(rTag)						defineJrow(rTag,F(#rTag));
#define createArow(rTag,atype,tTag,value)	defineRow(rTag,F(#rTag),atype,tTag,NULL,value)
#define createPage(pTag)					definePage(pTag,F(#pTag),NULL);
#define createHome(pTag)	definePage(HOME,F(#pTag),NULL);
#define createStack(text,value)	brain.defineStack(F(text),value);
#define P(str) (strcpy_P(Pbuffer, PSTR(str)), Pbuffer)
//#define defineGlobal(gTag,fTag,value,flags) createGlobal(gTag,#gTag,fTag value,format,flags)

//#define createFiles(mTag)    defineFiles(mTag,#mTag);
//#define createDate(mTag)	 defineDate(mTag,#mTag);



//#define stack(text)			brain.createStack(text,NULL);
     //createDiagnostics(STKPAGE,"STACK");

	 //void dbug(char *fmt, ... );
	 //char* floatToStr(float value, int places,char *buffer);



#endif

