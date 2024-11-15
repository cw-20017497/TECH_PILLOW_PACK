#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "prj_type.h"

/* Buffer index - ROW( 1 ~ 2 ) - 하위 2Byte */
#define ROW_0    ( 0x00000001 )
#define ROW_1    ( 0x00000002 )

/* Buffer index - COM( 1 ~ 2 ) - 상위 2Byte */
#define COM_0     ( 0x00000000 )
#define COM_1     ( 0x00010000 ) 


/* COM과 ROW bit 조합으로 정의 */
#define COLD_WATER_STRONG        ( COM_1 | ROW_0 )
#define HOT_WATER_STRONG         ( COM_0 | ROW_1 )
#define COLD_WATER_WEAK          ( COM_1 | ROW_1 )
#define HOT_WATER_WEAK           ( COM_0 | ROW_0 )


/* Functinos */
void DispColdWaterWeak( U8 mu8OnOff );
void DispColdWaterStrong( U8 mu8OnOff );

void DispHotWaterWeak( U8 mu8OnOff );
void DispHotWaterStrong( U8 mu8OnOff );

void DispVersion(U8 mu8Val);

/*****************************************************************************************
 FUNCTION NAME : void Evt_1msec_LED_Handler(void)

 DESCRIPTION
 *  -. 1msec interrupt에서 동작
 *  -. gu8WdataArray[] 값을 Matrix Select쪽에 Match

 PARAMETERS: void

 RETURN VALUE: NULL

 NOTES:
 
*****************************************************************************************/
void Evt_1msec_LED_Handler(void);

#endif /* __DISPLAY_H__ */
