#include "hal_eeprom.h"
#include "eeprom.h"
#include "util.h"
#include "crc16.h" 

#include "time_short.h" 
#include "process_display.h" 

#include "hot_water.h"
#include "cold_water.h"
#include "error.h"



#define EEP_ADDR_OFFSET   0x0010

// TOTAL 32 blocks
// 16 bytes per block
typedef enum
{
    EEP_BLOCK_NONE,
    EEP_BLOCK_0,
    EEP_BLOCK_1,
   // EEP_BLOCK_2,
   // EEP_BLOCK_3,
   // EEP_BLOCK_4,

    NUM_EEP_BLOCK
} EepromBlock_T;

typedef struct _eeprom_control_
{
    /* Init / Write  */
    U8 Init;
    U8 Write;
    U8 Erase;
    U8 FactoryReset;        // Í≥µÏû?Ï¥àÍ∏∞Ìôî ?òÎ©? TRUE Í∑∏Î†áÏßÄ ?ä?ºÎ©¥ FALSE

    U16 DelayTime;
    U8 ReadData[ EEP_PAGE_SIZE ];
    U8 WriteData[ EEP_PAGE_SIZE ];
} EEP_Control_T;


EEP_Control_T  Eep;


static U8 ReadDataBlocks(void);
static U8 WriteDataBlocks(void);
static void InitBlockTableAllSet(void);

void InitEeprom(void)
{
    Eep.Init      = FALSE;
    Eep.Erase     = FALSE;
    Eep.FactoryReset = FALSE;

    if( ReadDataBlocks() == TRUE )
    {
        // ?ΩÍ∏∞ ?±Í≥µ
        Eep.Init = TRUE; 
    }
    else
    {
        // Ï¥àÍ∏∞Ìôî ÏßÑ?â
        Eep.FactoryReset = TRUE;

        InitBlockTableAllSet();
        WriteDataBlocks();
    }

    // Write?òÍ≥† Delay Time?Ä ReadData() ?¥ÌõÑ?ê ?§Ìñâ?ò?¥Ïï??ú??
    // ReadData ?ô?ë Ï§ë?ê, Set?®Ïàò?§Ïóê ?Ä?¥ÏÑú Eep.WriteÍ∞Ä TRUEÍ∞Ä?òÍ∏??åÎ¨∏Ïù¥Îã?
    // Eep.WriteÍ∞Ä TRUEÍ∞Ä?òÎ©?Î∂à?Ñ?î?ú eeprom write ?ô?ë???ú Î≤à ?§Ìñâ?ò?î Í≤É?Ñ ÎßâÍ∏??Ñ?¥ÏÑú?¥Îã?
    Eep.Write     = FALSE;
    Eep.DelayTime = 0;
}

typedef struct _eeprom_block_map_
{
    EepromId_T      id;
    EepromBlock_T   blockId;
} EepromMap_T;

const static EepromMap_T EepromMappingTable[] = 
{
    { EEP_ID_CONF_MAKE_COLD,            EEP_BLOCK_0 },      
    { EEP_ID_CONF_MAKE_HOT,             EEP_BLOCK_0 },      
    { EEP_ID_HOT_ALTITUDE,              EEP_BLOCK_0 },      

    { EEP_ID_MEMENTO_1,                 EEP_BLOCK_1 },      
    { EEP_ID_MEMENTO_2,                 EEP_BLOCK_1 },      
    { EEP_ID_MEMENTO_3,                 EEP_BLOCK_1 },      
    { EEP_ID_MEMENTO_4,                 EEP_BLOCK_1 },      
    { EEP_ID_MEMENTO_5,                 EEP_BLOCK_1 },      

};
#define SZ_EEPROM_MAPPING_TABLE     (sizeof(EepromMappingTable)/sizeof(EepromMap_T))

// eeprom id?ê ?¥ÎãπÌïò?î block idÎ•?Î∞ò?ò
static EepromBlock_T GetBlockId(EepromId_T mu8Id )
{
    U8 i;

    for( i = 0; i < SZ_EEPROM_MAPPING_TABLE; i++ )
    {
        if( mu8Id == EepromMappingTable[ i ].id )
        {
            return EepromMappingTable[ i ].blockId;
        }
    }

    return EEP_BLOCK_NONE;  // ?¥Îã?ID ?Ü?å.
}


