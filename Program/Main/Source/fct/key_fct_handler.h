#ifndef __KEY_FCT_HANDLER_H__
#define __KEY_FCT_HANDLER_H__

#include "prj_type.h"
#include "fct.h"


U8 IsValidFctKeyCondition(U16 mu16Key);

void* GetFctKeyEventList(void);

U8 GetFctKeyEventListSize(void);

#endif /* __KEY_FCT_HANDLER_H__ */
