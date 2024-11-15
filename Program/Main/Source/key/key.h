#ifndef __KEY_H__
#define __KEY_H__

#include "hal_key.h"

void InitKey(void);

/* SINGLE KEY */
#define  K_NONE         HAL_KEY_NONE
#define  K_HOT          HAL_KEY_2
#define  K_COLD         HAL_KEY_1
#define  K_ALL          HAL_KEY_ALL


U16 GetKeyVal( void );
U16 GetKeyPopVal( void );

/* EvnetFlag Index */
#define  KEY_EVENT_PUSHED_SHORT     0x01
#define  KEY_EVENT_PUSHED_LONG_5S   0x02
#define  KEY_EVENT_PUSHED_LONG_3S   0x04
#define  KEY_EVENT_PUSHED_LONG_2S   0x08
#define  KEY_EVENT_PUSHED_LONG_1S   0x10
#define  KEY_EVENT_POP              0x20
#define  KEY_EVENT_ALL              0x3F
void SetKeyEventFlag(U8 mu8Event );
void ClearKeyEventFlag(U8 mu8Event );
U8 IsSetKeyEventFlag( U8 mu8Event );

void ProcessScanKey(void);

#endif /* __KEY_H__ */
