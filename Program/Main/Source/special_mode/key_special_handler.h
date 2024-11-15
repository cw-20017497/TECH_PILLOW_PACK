#ifndef __KEY_SPECIAL_HANDLER_H__
#define __KEY_SPECIAL_HANDLER_H__

#include "prj_type.h"
#include "special_mode.h"


U8 IsValidSpecialKeyCondition(U16 mu16Key);

void* GetSpecialKeyEventList(void);

U8 GetSpecialKeyEventListSize(void);

#endif /* __KEY_EOL_HANDLER_H__ */
