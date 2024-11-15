#ifndef __TIMER_ID_H__
#define __TIMER_ID_H__

/* TIMER - MS */
enum
{
    TIMER_ID_COMM_UART_1_RX,
    TIMER_ID_COMM_UART_1_TX,

    TIMER_ID_TOF,
    TIMER_ID_1MS,
    TIMER_ID_10MS,
    TIMER_ID_100MS,
    TIMER_ID_1SEC,

    MAX_TIMER_ID_NUM
};

#define TIMER_ID_ADC_INTERVAL                       10UL

#endif /* __TIMER_ID_H__ */
