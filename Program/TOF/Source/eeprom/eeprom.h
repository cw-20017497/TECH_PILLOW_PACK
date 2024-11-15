#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum
{
    /* CONFIG */
    EEP_ID_LOCK_ALL,
    EEP_ID_LOCK_HOT,
    EEP_ID_CONF_DECO_LED,
    EEP_ID_FACTORY_SETUP,

    EEP_ID_CONF_MAKE_COLD,      // 냉수 설정/해제
    EEP_ID_CONF_MAKE_HOT,       // 온수 설정/해제
    EEP_ID_CONF_POWER_SAVING,   // 절전 사용 설정
    EEP_ID_CONF_UNUSED_SAVE,    // 온수 미사용 절전 설정
    EEP_ID_HOT_ALTITUDE,        // 온수 고도 설정 

    EEP_ID_MEMENTO_1,           // 메멘토 에러 1
    EEP_ID_MEMENTO_2,           // 메멘토 에러 2
    EEP_ID_MEMENTO_3,           // 메멘토 에러 3
    EEP_ID_MEMENTO_4,           // 메멘토 에러 4
    EEP_ID_MEMENTO_5,           // 메멘토 에러 5

    EEP_ID_NUM
} EepromId_T;

void InitEeprom(void);

void SaveEepromId( EepromId_T mu8Id );

void EraseEeprom(void);

void ProcessEeprom(void);

U8 TestEeprom(void);

#endif /* __EEPROM_H__ */
