#ifndef __RELAY_H__
#define __RELAY_H__

#include "prj_type.h"

void InitRelay(void);

#define RELAY_COMP            0x0001
#define RELAY_HEATER          0x0002
#define MAX_RELAY_NUM         2

void TurnOnRelay(U16 mu16Relay);
void TurnOffRelay(U16 mu16Relay);
void TurnOffRelayAll(void);

U8 IsTurnOnRelay(U16 mu16Relay);

U16 GetRelayStatus(void);

void ControlRelayExclusive(void);
void ControlRelayConcurrent(void);

void TEST_ControlRelay(void);

#endif /* __RELAY_H__ */
