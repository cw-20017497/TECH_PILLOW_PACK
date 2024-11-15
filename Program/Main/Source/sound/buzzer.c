#include "hw.h"
#include "buzzer.h"



/* TIMER */
#define	P_BUZZER_ON		         P_BUZZER_ONOFF
#define	BUZ_SET_NOTE( x )	      do{ TDR11 = (x); }while(0)
#define  BUZZER_TIMER_START()    do{ R_TAU1_Channel1_Start(); }while(0)
#define  BUZZER_TIMER_STOP()     \
    do{ \
        R_TAU1_Channel1_Stop(); \
        TO1 |= _0002_TAU_CH1_OUTPUT_VALUE_1; \
    }while(0)

/* Local Variable */
U8	gu8Mute  = 0;           // 음소거모드 flag
U8	gu8Buzzer_Order = 0;
U8 	gu8Buzzer_Mode  = 0;
U16 gu16Buzzer_Time  = 0;

static void Buz_Start(void)
{
    BUZZER_TIMER_START();
}

static void Buz_Stop(void)
{
    BUZZER_TIMER_STOP();
}

/// @brief    Buzzer Initialize
/// @param    void
/// @return   void
void InitBuzzer(void)
{
	gu8Mute	= 0;           // 음소거모드 flag
	gu8Buzzer_Order	= 0;
	gu8Buzzer_Mode	= 0;
	gu16Buzzer_Time	= 0;  

	Buz_Stop();     
	BUZ_SET_NOTE( 0 );
}

U8	GetBuzMute(void)
{
	return gu8Mute;
}

void SetBuzMute(U8 mu8Mute )
{
	gu8Mute = mu8Mute;
}

void BuzToggleMute(void)
{
	gu8Mute = !gu8Mute;
}


static void SetBuzTime(U16 mu16Time )
{
    gu16Buzzer_Time = mu16Time * 10;
}

/// @brief    Buzzer 종류 설정 함수
/// @param    mu8Step : Buzzer 종류
/// @return   void
void BuzStep(U8 mu8Step)
{
	gu16Buzzer_Time = 0;
	gu8Buzzer_Order = 1;
	gu8Buzzer_Mode = mu8Step;
}

U8 IsDonePlayBuz(void)
{
	if( gu8Buzzer_Order != 0 )
   {
       return FALSE;
   }

   return TRUE;
}


/// @brief    Buzzer Time Count 제어 함수(10ms)
/// @param    void
/// @return   void
void BuzTimeCounter(void)
{
	if (gu16Buzzer_Time != 0)
	{ 
		gu16Buzzer_Time--;
	}
}

/// @brief    Buzzer 제어 함수(1ms)
/// @param    void
/// @return   void
void BuzControl(void)
{
    if (!gu8Mute)
    {
        switch (gu8Buzzer_Mode)
        {
            case BUZZER_OFF:
                P_BUZZER_ON = OFF;
                gu16Buzzer_Time = 0;
                Buz_Stop();
                BUZ_SET_NOTE( 0 );
                break;

            case BUZZER_POWER_ON:
                // Power On ************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Si5 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 4 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 4 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 4 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Pas6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 5 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 65 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 2 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 112 );
                }
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;   

            case BUZZER_EFFLUENT:
                // 추출시작음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_EFFLUENT_END:
                // 추출종료음 **********************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La5 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Pas6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 30 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_COOL_SETUP:
                // 냉각 ON음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Pas6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 5 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_COOL_CANCEL:
                // 냉각 OFF음 **********************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sols5 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Si5 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Mi5 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_SELECT:
                // 선택음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 28 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_ERROR:
                // 에러(경고)음 ******gu8Buzzer_Order****************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 2 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 28 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 2 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 28 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break; 

            case BUZZER_SETUP:
            case BUZZER_MUTE:
                // 설정음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 5 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_CANCEL:
                // 해제음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 5 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;        

            case BUZZER_EFFLUENT_CONTINUE:
                // 연속추출음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Dos7 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }   
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                } 
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Mi6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La5 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 12) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                } 
                if ((gu8Buzzer_Order == 13) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 14) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }   
                if ((gu8Buzzer_Order == 15) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La6 );
                    Buz_Start();
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if ((gu8Buzzer_Order == 16) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 20 );
                }        
                if ((gu8Buzzer_Order == 17) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;       

            case BUZZER_STERILIZE_START:
                // 살균시작음 **********************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Mi6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Dos6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_STERILIZE_END:
                // 살균종료음 **********************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( La6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 28 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Dos6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 8 );
                }
                if((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 28 );
                }        
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Dos6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 3 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 21 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Pas6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Mi6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 14 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }        
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;        

            case BUZZER_MEMENTO_1:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_2:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_3:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_4:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_5:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_1_MUTE:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_5_MUTE:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;
            default:
                break;
        }
    }
    else    // 음소거모드 동작
    {
        switch( gu8Buzzer_Mode )
        {
            case BUZZER_MUTE:
                // 설정음 **************************************************************
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 5 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 19 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Re7 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 38 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_1_MUTE:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            case BUZZER_MEMENTO_5_MUTE:
                if ((gu8Buzzer_Order == 1) && (gu16Buzzer_Time == 0))
                {
                    BUZ_SET_NOTE( Sol6 );
                    Buz_Start();  //buzzer start
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 2) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 3) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 4) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 5) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 6) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 7) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 8) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 10 );
                }
                if ((gu8Buzzer_Order == 9) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = ON;
                    gu8Buzzer_Order++;
                    SetBuzTime( 6 );
                }
                if ((gu8Buzzer_Order == 10) && (gu16Buzzer_Time == 0))
                {
                    P_BUZZER_ON = OFF;
                    gu8Buzzer_Order++;
                    SetBuzTime( 50 );
                }
                if ((gu8Buzzer_Order == 11) && (gu16Buzzer_Time == 0))
                {
                    gu8Buzzer_Order = 0;
                    SetBuzTime( 0 );
                    gu8Buzzer_Mode = 0;
                    Buz_Stop();
                    BUZ_SET_NOTE( 0 );
                }
                break;

            default:
                P_BUZZER_ON = OFF;
                gu8Buzzer_Order = 0;
                SetBuzTime( 0 );
                gu8Buzzer_Mode = 0;
                Buz_Stop();    
                BUZ_SET_NOTE( 0 );
                break;
        }
    }

    BuzTimeCounter();
}