typedef U8 (*Action_T)(EepromBlock_T);
typedef struct _eeprom_block_table_
{
    EepromBlock_T   blockId;
    U8 write;                   // TRUE?¥Î©¥ Write ?ô?ë ?ò?â
    Action_T    pWriteAction;   
    Action_T    pReadAction;   
}EepBlockTable_T;

static U8 WriteBlock0(EepromBlock_T mBlockId);
static U8 ReadBlock0(EepromBlock_T mBlockId);
static U8 WriteBlock1(EepromBlock_T mBlockId);
static U8 ReadBlock1(EepromBlock_T mBlockId);

static U8 WriteBlock2(EepromBlock_T mBlockId);
static U8 ReadBlock2(EepromBlock_T mBlockId);
static U8 WriteBlock3(EepromBlock_T mBlockId);
static U8 ReadBlock3(EepromBlock_T mBlockId);
static U8 WriteBlock4(EepromBlock_T mBlockId);
static U8 ReadBlock4(EepromBlock_T mBlockId);

// Î∏î?≠ ID?ê ?¥ÎãπÌïò?î ?®Ïàò ?∏Ï?
static EepBlockTable_T    EepromBlockTableList[] = 
{
    { EEP_BLOCK_0,      FALSE,   WriteBlock0,   ReadBlock0 },
    { EEP_BLOCK_1,      FALSE,   WriteBlock1,   ReadBlock1 },
};
#define SZ_BLOCK_TABLE     (sizeof(EepromBlockTableList)/sizeof(EepBlockTable_T))

static void InitBlockTableAllSet(void)
{
    U8 i;

    for( i = 0; i < SZ_BLOCK_TABLE; i++ )
    {
        EepromBlockTableList[ i ].write = TRUE;
    }
}

static void SetBlockWrite( EepromBlock_T muBlockId )
{
    U8 i;

    for( i = 0; i < SZ_BLOCK_TABLE; i++ )
    {
        if( muBlockId == EepromBlockTableList[ i ].blockId )
        {
            EepromBlockTableList[ i ].write = TRUE;
        }
    }
}


void SaveEepromId( EepromId_T mu8Id )
{
    EepromBlock_T   muBlockId;

    if( mu8Id > EEP_ID_NUM )
    {
        return ;
    }

    Eep.DelayTime = 1;  // 1sec..
    Eep.Write = TRUE;  

    muBlockId = GetBlockId( mu8Id );
    if( muBlockId != EEP_BLOCK_NONE )
    {
        SetBlockWrite( muBlockId );
    }
}

void EraseEeprom(void)
{
    Eep.Write = TRUE;
    Eep.Erase = TRUE;
}


static U8 EepromPageWrite( U16 mu16Addr, U8 *pData )
{
//    if( IsRtcBatBackUpMode() == TRUE )
//    {
//        return FALSE;
//    }
//
    mu16Addr += EEP_ADDR_OFFSET;
    return HAL_RTC_PageWrite( DEV_ADDR_EEP, mu16Addr, pData, EEP_PAGE_SIZE);
}

static U8 EepromSeqRead( U16 mu16Addr, U8 *pData, U8 mu16Length )
{
//    if( IsRtcBatBackUpMode() == TRUE )
//    {
//        return FALSE;
//    }

    mu16Addr += EEP_ADDR_OFFSET;
    return HAL_RTC_SeqRead( DEV_ADDR_EEP, mu16Addr, pData, mu16Length );
}


