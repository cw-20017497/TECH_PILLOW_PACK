#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"


void InitDisplay(void);

/* 초�?FRONT 초�??��??�료 ?�?� */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

/* ?�수 고?� ?�정 */
U8 GetAltidueConfig(void);
void SetAltitudeConfig(U8 mu8Conf );

/* 미사�??�?� ?�정 */
U8 GetDispUnusedSaveConfig(void);
void SetDispUnusedSaveConfig(U8 mu8Conf );

/* ��� �ð� ���� */
U8 GetDispAdjustHourConfig(void);
void SetDispAdjustHourConfig(U8 mu8Conf );


/* ?�정 모드 진?� ?�?��??�?�머 */
U16 IsExpiredConfigTime(void);

/* 버?� ?�?� */
void SetVersionDisp(U8 mu8DispSec);

/* 메멘?� */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 mu8Conf );


/* ?�?�플?�???�??*/
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
