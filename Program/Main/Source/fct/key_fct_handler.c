#include "key_fct_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "process_fct.h"

static void SetKeyBit(U32 mu32MaskBit );
static U8 FCT_Room(void);
static U8 FCT_Cold(void);
static U8 FCT_Conty(void);

static U8 FCT_Soda(void);
static U8 FCT_User(void);
static U8 FCT_Back(void);
static U8 FCT_Setting(void);
static U8 FCT_Left(void);
static U8 FCT_Right(void);

/* FCT */
const static KeyEventList_T FCT_KeyEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
//    { K_ROOM,        FCT_Room,      NULL, NULL, NULL, NULL, NULL },
    { K_COLD,        FCT_Cold,      NULL, NULL, NULL, NULL, NULL }
};


U8 IsValidFctKeyCondition(U16 mu16Key)
{

    return TRUE;
}

void* GetFctKeyEventList(void)
{
    return (void *)FCT_KeyEventList;
}

U8 GetFctKeyEventListSize(void)
{
    return ( sizeof(FCT_KeyEventList) / sizeof(KeyEventList_T) );
}

static void SetKeyBit(U32 mu32MaskBit )
{
    U32 mu32InputVal;


    mu32InputVal = GetFctTestInputVal();
    if( (mu32InputVal & mu32MaskBit ) != 0 )
    {
        ClearFctTestInputBit( mu32MaskBit );
    }
    else
    {
        SetFctTestInputBit( mu32MaskBit );
    }
}

static U8 FCT_Room(void)
{
    SetKeyBit( MK_FCT_KEY_ROOM );


    ClearFctTestInputBit( MK_FCT_KEY_COLD );
    ClearFctTestInputBit( MK_FCT_KEY_SODA );
    return SOUND_SELECT;
}

static U8 FCT_Cold(void)
{
    SetKeyBit( MK_FCT_KEY_COLD );

    ClearFctTestInputBit( MK_FCT_KEY_ROOM );
    ClearFctTestInputBit( MK_FCT_KEY_SODA );

    return SOUND_SELECT;
}

static U8 FCT_Conty(void)
{
    SetKeyBit( MK_FCT_KEY_CONTY );

    return SOUND_SELECT;
}

static U8 FCT_Soda(void)
{
    SetKeyBit( MK_FCT_KEY_SODA );

    ClearFctTestInputBit( MK_FCT_KEY_ROOM );
    ClearFctTestInputBit( MK_FCT_KEY_COLD );

    return SOUND_SELECT;
}

static U8 FCT_User(void)
{
    SetKeyBit( MK_FCT_KEY_USER );

    return SOUND_SELECT;
}

static U8 FCT_Back(void)
{
    SetKeyBit( MK_FCT_KEY_BACK );

    return SOUND_SELECT;
}

static U8 FCT_Setting(void)
{
    SetKeyBit( MK_FCT_KEY_SETTING );

    return SOUND_SELECT;
}

static U8 FCT_Left(void)
{
    SetKeyBit( MK_FCT_KEY_LEFT );

    return SOUND_SELECT;
}

static U8 FCT_Right(void)
{
    SetKeyBit( MK_FCT_KEY_RIGHT );

    return SOUND_SELECT;
}
