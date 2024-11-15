#include "error.h"
#include "eeprom.h"
#include "time_short.h"


Error_T  Err;

void InitError(void)
{
    memset( &Err, 0, sizeof( Error_T ));
}

void GetErrorData(Error_T *pData)
{
    memcpy( pData, &Err, sizeof( Error_T));
}

void SetErrorStatus(ErrorId_T mId, U8 mu8Status)
{
    if( mId < ERR_NUM ) 
    {
        Err.Status[ mId ] = mu8Status;
    }
}

void SetErrorStatusDate(ErrorId_T mId, U8 mu8Status, ErrorDate_T *pDate)
{
    if( mId < ERR_NUM ) 
    {
        Err.Status[ mId ] = mu8Status;
        memcpy( &Err.StatusDate[ mId ], pDate, sizeof( ErrorDate_T ));
    }
}

U8 GetErrorStatus(ErrorId_T mId)
{
    return Err.Status[ mId ];
}

void GetErrorStatusDate(ErrorId_T mId, ErrorDate_T *pDate)
{
    if( mId < ERR_NUM ) 
    {
        memcpy(  pDate, &Err.StatusDate[ mId ], sizeof( ErrorDate_T ));
    }
}

U8	IsError(ErrorId_T mId )
{
    return Err.Status[ mId ];
}

ErrorId_T	GetErrorId(void)
{
    return Err.ErrorId;
}

void SetErrorId(ErrorId_T mErrId)
{
    Err.ErrorId = mErrId;
}

U8  GetErrorDisplayId(ErrorId_T mErrId)
{
    U8 mu8ErrorCode = 0;

#if 0
    switch( mErrId )
    {
        case ERR_ROOM_LOW_LEVEL:        mu8ErrorCode = 11; break;
        case ERR_ROOM_HIGH_LEVEL:       mu8ErrorCode = 13; break;
        case ERR_ROOM_COMPLEX:          mu8ErrorCode = 14; break;
        case ERR_ROOM_OVF:              mu8ErrorCode = 10; break;
        case ERR_LEAK:                  mu8ErrorCode = 1; break;
        case ERR_TEMP_COLD_WATER:       mu8ErrorCode = 44; break;
        case ERR_TEMP_ROOM_WATER:       mu8ErrorCode = 42; break;
        case ERR_TEMP_AMBIENT:          mu8ErrorCode = 43; break;

        case ERR_DRAIN_PUMP:            mu8ErrorCode = 60; break;
        case ERR_TEMP_EVA_2:            mu8ErrorCode = 63; break;
        case ERR_TEMP_EVA_1:            mu8ErrorCode = 63; break;
        case ERR_COLD_LOW_LEVEL:        mu8ErrorCode = 20; break;
        case ERR_SODA_PUMP:             mu8ErrorCode = 94; break;
        case ERR_MICRO_SW_DETECT:       mu8ErrorCode = 61; break;
        case ERR_MICRO_SW_MOVE:         mu8ErrorCode = 62; break;

        //case ERR_COMM_FRONT_DISPLAY:    mu8ErrorCode = 11; break;
        //case ERR_COMM_LCD:              mu8ErrorCode = 11; break;
        default:                        mu8ErrorCode = 0; break;
                                        
    }

#endif
    return mu8ErrorCode;
}

void GetErrorDate(ErrorDate_T *pDate)
{
#if 0
    TimeData_T mTime;

    if( pDate != NULL )
    {
        GetRtcTime( &mTime );

        pDate->u16Year = mTime.Year + 2000;
        pDate->u8Month = mTime.Month;
        pDate->u8Date = mTime.Date;
    }
#endif
}

U8 IsEmptyMementoError(void)
{
    U8 mu8Num;
    Memento_T mMemento;

    mu8Num = MEMENTO_LIST_NUM - 1;
    GetMementoError( mu8Num, &mMemento );
    if( mMemento.Error == ERR_NONE )
    {
        return TRUE;
    }

    return FALSE;
}

U8	GetMementoError(U8 mu8Index, Memento_T *pMemento)
{
    if( mu8Index < MEMENTO_LIST_NUM )
    {
        memcpy( pMemento, &Err.MementoList[ mu8Index ], sizeof( Memento_T ));
        return TRUE;
    }

    return FALSE;
}

