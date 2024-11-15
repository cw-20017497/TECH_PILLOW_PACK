#include "process_error.h"
#include "error.h"

#include "eeprom.h"
#include "level.h"
#include "time_short.h"

#include "err_temp_cold_water.h"
#include "err_temp_hot_water.h"
#include "err_room_level.h"



typedef U8 (*ErrFun_T)(U8 mu8Error);
typedef struct _error_list_
{
    ErrorId_T Id;           // 에러 ID
    U8 PrevError;           // 이전 에러 
    U8 Error;               // 현재 에러
    U8 SterActive;          // 살균 중 에러 표시
    U8 DispActive;          // 에러 표시 여부

    ErrFun_T Check;         // 에러 검출 함수
    ErrFun_T Release;       // 에러 해제 함수
} ErrorList_T;

void InitErrorList(void)
{
    InitError();
}

static U8 ReleaseErrRoomOvfEx(U8 mu8Error);
static U8 ReleaseErrRoomComplexEx(U8 mu8Error); 
static U8 ReleaseErrRoomHighEx(U8 mu8Error);
static U8 ReleaseErrRoomLowEx(U8 mu8Error);

// ???? ??전 조건???? ????검출
ErrorList_T ErrList[] = 
{
    /*ID                        PrevError Error   Ster      Disp      Check error           Check release error */
    { ERR_ROOM_COMPLEX,         FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomComplex,   ReleaseErrRoomComplexEx }, 
    { ERR_ROOM_HIGH_LEVEL,      FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomHigh,      ReleaseErrRoomHighEx }, 
    { ERR_ROOM_LOW_LEVEL,       FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomLow,       ReleaseErrRoomLowEx },
    { ERR_ROOM_OVF,             FALSE,    FALSE,  TRUE,     TRUE,    CheckErrRoomOvf,       ReleaseErrRoomOvfEx },  
    { ERR_TEMP_COLD_WATER,      FALSE,    FALSE,  TRUE,     TRUE,    CheckErrTempColdWater, ReleaseErrTempColdWater },
    { ERR_TEMP_HOT_WATER,       FALSE,    FALSE,  TRUE,     TRUE,    CheckErrTempHotWater,  ReleaseErrTempHotWater }
};
#define  SZ_ERR_LIST      (sizeof(ErrList)/sizeof(ErrorList_T))


static ErrorId_T FindCheckError(  ErrorList_T *pErrList, U16 mu16Size )
{
    U8 i;
    ErrFun_T pCheck;
    ErrFun_T pRelease;
    ErrorId_T mId = ERR_NONE;
    ErrorId_T mNewId = ERR_NONE;
    U8 mu8Error = FALSE;
 //   ErrorDate_T mDate;
    ErrorList_T *pList = NULL;


//    GetErrorDate( &mDate );
    for( i = 0; i < mu16Size; i++ ) 
    {
        pList = (pErrList + i );

        // Mapping error status..
        mId      = pList->Id;
        mu8Error = pList->Error;
        pCheck   = pList->Check;
        pRelease = pList->Release;

#if CONFIG_STER
        if( IsStartSter() == TRUE )
        {
            if( pList->SterActive == FALSE )
            {
                continue;
            }
        }
#endif

        if( mu8Error == TRUE )
        {
#if CONFIG_STER
            if( GetSterErrWaterOut() == FALSE )
#endif
            {
                if( pRelease != NULL )
                {
                    mu8Error = pRelease( mu8Error );
                }
            }
        }
        else
        {
            if( pCheck != NULL )
            {
                mu8Error = pCheck( mu8Error );
            }
        }

        // Update new error status
        if( pList->PrevError != mu8Error )
        {
            pList->PrevError = pList->Error;
            pList->Error = mu8Error;

            SetErrorStatus( mId, pList->Error );
            //SetErrorStatusDate( mId, pList->Error, &mDate );
        }

        if( pList->DispActive == TRUE )
        {
            //  Top priority Error
            if( GetErrorStatus( mId ) == TRUE )
            {
                mNewId = mId;
            }
        }

    }

    return mNewId;
}


void ProcessError(void)
{
#if CONFIG_ERR_ALL
    ErrorId_T mNewId = ERR_NONE;

    // Check Error
    mNewId = FindCheckError( &ErrList[0], SZ_ERR_LIST );

    // Update New Error Id
    UpdateNewError( mNewId );
#endif  // CONFIG_ERR_ALL
}


static U8 ReleaseErrRoomOvfEx(U8 mu8Error)
{
    if( GetTimeShortStatus() == FALSE )
    {
        return mu8Error;
    }

    return ReleaseErrRoomOvf( mu8Error );
}

static U8 ReleaseErrRoomComplexEx(U8 mu8Error)
{
    if( GetTimeShortStatus() == FALSE )
    {
        return mu8Error;
    }

    return ReleaseErrRoomComplex( mu8Error );
}

static U8 ReleaseErrRoomHighEx(U8 mu8Error)
{
    if( GetTimeShortStatus() == FALSE )
    {
        return mu8Error;
    }

    return ReleaseErrRoomHigh( mu8Error );
}

static U8 ReleaseErrRoomLowEx(U8 mu8Error)
{
    if( GetTimeShortStatus() == FALSE )
    {
        return mu8Error;    
    }

    return ReleaseErrRoomLow( mu8Error );
}
