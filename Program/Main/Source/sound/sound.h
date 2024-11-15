#ifndef __SOUND_H__
#define __SOUND_H__

#include "prj_type.h"


typedef enum
{
    SOUND_POWER_OFF,          	/* Power Off (제품 공통 ) */
    SOUND_POWER_ON,             	/* Power On (제품 공통) */

    SOUND_EFFLUENT,             	/* 추출 시작 */
    SOUND_EFFLUENT_END,         	/* 추출 완료 */

    SOUND_CONFIG_SETUP,         	/* 냉각/온수 on */
    SOUND_CONFIG_CANCEL,        	/* 냉각/온수 off */

    SOUND_SELECT,               	/* 조절 (온/냉/정수 및 추출량 선택 시 ) */
    SOUND_ERROR,                	/* 효과음 _ 경고 */

    SOUND_SETUP,                	/* 부가 기능 설정 */
    SOUND_CANCEL,               	/* 부가 기능 해제 */

    SOUND_LOCK_SETUP,           	/* 잠금 설정 */
    SOUND_LOCK_CANCEL,          	/* 잠금 풀림 */

    SOUND_EFFLUENT_CONTINUE, 		/* 연속 추출 */
    SOUND_STERILIZE_START,   		/* 즉시 살균 시작음 */
    SOUND_STERILIZE_END,     		/* 즉시 살균 완료음 */

    SOUND_MEMENTO_1,         		/* 메멘토 1 */
    SOUND_MEMENTO_2,         		/* 메멘토 2 */
    SOUND_MEMENTO_3,         		/* 메멘토 3 */
    SOUND_MEMENTO_4,         		/* 메멘토 4 */
    SOUND_MEMENTO_5,         		/* 메멘토 5 */

    SOUND_MUTE,                     /* 부저음 셋업 전용 */
    SOUND_COOK_ALARM,               /* COOK TIMER 알람 */
    SOUND_COOK_ALARM_DONE,          /* COOK TIMER 알람 완료 음 */
    SOUND_NONE,			      		/* 부저음 발생 없음 */

    MAX_SOUND_NUM              
}SoundId_T;


/* 무음 설정 확인 */
U8	IsSetSoundMute(void);

/* 무음 설정  */
void	SetSoundMute(void);
U8    GetSoundMute(void);

/* 무음 해제 */
void	ClearSoundMute(void);

/* 소리 출력 */
void  Sound(SoundId_T mId);

/* 사운드 테스트 */
void TEST_Sound(void);

#endif /* _SOUND_H_ */