static U8 WriteBlock0(EepromBlock_T mBlockId)
{
    U16 mu16CheckSum = 0U;

    Eep.WriteData[ 0 ]  = GetColdWaterConfigMake();    
    Eep.WriteData[ 1 ]  = GetHotWaterConfigMake();
    Eep.WriteData[ 2 ]  = GetHotWaterAltidue();
    Eep.WriteData[ 3 ]  = 0;
    Eep.WriteData[ 4 ]  = 0;
    Eep.WriteData[ 5 ]  = 0; 
    Eep.WriteData[ 6 ]  = 0; 
    Eep.WriteData[ 7 ]  = 0; 
    mu16CheckSum = crc16_cal( &Eep.WriteData[0], (EEP_PAGE_SIZE - 2) );

    Eep.WriteData[EEP_PAGE_SIZE - 2] = GET_HIGH_BYTE( mu16CheckSum );
    Eep.WriteData[EEP_PAGE_SIZE - 1] = GET_LOW_BYTE( mu16CheckSum );


    // Write
    return EepromPageWrite( ((U16)mBlockId * EEP_PAGE_SIZE) , &Eep.WriteData[0] );
}

static U8 ReadBlockId(U8 mu8BlockId, U8 *pBuf)
{
    U8 mu8Ret;
    U16 mu16SrcChksum = 0U;
    U16 mu16CalcChksum = 0U;


    // Read 
    mu8Ret = EepromSeqRead( ((U16)mu8BlockId * EEP_PAGE_SIZE ), pBuf, EEP_PAGE_SIZE );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Checksum
    mu16SrcChksum  = GET_UINT_WORD( pBuf[EEP_PAGE_SIZE - 1], pBuf[EEP_PAGE_SIZE - 2] ); 
    mu16CalcChksum = crc16_cal( pBuf, (EEP_PAGE_SIZE - 2) );
    if( mu16SrcChksum != mu16CalcChksum )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 ReadBlock0(EepromBlock_T mBlockId)
{
    U8 mu8Ret;

    mu8Ret = ReadBlockId( mBlockId, &Eep.ReadData[0] );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Load Data
    SetColdWaterConfigMake( Eep.ReadData[0] );
    SetHotWaterConfigMake( Eep.ReadData[1] );
    SetHotWaterAltidue( Eep.ReadData[2] );

    return TRUE;
}

static U8 WriteBlock1(EepromBlock_T mBlockId)
{
    U16 mu16CheckSum = 0U;

    Memento_T   mMemento;


    // Memento 1
    GetMementoError( 0, &mMemento );
    Eep.WriteData[ 0 ] = mMemento.Error;

    // Memento 2
    GetMementoError( 1, &mMemento );
    Eep.WriteData[ 1 ] = mMemento.Error;

    // Memento 3
    GetMementoError( 2, &mMemento );
    Eep.WriteData[ 2 ] = mMemento.Error;

    // Memento 4
    GetMementoError( 3, &mMemento );
    Eep.WriteData[ 3 ] = mMemento.Error;

    // Memento 5
    GetMementoError( 4, &mMemento );
    Eep.WriteData[ 4 ] = mMemento.Error;

    mu16CheckSum = crc16_cal( &Eep.WriteData[0], (EEP_PAGE_SIZE - 2) );

    Eep.WriteData[EEP_PAGE_SIZE - 2] = GET_HIGH_BYTE( mu16CheckSum );
    Eep.WriteData[EEP_PAGE_SIZE - 1] = GET_LOW_BYTE( mu16CheckSum );


    // Write
    return EepromPageWrite( ((U16)mBlockId * EEP_PAGE_SIZE) , &Eep.WriteData[0] );
}

static U8 ReadBlock1(EepromBlock_T mBlockId)
{
    U8 mu8Ret;
    Memento_T   mMemento;


    mu8Ret = ReadBlockId( mBlockId, &Eep.ReadData[0] );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mMemento.Date.u16Year = 0;  // unused
    mMemento.Date.u8Month = 0;  // unsued
    mMemento.Date.u8Date  = 0;  // unused

    // Memento 0 
    mMemento.Error = Eep.ReadData[0];
    SetMementoError( 0, mMemento.Error, &mMemento.Date );

    mMemento.Error = Eep.ReadData[1];
    SetMementoError( 1, mMemento.Error, &mMemento.Date );

    mMemento.Error = Eep.ReadData[2];
    SetMementoError( 2, mMemento.Error, &mMemento.Date );

    mMemento.Error = Eep.ReadData[3];
    SetMementoError( 3, mMemento.Error, &mMemento.Date );

    mMemento.Error = Eep.ReadData[4];
    SetMementoError( 4, mMemento.Error, &mMemento.Date );
        
    return TRUE;
}


static U8 WriteDataBlocks(void)
{
    U8 i = 0;
    volatile U8 mu8Ret = TRUE;
    EepromBlock_T mBlockId;

    for( i = 0; i < SZ_BLOCK_TABLE ; i++ )
    {
        if( EepromBlockTableList[ i ].write == TRUE )
        {
            if( EepromBlockTableList[ i ].pWriteAction != NULL ) 
            {
                mBlockId = EepromBlockTableList[ i ].blockId;
                mu8Ret = EepromBlockTableList[ i ].pWriteAction( mBlockId );
                if( mu8Ret == TRUE )
                {
                    EepromBlockTableList[ i ].write = FALSE;
                }
            }
            else
            {
                EepromBlockTableList[ i ].write = FALSE;
            }
        }
    }

    return TRUE;
}


static U8 ReadDataBlocks(void)
{
    U8 i = 0;
    volatile U8 mu8Ret = TRUE;
    EepromBlock_T mBlockId;


    for( i = 0; i < SZ_BLOCK_TABLE ; i++ )
    {
        if( EepromBlockTableList[ i ].pReadAction != NULL ) 
        {
            mBlockId = EepromBlockTableList[ i ].blockId;
            mu8Ret = EepromBlockTableList[ i ].pReadAction( mBlockId );
            if( mu8Ret == FALSE )
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}


void ProcessEeprom(void)
{
    // TimeShort Î™®Îìú?ê?ú?î eeprom Í∏∞Îä?ÎπÑ?ú?±Ìôî
    // ?? Î©îÎ©ò?† Î™®Îìú??Í≤ΩÏö∞Ïóê?î ?à?©Ìïú??
    if( GetTimeShortStatus() == TRUE )
    {
        Eep.Write = FALSE;
        return ;
    }

    //// Ï∂îÏ∂ú Ï§ë?ê?î Ï§ëÏßÄ
    //if( GetWaterOut() == TRUE )
    //{
    //    return ;
    //}

    // Write Î™Ö??Ï°∞Í±¥ Í≤Ä??
    if( Eep.Write == FALSE )
    {
        return ;
    }

    // Write ?úÍ∞Ñ ÏßÄ???ï??
    if( Eep.DelayTime != 0 )
    {
        Eep.DelayTime--;
        return ;
    }

    // Write Data
    //if( IsRtcBatBackUpMode() == FALSE )
    {
        if( Eep.Erase == TRUE )
        {
            // eeprom Ï¥àÍ∏∞Ìôî?î Ï¥àÍ∏∞Ìôî ?Ñ, ?ú?§ÌÖú RESET
            if( HAL_RTC_EEPROM_AllErase() == TRUE )
            {
                Eep.Write = FALSE;
                Eep.Erase = FALSE;
                Reset();
            }
        }
        else
        {
            WriteDataBlocks();
            Eep.Write = FALSE;
        }
    }

}

U8 TestEeprom(void)
{
    volatile U8 mu8Ret = 0;
    volatile U8 mu8Read = 0;
    U16 mu16Addr;

    // 0x00 Î≤àÏßÄ?ê 0x12Í∞í?Ñ ?ºÏù¥ÌåÖ ?òÍ≥† ?§Ïãú ?ΩÏñ¥Î??ºÎ???
    // eeprom ?ï?Å?ô?ë???ò?îÏßÄ ?ï?∏Ìï?..
    //if( IsRtcBatBackUpMode() == FALSE )
    {
        mu16Addr = (EEP_BLOCK_0 * EEP_PAGE_SIZE) + EEP_ADDR_OFFSET;
        HAL_RTC_ByteWrite( DEV_ADDR_EEP, mu16Addr, 0x12 );
        HAL_RTC_ByteRead( DEV_ADDR_EEP, mu16Addr, &mu8Read );
    }

    if( mu8Read == 0x12 )
    {
        return TRUE;
    }

    return FALSE;
}

