#ifndef __KEY_EOL_HANDLER_H__
#define __KEY_EOL_HANDLER_H__

#include "prj_type.h"
#include "eol.h"


U8 IsValidEolKeyCondition(U16 mu16Key);

void* GetEolKeyEventList(void);

U8 GetEolKeyEventListSize(void);

#endif /* __KEY_EOL_HANDLER_H__ */
