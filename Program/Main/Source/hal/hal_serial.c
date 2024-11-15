#define CONFIG_UART_0   0
#define CONFIG_UART_1   1
#define CONFIG_UART_2   0
#define CONFIG_UART_3   0  

/* UART0 : COMP */
#if CONFIG_UART_0 
#pragma interrupt INTST0    r_uart0_interrupt_send
#pragma interrupt INTSR0    r_uart0_interrupt_receive
#endif

/* UART1 : WIFI */
#if CONFIG_UART_1
#pragma interrupt INTST1    r_uart1_interrupt_send
#pragma interrupt INTSR1    r_uart1_interrupt_receive
#endif

/* UART2 : FRONT */
#if CONFIG_UART_2 
#pragma interrupt INTST2    r_uart2_interrupt_send
#pragma interrupt INTSR2    r_uart2_interrupt_receive
#endif

/* UART3 : EOL */
#if CONFIG_UART_3 
#pragma interrupt INTST3    r_uart3_interrupt_send
#pragma interrupt INTSR3    r_uart3_interrupt_receive
#endif


#include "hw.h"
#include "hal_serial.h"
#include <string.h>
#include "timer.h"


//#if CONFIG_UART_0
//#define  COMP_TXD TXD0
//#define  COMP_RXD RXD0
//#endif
//
//#if CONFIG_UART_1
//#define  TOF_TXD TXD1
//#define  TOF_RXD RXD1
//#endif
//
//#if CONFIG_UART_2
//#define  FRONT_TXD TXD2
//#define  FRONT_RXD RXD2
//#endif
//
//#if CONFIG_UART_3
//#define  EOL_TXD TXD3
//#define  EOL_RXD RXD3
//#endif

// @ms
#define UART0_RX_TIME_STAMP   100
#define UART1_RX_TIME_STAMP   10
#define UART2_RX_TIME_STAMP   10
#define UART3_RX_TIME_STAMP   10


#if CONFIG_UART_0
U8 recv_0_buf[ MAX_COMM_0_RX_BUF_SZ ];
U8 send_0_buf[ MAX_COMM_0_TX_BUF_SZ ];
#else
U8 recv_0_buf[ 1 ];
U8 send_0_buf[ 1 ];
#endif

#if CONFIG_UART_1
U8 recv_1_buf[ MAX_COMM_1_RX_BUF_SZ ];
U8 send_1_buf[ MAX_COMM_1_TX_BUF_SZ ];
#else
U8 recv_1_buf[ 1 ];
U8 send_1_buf[ 1 ];
#endif

#if CONFIG_UART_2
U8 recv_2_buf[ MAX_COMM_2_RX_BUF_SZ ];
U8 send_2_buf[ MAX_COMM_2_TX_BUF_SZ ];
#else
U8 recv_2_buf[ 1 ];
U8 send_2_buf[ 1 ];
#endif

#if CONFIG_UART_3
U8 recv_3_buf[ MAX_COMM_3_RX_BUF_SZ ];
U8 send_3_buf[ MAX_COMM_3_TX_BUF_SZ ];
#else
U8 recv_3_buf[ 1 ];
U8 send_3_buf[ 1 ];
#endif

typedef struct _comm_
{
    I16 recv_len;
    I16 send_len;
    I16 tx_len;
} comm_t;
comm_t  comm[ MAX_COMM_ID ];


void    HAL_InitCommId( U8 uart_id )
{
    if( uart_id == COMM_ID_UART_0 )
    {
#if CONFIG_UART_0
        MEMSET( (void __FAR *)&recv_0_buf[0], 0, MAX_COMM_0_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_0_buf[0], 0, MAX_COMM_0_TX_BUF_SZ );
#endif
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
#if CONFIG_UART_1
        MEMSET( (void __FAR *)&recv_1_buf[0], 0, MAX_COMM_1_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_1_buf[0], 0, MAX_COMM_1_TX_BUF_SZ );
#endif
    }
    else if( uart_id == COMM_ID_UART_2)
    {
#if CONFIG_UART_2
        MEMSET( (void __FAR *)&recv_2_buf[0], 0, MAX_COMM_2_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_2_buf[0], 0, MAX_COMM_2_TX_BUF_SZ );
#endif
    }
    else //if( uart_id == COMM_ID_UART_3)
    {
#if CONFIG_UART_3
        MEMSET( (void __FAR *)&recv_3_buf[0], 0, MAX_COMM_3_RX_BUF_SZ );
        MEMSET( (void __FAR *)&send_3_buf[0], 0, MAX_COMM_3_TX_BUF_SZ );
#endif
    }

    MEMSET( (void __FAR *)&comm[ uart_id ], 0, sizeof( comm_t ) );
}

