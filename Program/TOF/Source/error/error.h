#ifndef __ERROR_H__
#define __ERROR_H__

#include "prj_type.h"


/* Error Id List
 *
 * Top - Low priority for error display 
 * Bottom - Top Priority for error dispaly
 */
typedef enum _error_id_
{
    ERR_NONE,

    ERR_TEMP_COLD_WATER,
    ERR_TEMP_HOT_WATER,

//    ERR_COLD_LOW_LEVEL,    /* low */

    ERR_ROOM_COMPLEX,      /* high, low */
    ERR_ROOM_HIGH_LEVEL,   /* high */
    ERR_ROOM_LOW_LEVEL,    /* low */
    ERR_ROOM_OVF,          /* feed valve */

    ERR_NUM              
} ErrorId_T;

/* ERROR ISSUED DATE */
typedef struct _error_date_
{
    U16 u16Year;
    U8 u8Month;
    U8 u8Date;
} ErrorDate_T;

typedef struct _memento_
{
    ErrorId_T   Error;
    ErrorDate_T Date;
} Memento_T;

/* ERROR TYPE */
#define  MEMENTO_LIST_NUM   5
typedef struct _error_
{
    ErrorId_T PrevErrorId;    /* Previous ERROR STATUS */
    ErrorId_T ErrorId;        /* Current ERROR STATUS */
    ErrorDate_T ErrorDate;    /* Current ERROR ISSUED DATE */

    U8 Status[ ERR_NUM ];     /* TRUE : ERROR, FALSE : NORMAL */
    ErrorDate_T StatusDate[ ERR_NUM ];

    /* Memento */
    //ErrorId_T MementoList[ MEMENTO_LIST_NUM ];  
    Memento_T MementoList[ MEMENTO_LIST_NUM ];  
}Error_T;


void InitError(void);

void GetErrorData(Error_T *pData);

void SetErrorStatus(ErrorId_T mId, U8 mu8Status);
void SetErrorStatusDate(ErrorId_T mId, U8 mu8Status, ErrorDate_T *pDate);

U8 GetErrorStatus(ErrorId_T mId);
void GetErrorStatusDate(ErrorId_T mId, ErrorDate_T *pDate);

ErrorId_T GetErrorId(void);
void SetErrorId(ErrorId_T mErrId);
U8  GetErrorDisplayId(ErrorId_T mErrId);
void GetErrorDate(ErrorDate_T *pDate);


// 메멘토 에러 리스트 갑싱 있으면 TRUE 반환
U8 IsEmptyMementoError(void);
U8 GetMementoError(U8 mu8Index, Memento_T *pMemento);
void SetMementoError(U8 mu8Index, ErrorId_T mError, ErrorDate_T *pDate );
void ClearMementoError(void);

U8 IsError(ErrorId_T mId );
U8 IsErrorTypeNosFeed(void);
U8 IsErrorTypeColdWater(void);
U8 IsErrorTypeHotWater(void);
U8 IsErrorTypeIce(void);
U8 IsErrorTypeSoda(void);
U8 IsErrorTypeDrainWater(void);
U8 IsErrorTypeSter(void);

void UpdateNewError(U8 mu8Error);

#endif /* _ERROR_H__ */
