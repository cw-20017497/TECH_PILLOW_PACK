#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum
{
    /* CONFIG */
    EEP_ID_CONF_MAKE_COLD,      // ?�?� ?�정/?�제
    EEP_ID_CONF_MAKE_HOT,       // ?�수 ?�정/?�제
    EEP_ID_CONF_NONE_1,
    EEP_ID_CONF_NONE_2,
    EEP_ID_HOT_ALTITUDE,        // ?�수 고?� ?�정 

    EEP_ID_MEMENTO_1,           // 메멘?� ?�??1
    EEP_ID_MEMENTO_2,           // 메멘?� ?�??2
    EEP_ID_MEMENTO_3,           // 메멘?� ?�??3
    EEP_ID_MEMENTO_4,           // 메멘?� ?�??4
    EEP_ID_MEMENTO_5,           // 메멘?� ?�??5

    EEP_ID_NUM
} EepromId_T;

void InitEeprom(void);

void SaveEepromId( EepromId_T mu8Id );

void EraseEeprom(void);

void ProcessEeprom(void);

U8 TestEeprom(void);

#endif /* __EEPROM_H__ */