void    HAL_InitComm( void )
{
#if CONFIG_UART_0
    HAL_InitCommId( COMM_ID_UART_0 );
    R_UART0_Start();
#endif

#if CONFIG_UART_1
    HAL_InitCommId( COMM_ID_UART_1 );
    R_UART1_Start();
#endif

#if CONFIG_UART_2
    HAL_InitCommId( COMM_ID_UART_2 );
    R_UART2_Start();
#endif

#if CONFIG_UART_3
    HAL_InitCommId( COMM_ID_UART_3 );
    R_UART3_Start();
#endif

}

static I16 HAL_GetMaxRecvBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_COMM_0_RX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_COMM_1_RX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_COMM_2_RX_BUF_SZ;
    }

    return MAX_COMM_3_RX_BUF_SZ;
}

U8  HAL_IsFullRecvBuffer( U8 uart_id )
{
    if( comm[ uart_id ].recv_len >= HAL_GetMaxRecvBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
}

U8  HAL_IsEmptyRecvBuffer( U8 uart_id )
{
    if( comm[ uart_id ].recv_len > 0 )
    {
        return FALSE;
    }

    return TRUE;

}

static I16 HAL_GetMaxSendBufferLength(U8 uart_id)
{
    if( uart_id == 0 )
    {
        return MAX_COMM_0_TX_BUF_SZ;
    }
    else if( uart_id == 1 )
    {
        return MAX_COMM_1_TX_BUF_SZ;
    }
    else if( uart_id == 2 )
    {
        return MAX_COMM_2_TX_BUF_SZ;
    }

    return MAX_COMM_3_TX_BUF_SZ;
}

U8  HAL_IsFullSendBuffer( U8 uart_id )
{
    if( comm[ uart_id ].send_len >= HAL_GetMaxSendBufferLength( uart_id ) )
    {
        return TRUE;
    }

    return FALSE;
}

void HAL_InitRecvLength(U8 uart_id )
{
    comm[ uart_id ].recv_len = 0;
}

I16  HAL_GetRecvLength( U8 uart_id )
{
    return comm[ uart_id ].recv_len;
}

I16  HAL_GetSendLength( U8 uart_id )
{
    return comm[ uart_id].send_len;
}


void    HAL_SetRecvBuffer(U8 uart_id, U8 _byte )
{
    comm_t *p_comm;

    p_comm = &comm[ uart_id ];
    if( uart_id == COMM_ID_UART_0 )
    {
        recv_0_buf[ p_comm->recv_len ] = _byte;
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
        recv_1_buf[ p_comm->recv_len ] = _byte;
    }
    else if( uart_id == COMM_ID_UART_2)
    {
        recv_2_buf[ p_comm->recv_len ] = _byte;
    }
    else 
    {
        recv_3_buf[ p_comm->recv_len ] = _byte;
    }
    p_comm->recv_len++;

}

U8 HAL_GetRecvBuffer( U8 uart_id, U16 index )
{
    if( uart_id == COMM_ID_UART_0 )
    {
        return recv_0_buf[ index ];
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
        return recv_1_buf[ index ];
    }
    else if( uart_id == COMM_ID_UART_2)
    {
        return recv_2_buf[ index ];
    }
    else 
    {
        return recv_3_buf[ index ];
    }
}


void    HAL_SetSendBuffer( U8 uart_id, U8 _byte )
{
    comm_t *p_comm;

    if( HAL_IsFullSendBuffer( uart_id ) == FALSE )
    {
        p_comm = &comm[ uart_id ];
        if( uart_id == COMM_ID_UART_0 )
        {
            send_0_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == COMM_ID_UART_1 )
        {
            send_1_buf[ p_comm->send_len ] = _byte;
        }
        else if( uart_id == COMM_ID_UART_2)
        {
            send_2_buf[ p_comm->send_len ] = _byte;
        }
        else 
        {
            send_3_buf[ p_comm->send_len ] = _byte;
        }
        p_comm->send_len++;
    }
}

U8  HAL_GetSendBuffer( U8 uart_id, U16 index )
{
    if( uart_id == COMM_ID_UART_0 )
    {
        return send_0_buf[index];
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
        return send_1_buf[index];
    }
    else if( uart_id == COMM_ID_UART_2)
    {
        return send_2_buf[index];
    }
    else 
    {
        return send_3_buf[index];
    }
}


void    HAL_SendByte( U8 uart_id )
{
    U8 ch;
    comm_t *p_comm;


    p_comm  = &comm[ uart_id ];
    if( uart_id == COMM_ID_UART_0 )
    {
        ch = send_0_buf[p_comm->tx_len];
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
        ch = send_1_buf[p_comm->tx_len];
    }
    else if( uart_id == COMM_ID_UART_2)
    {
        ch = send_2_buf[p_comm->tx_len];
    }
    else 
    {
        ch = send_3_buf[p_comm->tx_len];
    }

    p_comm->tx_len++;

    if( uart_id == COMM_ID_UART_0 )
    {
#if CONFIG_UART_0
        TXD0 = ch;
#endif
    }
    else if( uart_id == COMM_ID_UART_1 )
    {
#if CONFIG_UART_1
        TXD1 = ch;
#endif
    }
    else if( uart_id == COMM_ID_UART_2 )
    {
#if CONFIG_UART_2
        TXD2 = ch;
#endif
    }
    else if( uart_id == COMM_ID_UART_3 )
    {
#if CONFIG_UART_3
        TXD3 = ch;
#endif
    }
}

static U8   IsCompleteTx( U8 uart_id )
{
    if( comm[uart_id].tx_len < comm[ uart_id ].send_len )
    {
        return FALSE;
    }

    return TRUE;

}

/* INTERRUPT */

/* UART 0 */
#if CONFIG_UART_0
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;


    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    rx_data = RXD0;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_UART_0 ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_UART_0, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_UART_0 );
        }

        StartTimer( TIMER_ID_COMM_COMP_RX, UART0_RX_TIME_STAMP );
    }
}