void	SetMementoError(U8 mu8Index, ErrorId_T mError, ErrorDate_T *pDate )
{
    if( mu8Index >= MEMENTO_LIST_NUM )
    {
        return ;
    }

    if( mError >= ERR_NUM )
    {
        return ;
    }

    Err.MementoList[ mu8Index ].Error = mError;
    memcpy( &Err.MementoList[ mu8Index ].Date, pDate, sizeof( ErrorDate_T ));
}

void ClearMementoError(void)
{
    U8 i;
    ErrorDate_T mDate;

    memset( &mDate, 0, sizeof( ErrorDate_T ));
    for( i = 0; i < MEMENTO_LIST_NUM ; i++ )
    {
        SetMementoError( i, ERR_NONE, &mDate );
        SaveEepromId( EEP_ID_MEMENTO_1 );
        SaveEepromId( EEP_ID_MEMENTO_2 );
        SaveEepromId( EEP_ID_MEMENTO_3 );
        SaveEepromId( EEP_ID_MEMENTO_4 );
        SaveEepromId( EEP_ID_MEMENTO_5 );
    }
}

static void UpdateMementoError( Memento_T *pMemento )
{
    if( pMemento->Error != ERR_NONE )
    {
        // e°∆°±e¢Æ©´ i°◊Å?? ?ê?°˛i¢‚¢Ê ?????ê?°˛e? e°∆¢‚?®˘e®œ¢• e®œ°±e®œ??? e|°˛i?¢¥i?¢¨i?ê
        // i¢“°±e°∆¢Ê??i°◊¢Ê ???°±?? 
        if( Err.MementoList[ MEMENTO_LIST_NUM - 1 ].Error != pMemento->Error )
        {
            // 0e©˜???e°∆¢Ê??e°◊?i°◊¢Êe°◊¢∂ ?ê??
            // 4e©˜???e°∆¢Ê??i•Ï©´e°§??ê??
            memcpy( &Err.MementoList[ 0 ], &Err.MementoList[1], sizeof( Memento_T ) * (MEMENTO_LIST_NUM - 1) ); 
            memcpy( &Err.MementoList[ MEMENTO_LIST_NUM - 1 ], pMemento, sizeof( Memento_T ) );

            SaveEepromId( EEP_ID_MEMENTO_1 );
            SaveEepromId( EEP_ID_MEMENTO_2 );
            SaveEepromId( EEP_ID_MEMENTO_3 );
            SaveEepromId( EEP_ID_MEMENTO_4 );
            SaveEepromId( EEP_ID_MEMENTO_5 );
        }
    }
}


