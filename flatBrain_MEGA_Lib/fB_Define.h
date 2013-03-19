/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
#ifndef FP_DEFINE_H
#define FP_DEFINE_H

#include <fB_SYS_Define.h>
#include <fB_USER_Define.h>

#define createPin(tag,cardTag,row,side,iodir,onval) definePin(tag,#tag,cardTag,row,side,iodir,onval)  
#define createCard(ctag,ctype,i2cAddr, aChan )  defineCard(ctag,#ctag,ctype,i2cAddr, aChan )  

#define createGlobal(gTag,value,format,fTag,flags) defineGlobal(gTag,value,#gTag,format,fTag,flags);
#define createGlobalPin(gTag,fTag,flags) defineGlobalPin(gTag,fTag,flags);

#define createLog(fTag) defineLog(fTag,#fTag);

//////////////////////

#define createSystem(mTag)   defineSystem(mTag,#mTag);
#define createGlobals(mTag)  defineGlobals(mTag,#mTag); 
#define createStacks(sTag)	defineStacks(sTag,#sTag);

#define createRow(tTag)		defineJrow(tTag,#tTag);
#define createPage(pTag)	definePage(pTag,#pTag,NULL);
#define createPage(pTag,tagStr)	definePage(pTag,tagStr,NULL);
#define stack(text,value)	brain.createStack(text,value);


//#define defineGlobal(gTag,fTag,value,flags) createGlobal(gTag,#gTag,fTag value,format,flags)

//#define createFiles(mTag)    defineFiles(mTag,#mTag);
//#define createDate(mTag)	 defineDate(mTag,#mTag);



//#define stack(text)			brain.createStack(text,NULL);
     //createDiagnostics(STKPAGE,"STACK");

	 //void dbug(char *fmt, ... );
	 //char* floatToStr(float value, int places,char *buffer);



#endif

