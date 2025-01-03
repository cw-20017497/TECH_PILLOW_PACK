#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"


void InitDisplay(void);

/* μ΄κΈ?FRONT μ΄κΈ??λ©??λ£ ?? */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

/* ?¨μ κ³ ? ?€μ  */
U8 GetAltidueConfig(void);
void SetAltitudeConfig(U8 mu8Conf );

/* λ―Έμ¬μ??? ?€μ  */
U8 GetDispUnusedSaveConfig(void);
void SetDispUnusedSaveConfig(U8 mu8Conf );

/* »μ±Υ ½Γ°£ ΊΈΑ€ */
U8 GetDispAdjustHourConfig(void);
void SetDispAdjustHourConfig(U8 mu8Conf );


/* ?€μ  λͺ¨λ μ§? ? ?¨μ???΄λ¨Έ */
U16 IsExpiredConfigTime(void);

/* λ²? ?? */
void SetVersionDisp(U8 mu8DispSec);

/* λ©λ©?  */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 mu8Conf );


/* ??€ν??????*/
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
