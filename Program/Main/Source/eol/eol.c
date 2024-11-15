#include "eol.h"


#define  DEFAULT_LIMIT_TIME_VALUE      300U    /* 30sec @ 100msec */
#define  DEFAULT_COMM_ERR_TIMER        30U      /* 3sec, @100msec */

typedef struct _end_of_line_
{
    U8 Status;         /* START or STOP */
    U8 Type;           /* TYPE, EOL_TYPE_FRONT, EOL_TYPE_LOAD */
    U8 Mode;           /* READY, LOAD */
    
    /* CHECK INPUT */
    U32 InputTest;      /* Input Test Value */

    /* LOAD TEST */
    U8 mu8LoadComm;     /* Not Used */      /* START or STOP - ë¶€?˜ ê²€???µì‹  ?œ?‘ */       

    /* Error */
    U8 ErrComm;         /* Not Used */

    U16 TestTimer;      /* Eol Enable Timer 5min */
    U16 LimitTimer;     /* Enter LimitTimer 30sec */
    U16 CommErrTimer;   /* Not Used */

    /* CHECK STATUS */
    U16 CheckList[ EOL_CHK_ID_NUM ];
} Eol_T;

Eol_T eol;


void InitEol(void)
{
    eol.Status       = FALSE; 
    eol.LimitTimer   = DEFAULT_LIMIT_TIME_VALUE;
    eol.TestTimer    = DEFAULT_TEST_TIME_VALUE;
    eol.CommErrTimer = DEFAULT_COMM_ERR_TIMER;

    eol.Mode         = EOL_MODE_READY;

    SetEolCheckStatus( EOL_CHK_ID_EEPROM,  0xA5 );
}


void StartEol(U8 mu8Type)
{
    if( eol.LimitTimer == 0 )
    {
        return ;
    }

    eol.Status = TRUE;
    eol.Type = mu8Type;
}

U8 IsExpiredEolLimitTimer(void)
{
    if( eol.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

U8 GetEolStatus(void)
{
    return eol.Status;
}

U8 GetEolType(void)
{
    return eol.Type;
}

void SetEolMode(U8 mu8Mode)
{
    eol.Mode = mu8Mode;
}

U8 GetEolMode(void)
{
    return eol.Mode;
}

void SetEolTestTimer(U16 mu16Time)
{
    eol.TestTimer = mu16Time;
}

U16 GetEolTestTimer(void)
{
    return eol.TestTimer;
}



// Test Input
void SetEolTestInputBit(U32 mu32MaskBit)
{
    eol.InputTest |= mu32MaskBit;
}

void ClearEolTestInputBit(U32 mu32MaskBit)
{
    eol.InputTest &= ~mu32MaskBit;
}

void SetEolTestInputVal(U32 mu32MaskBit)
{
    eol.InputTest = mu32MaskBit;
}

U32 GetEolTestInputVal(void)
{
    return eol.InputTest;
}


// Error
void SetEolErrorComm(U8 mu8Error)
{
    eol.ErrComm = mu8Error;
}

U8 GetEolErrorComm(void)
{
    return eol.ErrComm;
}



U16 GetEolCheckStatus(U8 mu8Id )
{
    if( mu8Id >= EOL_CHK_ID_NUM )
    {
        return 0xFF; // index error
    }

    return eol.CheckList[ mu8Id ];
}

void SetEolCheckStatus(U8 mu8Id, U16 mu16Val)
{
    if( mu8Id >= EOL_CHK_ID_NUM )
    {
        return ;
    }

    eol.CheckList[ mu8Id ] = mu16Val;
}

void UpdateEolTimer(void)
{
    if( eol.LimitTimer != 0 )
    {
        eol.LimitTimer--;
    }

    if( eol.TestTimer != 0 )
    {
        eol.TestTimer--;
    }
}

