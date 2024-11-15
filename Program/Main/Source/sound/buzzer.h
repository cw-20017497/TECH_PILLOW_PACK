#ifndef __BUZZER_H__
#define __BUZZER_H__

#include "prj_type.h"


/* MAIN 주파수 설정 */
#define	OSC_8MHZ    0
#define	OSC_16MHZ	0
#define	OSC_32MHZ	1

#if (OSC_8MHZ == 1)

/* 4옥타브 */
#define Do4       (15288U)
#define Dos4      (14430U)
#define Re4       (13620U)
#define Mi4       (12134U)
#define Pa4       (11453U)
#define Pas4      (10810U)
#define Sol4      (10203U)
#define Sols4     (9630U)
#define La4       (9090U)
#define Si4       (8098U)

#elif (OSC_16MHZ == 1)

/* 4옥타브 */
#define Do4       (30577U)
#define Dos4      (28861U)
#define Re4       (27241U)
#define Mi4       (24269U)
#define Pa4       (22907U)
#define Pas4      (21621U)
#define Sol4      (20407U)
#define Sols4     (19262U)
#define La4       (18181U)
#define Si4       (16197U)

#else // OSC_32MHZ
/* 4옥타브 */
#define Do4       (61155U)
#define Dos4      (57723U)
#define Re4       (54483U)
#define Mi4       (48539U)
#define Pa4       (45814U)
#define Pas4      (43243U)
#define Sol4      (40816U)
#define Sols4     (38525U)
#define La4       (36363U)
#define Si4       (32395U)
#endif   

/* 5옥타브 */
#define Do5       (Do4 / 2 )
#define Dos5      (Dos4 / 2 ) 
#define Re5       (Re4 / 2)    
#define Mi5       (Mi4 / 2)  
#define Pa5       (Pa4 / 2)  
#define Pas5      (Pas4 / 2)  
#define Sol5      (Sol4 / 2) 
#define Sols5     (Sols4 / 2) 
#define La5       (La4 / 2)
#define Si5       (Si4 / 2)      

/* 6 옥타브 */
#define Do6       (Do4 / 4 )
#define Dos6      (Dos4 / 4 ) 
#define Re6       (Re4 / 4)    
#define Mi6       (Mi4 / 4)  
#define Pa6       (Pa4 / 4)  
#define Pas6      (Pas4 / 4)  
#define Sol6      (Sol4 / 4) 
#define Sols6     (Sols4 / 4) 
#define La6       (La4 / 4)
#define Si6       (Si4 / 4)      

/* 7 옥타브 */
#define Do7       (Do4 / 8 )
#define Dos7      (Dos4 / 8 ) 
#define Re7       (Re4 / 8)    
#define Mi7       (Mi4 / 8)  
#define Pa7       (Pa4 / 8)  
#define Pas7      (Pas4 / 8)  
#define Sol7      (Sol4 / 8) 
#define Sols7     (Sols4 / 8) 
#define La7       (La4 / 8)
#define Si7       (Si4 / 8)      



/* 부저 초기화 */
void	InitBuzzer(void);

/* 부저 무음 */
U8		GetBuzMute(void);
void	SetBuzMute(U8 mu8Mute );
void	BuzToggleMute(void);

/* 부저 설정 */
#define BUZZER_OFF                  0		/* Power On (제품 공통 ) */
#define BUZZER_POWER_ON             1		/* Power Off (제품 공통) */
#define BUZZER_EFFLUENT             2		/* 추출 시작 */
#define BUZZER_EFFLUENT_END         3		/* 추출 완료 */
#define BUZZER_COOL_SETUP           4		/* 냉각/온수 on */
#define BUZZER_COOL_CANCEL          5		/* 냉각/온수 off */
#define BUZZER_SELECT               6		/* 조절 (온/냉/정수 및 추출량 선택 시 ) */
#define BUZZER_ERROR                7		/* 효과음 _ 경고 */
#define BUZZER_SETUP                8		/* 잠금 설정 ( 부가 기능 설정 ) */
#define BUZZER_CANCEL               9		/* 잠금 풀림 ( 부가 기능 해제 ) */
#define BUZZER_EFFLUENT_CONTINUE    10		/* 연속 추출 */
#define BUZZER_STERILIZE_START      11		/* 즉시 살균 시작음 */
#define BUZZER_STERILIZE_END        12		/* 즉시 살균 완료음 */
#define BUZZER_MEMENTO_1            13		/* 메멘토 1 */
#define BUZZER_MEMENTO_2            14		/* 메멘토 2 */
#define BUZZER_MEMENTO_3            15		/* 메멘토 3 */
#define BUZZER_MEMENTO_4            16		/* 메멘토 4 */
#define BUZZER_MEMENTO_5            17		/* 메멘토 5 */
#define BUZZER_MUTE                 18      /* 부저음 셋업 전용 */
#define BUZZER_MEMENTO_1_MUTE       19		/* 메멘토 1 - COOK TIMER */
#define BUZZER_MEMENTO_5_MUTE       20		/* 메멘토 5 - COOK TIMER */
#define BUZZER_NONE			        21		/* 음 종료 */
void	BuzStep(U8 mu8Step);

U8 IsDonePlayBuz(void);

/* 부저 타이머 10ms */
void	BuzTimeCounter(void);

/* 부저 제어 1ms */
void	BuzControl(void);

#endif /* _FUNC_BUZZER_H_ */
