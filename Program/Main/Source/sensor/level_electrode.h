/**
 * File : level_electrode.h
 * 
 * Level Electrode ( 전극봉 )
**/
#ifndef __LEVEL_ELECTRODE_H__
#define __LEVEL_ELECTRODE_H__

#include "prj_type.h"


typedef enum _level_elec_id_
{
    LEVEL_ELEC_ID_HOT_TANK = 0,   // 0 :
    LEVEL_ELEC_ID_MAX,
} ELevelElecId;

typedef enum _level_status_
{
    LEVEL_STATUS_NONE = 0,       // 0 :
    LEVEL_STATUS_UNDETECT,       // 0 :
    LEVEL_STATUS_DETECT,         // 0 :
} ELevelStatus;

void EnalbeLevelElec(void);
void DisableLevelElec(void);
U8 IsTurnOnLevelElec(void);

void SetLevelElecStatus( ELevelElecId xUid, ELevelStatus xEstatus );
ELevelStatus GetLevelElecStatus( ELevelElecId xUid );

void ProcessLevelElec(void);
void ProcessScanLevelElec(void);

#endif  /* __LEVEL_ELECTRODE_H__ */
