#include "key_eol_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "process_eol.h"

static void SetKeyBit(U32 mu32MaskBit );

/* EOL */
// KEY EVENT LIST - EOL FRONT TEST 
static U8 EOL_Cold(void);
const static KeyEventList_T EOL_KeyEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
    { K_COLD,        EOL_Cold,       NULL,  NULL,  NULL, NULL, NULL },
};


// KEY EVENT LIST - EOL LOAD TEST 


static U8 EOL_SelCold(void);
const static KeyEventList_T EOL_KeyLoadEventList[] =
{
    /* KEY,      Short Pushed,  Long Pushed 2sec,  3sec, 5sec, Pop, TS */

    /* SINGLE KEY */
    { K_COLD,        EOL_SelCold,      NULL,     NULL, NULL,    NULL, NULL }
};


static U8 IsValidkeyEol(U16 mu16Key)
{
    if( GetEolMode() != EOL_MODE_READY &&
            GetEolMode() != EOL_MODE_DONE )
    {
        return FALSE;   
    }

    // 추출 중이면, 입력을 받지 않는다.
    return TRUE;
}

static U8 IsValidKeyFront(U16 mu16Key)
{
    return IsValidkeyEol( mu16Key );
}

U8 IsValidEolKeyCondition(U16 mu16Key)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return IsValidKeyFront( mu16Key );
    }

    return IsValidkeyEol( mu16Key );
}

void* GetEolKeyEventList(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return (void *)EOL_KeyEventList;
    }

    return (void *)EOL_KeyLoadEventList;
}

U8 GetEolKeyEventListSize(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return ( sizeof(EOL_KeyEventList) / sizeof(KeyEventList_T) );
    }

    return ( sizeof(EOL_KeyLoadEventList) / sizeof(KeyEventList_T) );
}

static void SetKeyBit(U32 mu32MaskBit )
{
    U32 mu32InputVal;


    mu32InputVal = GetEolTestInputVal();
    if( (mu32InputVal & mu32MaskBit ) != 0 )
    {
        ClearEolTestInputBit( mu32MaskBit );
    }
    else
    {
        SetEolTestInputBit( mu32MaskBit );
    }
}


static U8 EOL_Cold(void)
{
    SetKeyBit( MK_EOL_KEY_COLD );

    return SOUND_SELECT;
}


// KEY EVENT LIST - EOL LOAD TEST 
static U8 EOL_SelCold(void)
{
    // Select water type
    //SetWaterOutSelect( SEL_WATER_COLD );
    return SOUND_SELECT;
}

