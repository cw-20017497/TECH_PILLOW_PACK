#include "sound.h"
//#include "buzzer.h"
#include "eeprom.h"

typedef struct _sound_
{
    U8   Mute;
} Sound_T;

Sound_T sound;

typedef struct _sound_list_t
{
    SoundId_T   SoundId;
    U8   BuzzerId;
} SoundList_T;

SoundList_T  SoundList[] = 
{
#if 0
    /* SOUND ID */             /* BUZZER ID */
    { SOUND_POWER_OFF         , BUZZER_OFF },
    { SOUND_POWER_ON          , BUZZER_POWER_ON },
    { SOUND_EFFLUENT          , BUZZER_EFFLUENT },
    { SOUND_EFFLUENT_END      , BUZZER_EFFLUENT_END },       
    { SOUND_CONFIG_SETUP      , BUZZER_COOL_SETUP },         
    { SOUND_CONFIG_CANCEL     , BUZZER_COOL_CANCEL },        
    { SOUND_SELECT            , BUZZER_SELECT },             
    { SOUND_ERROR             , BUZZER_ERROR },              
    { SOUND_SETUP             , BUZZER_SETUP },              
    { SOUND_CANCEL            , BUZZER_CANCEL },             
    { SOUND_LOCK_SETUP        , BUZZER_SETUP },              
    { SOUND_LOCK_CANCEL       , BUZZER_CANCEL },             
    { SOUND_EFFLUENT_CONTINUE , BUZZER_EFFLUENT_CONTINUE },  
    { SOUND_STERILIZE_START   , BUZZER_STERILIZE_START },    
    { SOUND_STERILIZE_END     , BUZZER_STERILIZE_END },      
    { SOUND_MEMENTO_1         , BUZZER_MEMENTO_1 },          
    { SOUND_MEMENTO_2         , BUZZER_MEMENTO_2 },          
    { SOUND_MEMENTO_3         , BUZZER_MEMENTO_3 },          
    { SOUND_MEMENTO_4         , BUZZER_MEMENTO_4 },          
    { SOUND_MEMENTO_5         , BUZZER_MEMENTO_5 },          
    { SOUND_MUTE              , BUZZER_MUTE },               
    { SOUND_COOK_ALARM        , BUZZER_MEMENTO_1_MUTE },               
    { SOUND_COOK_ALARM_DONE   , BUZZER_MEMENTO_5_MUTE },               
//    { SOUND_NONE	      , BUZZER_NONE }					
#endif
    { 0,       0 }
};
#define	SZ_LIST		( sizeof( SoundList ) / sizeof( SoundList_T ) )

void Sound(SoundId_T mId)
{
#if 0
    U8   i;

    // Check Buzzer playing sound...
    //if( IsDonePlayBuz() == FALSE )
    //{
    //    return ;  // waiting done to play buzzer
    //}

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( SoundList[ i ].SoundId == mId )
        {
            BuzStep( SoundList[ i ].BuzzerId );
            break;
        }
    }
#endif
}

U8	IsSetSoundMute(void)
{
#if 0
    return GetBuzMute();
#else
    return 0;
#endif
}

void SetSoundMute(void)
{
#if 0
    sound.Mute = TRUE;
    SaveEepromId( EEP_ID_CONF_MUTE );

    SetBuzMute( TRUE );
#endif
}

U8    GetSoundMute(void)
{
    return sound.Mute;
}

void	ClearSoundMute(void)
{
#if 0
    sound.Mute = FALSE;
    SaveEepromId( EEP_ID_CONF_MUTE );

    SetBuzMute( FALSE );
#endif
}

void TEST_Sound(void)
{
    static U8 count = 0;


    Sound( count );
    count++;
    if( count >= MAX_SOUND_NUM )
    {
        count = 0;
    }
}
