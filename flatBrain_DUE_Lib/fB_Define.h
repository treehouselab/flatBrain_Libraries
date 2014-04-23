/* This master file includes all the modules needed 
for the I/O expander backplane functionality. 
Treehouselab.com - Mike Price */
#ifndef FP_DEFINE_H
#define FP_DEFINE_H

#include <fB_SYS_Define.h>
#include <fB_USER_Define.h>

#define definePin(tag,ctag,row,side,dir,onval)		initPin(tag,#tag,ctag,row,side,dir,onval) 
#define defineCard(ctag,ctype,i2cAddr, aChan )		initCard(ctag,#ctag,ctype,i2cAddr, aChan )  
#define defineLog(fTag)								initLog(fTag,#fTag);
#define defineTag(tag,flags)						initTag(tag,#tag,flags)
#define defineAlias(tag,alias)						initAlias(tag,#alias)
#define defineLog(tag,fTag)						initLog(tag,fTag,#fTag)
#define defineHome(tag)							initPage(HOME,#tag,NULL)
#define definePage(tag,parentTag)				initPage(tag,#tag,parentTag)
#define defineRow(tag,flags)					initRow(tag,#tag,flags)
#define defineRowList(tag,parentTag,flags)		initRowList(tag,#tag,parentTag,flags)
#define defineSpace()							initSpace()
#define defineJump(tag)							initJump(tag);

//#define _MOUNTED								_sysStatus & _SD

#endif