__interrupt static void r_uart0_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_UART_0 ) == FALSE )
    {
        HAL_SendByte( COMM_ID_UART_0 );
    }
    else
    {
        HAL_InitCommId( COMM_ID_UART_0 );
    }
}
#endif

/* UART 1 */
#if CONFIG_UART_1
__interrupt static void r_uart1_interrupt_receive(void)
{

    volatile U8 rx_data;
    volatile U8 err_type;

    err_type = (U8)(SSR03 & 0x0007U);
    SIR03 = (U16)err_type;

    rx_data = RXD1;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_UART_1 ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_UART_1, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_UART_1 );
        }
    }

    StartTimer( TIMER_ID_COMM_UART_1_RX, UART1_RX_TIME_STAMP );

}

__interrupt static void r_uart1_interrupt_send(void)
{

    if( IsCompleteTx( COMM_ID_UART_1 ) == FALSE )
    {
        HAL_SendByte( COMM_ID_UART_1 );
    }
    else
    {
        HAL_InitCommId( COMM_ID_UART_1 );
    }

}
#endif

/* UART 2 */
#if CONFIG_UART_2
U16 the_rx_err_count = 0;
__interrupt static void r_uart2_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR11 & 0x0007U);
    SIR11 = (uint16_t)err_type;

    rx_data = RXD2;

    EI();
    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_UART_2 ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_UART_2, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_UART_2 );
        }

        StartTimer( TIMER_ID_COMM_FRONT_RX, UART2_RX_TIME_STAMP );
    }
    else
    {
        the_rx_err_count++;
    }
}

__interrupt static void r_uart2_interrupt_send(void)
{
    EI();
    if( IsCompleteTx( COMM_ID_UART_2 ) == FALSE )
    {
        HAL_SendByte( COMM_ID_UART_2 );
    }
    else
    {
        HAL_InitCommId( COMM_ID_UART_2 );
    }
}
#endif


/* UART 3 */
#if CONFIG_UART_3
U16 the_isr_err_count = 0;
__interrupt static void r_uart3_interrupt_receive(void)
{
    volatile U8 err_type;
    volatile U8 rx_data;

    err_type = (uint8_t)(SSR13 & 0x0007U);
    SIR13 = (uint16_t)err_type;

    rx_data = RXD3;

    if( err_type == 0 )
    {
        if( HAL_IsFullRecvBuffer( COMM_ID_UART_3 ) == FALSE )
        {
            HAL_SetRecvBuffer( COMM_ID_UART_3, rx_data );
        }
        else
        {
            HAL_InitCommId( COMM_ID_UART_3 );
        }

        StartTimer( TIMER_ID_COMM_EOL_RX, UART3_RX_TIME_STAMP );
    }
    else
    {
        the_isr_err_count++;
    }
}


__interrupt static void r_uart3_interrupt_send(void)
{
    if( IsCompleteTx( COMM_ID_UART_3 ) == FALSE )
    {
        HAL_SendByte( COMM_ID_UART_3 );
    }
    else
    {
        HAL_InitCommId( COMM_ID_UART_3 );
    }
}
#endif
