#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"


void InitDisplay(void);

/* Ï¥àÍ∏?FRONT Ï¥àÍ∏??êÎ©??ÑÎ£å ?Å?ú */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

/* ?®Ïàò Í≥†?Ñ ?§Ï†ï */
U8 GetAltidueConfig(void);
void SetAltitudeConfig(U8 mu8Conf );

/* ÎØ∏ÏÇ¨Ïö??à?Ñ ?§Ï†ï */
U8 GetDispUnusedSaveConfig(void);
void SetDispUnusedSaveConfig(U8 mu8Conf );

/* ªÏ±’ Ω√∞£ ∫∏¡§ */
U8 GetDispAdjustHourConfig(void);
void SetDispAdjustHourConfig(U8 mu8Conf );


/* ?§Ï†ï Î™®Îìú ÏßÑ?Ö ?†?®ÏÑ??Ä?¥Î®∏ */
U16 IsExpiredConfigTime(void);

/* Î≤Ñ?Ñ ?ú?ú */
void SetVersionDisp(U8 mu8DispSec);

/* Î©îÎ©ò?† */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 mu8Conf );


/* ?î?§Ìîå?à???ú??*/
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
