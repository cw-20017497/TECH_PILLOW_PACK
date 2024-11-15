#ifndef __HEATER_H__
#define __HEATER_H__

#include "prj_type.h"

void InitHeater(void);

#define  HEATER_MODE_NONE     0
#define  HEATER_MODE_FORCED   1
void SetHeaterMode(U8 mu8Mode);
U8 GetHeaterMode(U8 mu8Mode);

void SetHeaterForcedOnOff(U8 mu8OnOff);
U8 GetHeaterOnOff(void);

U32   GetHeaterOnTime(void);
U32   GetHeaterOffTime(void);

void ControlHeater(U8 mu8OnOff);

#endif /* __HEATER_H__ */
