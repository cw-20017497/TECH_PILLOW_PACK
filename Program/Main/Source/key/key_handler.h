#ifndef __KEY_HANDLER_H__
#define __KEY_HANDLER_H__

#include "prj_type.h"

/* MULTI KEY */
//#define K_VERSION       ( K_COLD | K_HOT )
//#define K_MEMENTO       ( K_ICE_LOCK | K_ICE_MAKE )
#define K_SPECIAL       ( K_HOT | K_COLD )

/* FUNCTION KEY - TEST */
//#define K_TEST_FCT      ( K_HOT  | K_SAVE )          // TEST MAIN PBA
//#define K_TEST_EOL      ( K_COLD | K_HOT ) 



U8 IsValidKeyCondition(U16 mu16Key);
void* GetKeyEventList(void);

U8 GetKeyEventListSize(void);

#endif /* __KEY_HANDLER_H__ */