/* Nos Valve ??e¢¨? ?ê??*/
U8 IsErrorTypeNosFeed(void)
{
#if 0
    if( IsError( ERR_ROOM_OVF ) == TRUE 
            || IsError( ERR_ROOM_COMPLEX ) == TRUE 
            || IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            //|| IsError( ERR_ROOM_LOW_LEVEL ) == TRUE 
            //|| IsError( ERR_LEAK ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
#else
    return FALSE;
#endif
}

#if 0
U8 IsErrorTypeColdWater(void)
{
    if( IsError( ERR_TEMP_COLD_WATER ) == TRUE 
            || IsError( ERR_ROOM_COMPLEX ) == TRUE
            || IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            || IsError( ERR_ROOM_OVF ) == TRUE 
            || IsError( ERR_ROOM_LOW_LEVEL ) == TRUE 
      )
    {
        return TRUE;
    }

    return FALSE;
}

U8 IsErrorTypeHotWater(void)
{

    if( IsError( ERR_TEMP_HOT_WATER ) == TRUE
            || IsError( ERR_ROOM_COMPLEX ) == TRUE
            || IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            || IsError( ERR_ROOM_OVF ) == TRUE 
            || IsError( ERR_ROOM_LOW_LEVEL ) == TRUE 
      )
    {
        return TRUE;
    }


    return FALSE;
}
#else
/* ≥√ºˆ ª˝º∫ ¡ﬂ¡ˆ ø°∑Ø */
U8 IsErrorTypeColdWater(void)
{
    // ≈◊Ω∫∆Æ ∏µÂø°º≠¥¬ ºˆ¿ß ºæº≠ ø°∑Ø πﬂª˝«ÿµµ ≥√∞¢ øÓ¿¸...
    if( GetTimeShortStatus() == FALSE )
    {
        if( 
	//IsError( ERR_ROOM_COMPLEX ) == TRUE
            //|| IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            //|| IsError( ERR_ROOM_OVF ) == TRUE 
            //|| 
	    IsError( ERR_ROOM_LOW_LEVEL ) == TRUE )
        {
            return TRUE;
        }
    }

    if( IsError( ERR_TEMP_COLD_WATER ) == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

/* ø¬ºˆ ª˝º∫ ¡ﬂ¡ˆ ø°∑Ø */
U8 IsErrorTypeHotWater(void)
{
    // ≈◊Ω∫∆Æ ∏µÂø°º≠¥¬ ºˆ¿ß ºæº≠ ø°∑Ø πﬂª˝«ÿµµ »˜∆√ øÓ¿¸...
    if( GetTimeShortStatus() == FALSE )
    {
        if( 
	//IsError( ERR_ROOM_COMPLEX ) == TRUE
            //|| IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            //|| IsError( ERR_ROOM_OVF ) == TRUE 
            //|| 
	    IsError( ERR_ROOM_LOW_LEVEL ) == TRUE )
        {
            return TRUE;
        }
    }

    if( IsError( ERR_TEMP_HOT_WATER ) == TRUE )
    {
        return TRUE;
    }


    return FALSE;
}
#endif

U8 IsErrorTypeIce(void)
{
#if 0
    if( IsError( ERR_TEMP_ROOM_WATER ) == TRUE
            || IsError( ERR_TEMP_AMBIENT ) == TRUE
            || IsError( ERR_ROOM_COMPLEX ) == TRUE
            || IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            || IsError( ERR_ROOM_OVF ) == TRUE 
            || IsError( ERR_ROOM_LOW_LEVEL ) == TRUE 
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
            || IsError( ERR_MICRO_SW_DETECT ) == TRUE 
            || IsError( ERR_MICRO_SW_MOVE ) == TRUE 
            || IsError( ERR_LEAK ) == TRUE 
      )
    {
        return TRUE;
    }
#endif

    return FALSE;
}

U8 IsErrorTypeSoda(void)
{
#if 0
    if( IsError( ERR_COLD_LOW_LEVEL ) == TRUE )
    {
        return TRUE;
    }

#endif
    return FALSE;
}

U8 IsErrorTypeDrainWater(void)
{
#if 0
    if(  IsError( ERR_ROOM_COMPLEX ) == TRUE
            || IsError( ERR_ROOM_HIGH_LEVEL ) == TRUE 
            || IsError( ERR_ROOM_OVF ) == TRUE 
            || IsError( ERR_ROOM_LOW_LEVEL ) == TRUE 
            || IsError( ERR_COLD_LOW_LEVEL ) == TRUE 
            || IsError( ERR_DRAIN_PUMP ) == TRUE 
            || IsError( ERR_LEAK ) == TRUE 
      )
    {
        return TRUE;
    }

#endif
    return FALSE;
}

U8 IsErrorTypeSter(void)
{
#if 0
    if( IsErrorTypeDrainWater() == TRUE )
    {
        return TRUE;
    }

    if( IsError( ERR_NFC_COMM ) == TRUE )
    {
        return TRUE;
    }
#endif

    return FALSE;
}



void UpdateNewError(U8 mu8Error)
{
    Memento_T mMemento;

#if 0
    // EVA ?ê?°˛e?°± Skip
    if( mu8Error == ERR_TEMP_EVA_1 
            || mu8Error == ERR_TEMP_EVA_2 )
    {
        return ;
    }
#endif

    if( Err.ErrorId != mu8Error ) 
    {
        Err.PrevErrorId = Err.ErrorId;

        mMemento.Error = mu8Error;
        GetErrorStatusDate( mMemento.Error, &mMemento.Date );

//        UpdateMementoError( &mMemento );
    }

    Err.ErrorId = mu8Error;
}

