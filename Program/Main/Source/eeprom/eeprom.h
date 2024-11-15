#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum
{
    /* CONFIG */
    EEP_ID_CONF_MAKE_COLD,      // ?â?ò ?§Ï†ï/?¥Ï†ú
    EEP_ID_CONF_MAKE_HOT,       // ?®Ïàò ?§Ï†ï/?¥Ï†ú
    EEP_ID_CONF_NONE_1,
    EEP_ID_CONF_NONE_2,
    EEP_ID_HOT_ALTITUDE,        // ?®Ïàò Í≥†?Ñ ?§Ï†ï 

    EEP_ID_MEMENTO_1,           // Î©îÎ©ò?† ?ê??1
    EEP_ID_MEMENTO_2,           // Î©îÎ©ò?† ?ê??2
    EEP_ID_MEMENTO_3,           // Î©îÎ©ò?† ?ê??3
    EEP_ID_MEMENTO_4,           // Î©îÎ©ò?† ?ê??4
    EEP_ID_MEMENTO_5,           // Î©îÎ©ò?† ?ê??5

    EEP_ID_NUM
} EepromId_T;

void InitEeprom(void);

void SaveEepromId( EepromId_T mu8Id );

void EraseEeprom(void);

void ProcessEeprom(void);

U8 TestEeprom(void);

#endif /* __EEPROM_H__ */
