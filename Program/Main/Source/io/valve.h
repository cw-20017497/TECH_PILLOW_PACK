#ifndef __VALVE_H__
#define __VALVE_H__

#include "prj_type.h"

#define VALVE_NOS               0x0001
#define VALVE_FEED              0x0002
#define VALVE_AIR               0x0004

#define MAX_VALVE_NUM           3

#define VALVE_ALL ( \
        VALVE_NOS \
        | VALVE_FEED \
        | VALVE_AIR \
        )

void InitValve(void);

void OpenValve(U16 mu16Valve);

void CloseValve(U16 mu16Valve);

U16 GetValveStatus(void);

U8 IsExpiredBootTime(void);

U8 IsOpenValve(U16 mu16Valve );

void ControlValve(void);

void TEST_ControlValve(void);

#endif /* __VALVE_H__ */
