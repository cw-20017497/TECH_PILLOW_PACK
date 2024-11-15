#ifndef __COLD_WATER_H__
#define __COLD_WATER_H__

#include "prj_type.h"
#include "level.h"


/* COLD IN EXTRA FEED TIME WHEN HIGH LEVEL */
#define EXTRA_FEED_TIME    180U /* 18sec @100ms */

typedef struct _cold_water_
{
    U8 InitFull;
    U8 Level;
    
    /* MAKE */
    U8 ConfigMake;
    U8 Make;

    /* MODE */
    U8 Mode;

    /* TEMP */
    TEMP_T  TempTargetOn;
    TEMP_T  TempTargetOff;
    TEMP_T  TempCurrent;

    U16 FeedTime;   
    U16 ExtraFeedTime;  // ¸¸¼öÀ§ °¨Áö ÈÄ, Ãß°¡ ÀÔ¼ö ½Ã°£

    U8 ExtraMake;       // A©¬¡Æ¢® ©øA¡Æ¡Ë flag
    U16 ExtraMakeTime;  // A©¬¡Æ¢® ©øA¡Æ¡Ë ¨öA¡Æ¡Ì
    U16 ExtraMakeCheckTime; // ©øA¡Æ¡Ë 1¨¬¨¢ Timer: A©¬¡Æ¢® ©øA¡Æ¡Ë ¨öA¡Æ¡Ì E¢çA¢´
    
} ColdWater_T;

// ÃÊ±âÈ­
void  InitColdWater(void);
void  GetColdWaterData(ColdWater_T *pData);

// ³Ã¼ö ÃÖÃÊ ¸¸¼öÀ§
void  SetColdWaterInitFull(U8 mu8Val );
U8    GetColdWaterInitFull(void);

// ³Ã¼ö ¼öÀ§ 
void  SetColdWaterLevel(U8 mu8Val );
U8    GetColdWaterLevel(void);

// ³Ã¼ö »ı¼º ¼³Á¤
void  SetColdWaterConfigMake(U8 mu8Val );
U8    GetColdWaterConfigMake(void);

// ³Ã¼ö »ı¼º ¸í·É
void  SetColdWaterMake(U8 mu8Val );
U8    GetColdWaterMake(void);

// ³Ã¼ö ÀÔ¼ö Áö¿¬ ½Ã°£
void  SetColdWaterFeedTime(U16 mu16Time );
U16   GetColdWaterFeedTime(void);

// ³Ã¼ö Á¦¾î »ó¼¼ ¸ğµå
#define  COLD_MODE_NONE          0x00
#define  COLD_MODE_INIT          0x01
#define  COLD_MODE_COOLING       0x02
#define  COLD_MODE_DONE          0x08
#define  COLD_MODE_ALL           0xFF
void SetColdWaterMakeMode(U8 mu8Mode );
void ClearColdWaterMakeMode(U8 mu8Mode );
U8 IsSetColdWaterMakeMode(U8 mu8Mode );
U8 GetColdWaterMakeMode(void );


// ³Ã¼ö ÀÔ¼ö Áö¿¬ ½Ã°£
void  SetColdWaterExtraMakeTime(U16 mu16Time );
U16   GetColdWaterExtraMakeTime(void);

// ³Ã¼ö ÅÊÅ© ¼öÀ§ Á¦¾î
void  ControlColdWaterLevel(void);

TEMP_T   GetTargetColdOnTemp(void);
TEMP_T   GetTargetColdOffTemp(void);

// ?¢¶?? ???i¢®¡Æe¡¾¢¥ ?©«??
void  MakeColdWater(void);

#endif /* __COLD_WATER_H__ */
