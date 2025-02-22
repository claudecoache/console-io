/**
 * @file        consoleio.c
 * @brief       Console input/output management.
 * @author      Claude Coache
 * @date        Created on 2024-07-13
 * @copyright   NEP, Montreal, Qc
 * <h2><center>&copy; COPYRIGHT 2024 NEP, Montreal, Quebec, Canada</center></h2>
 * <h2><center>&copy; All rights reserved.</center></h2><br/><br/>
 * <center>This document contains confidential and proprietary information
 * of NEPTRONIC (subject to a non-disclosure agreement) and shall</center>
 * <center>not be disclosed, distributed, or reproduced in whole or in
 * part, without the prior written authorization of NEPTRONIC.</center>
 */
/* Includes ------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "main.h"
#include "fifoUsrBuffer.h"
#include "fifoCircular.h"
#include "consoleio.h"

/* Private Typedef ---------------------------------------------------------- */
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private enum --------------------------------------------------------------*/
/* Private struct ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static UART_HandleTypeDef   *prv_huart;

static uint32_t rxCnt = 0;
static uint32_t txCnt = 0;

#ifndef CONSOLE_RXQ_SIZE
#define CONSOLE_RXQ_SIZE 1024
#endif

#ifndef CONSOLE_TXQ_SIZE
#define CONSOLE_TXQ_SIZE 1024
#endif

#ifndef CONSOLE_PRINT_SIZE
#define CONSOLE_PRINT_SIZE  80
#endif

static uint8_t  rxBuffer[CONSOLE_RXQ_SIZE];
static uint8_t  txBuffer[CONSOLE_TXQ_SIZE];

static fifo_t   rxFifo;
static fifo_t   txFifo;


/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

static void cbISRConsoleioRx( UART_HandleTypeDef *huart )
{
    /* Read from  usart and write to fifo */
    uint32_t    dat = huart->Instance->RDR;

    fifoWriteCircular( &rxFifo, (uint8_t)dat );
    rxCnt++;
}

static void cbISRConsoleioTx( UART_HandleTypeDef *huart )
{
    if ( fifoIsEmpty( &txFifo ) )
    {
        // Disable TXIE interrupt
        ATOMIC_CLEAR_BIT( huart->Instance->CR1, USART_CR1_TXEIE_TXFNFIE );

        return;
    }

    // Transmitt the next character
    huart->Instance->TDR = fifoReadCircular( &txFifo );
    txCnt++;
}

/*----------------------------------------------------------------------------*/

bool consoleioInitialize( UART_HandleTypeDef *huart, bool rxenable )
{
    /* Initialize rx/tx fifos */
    fifoSetBuffer( &rxFifo, &rxBuffer[0], CONSOLE_RXQ_SIZE );
    fifoClear( &rxFifo );
    fifoSetBuffer( &txFifo, &txBuffer[0], CONSOLE_TXQ_SIZE );
    fifoClear( &txFifo );

    prv_huart = huart;

    /* Set rx/tx callback */
    huart->RxISR = cbISRConsoleioRx;
    huart->TxISR = cbISRConsoleioTx;

    if( rxenable )
    {
        /* Enable rx interrupt */
        ATOMIC_SET_BIT(huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE_RXFNEIE);
    }

    return true;
}

/*----------------------------------------------------------------------------*/

int console_putchar( int c )
{
    uint32_t prim;
    uint8_t dat;

    if( fifoIsFull( &txFifo ) )
    {
        return EOF;
    }

    prim = __get_PRIMASK();
    __disable_irq();

    dat = (uint8_t)c;
    fifoWriteCircular( &txFifo, dat );

    if (!prim)
    {
        __enable_irq();
    }

    /* Enable tx interrupt */
    ATOMIC_SET_BIT(prv_huart->Instance->CR1, USART_CR1_TXEIE_TXFNFIE);

    return dat;
}

/*----------------------------------------------------------------------------*/

int console_puts( const char *s )
{
    int cnt = 0;

    if ( s == NULL )
    {
        return 0;
    }

    while( *s )
    {
        if (console_putchar( *s ) == EOF )
        {
            // The output Q is full
            return cnt;
        }
        cnt++;
        s++;
    }

    return cnt;
}

int console_printf (const char *fmt, ...)
{
    va_list arg;
    char buffer[CONSOLE_PRINT_SIZE];

    va_start(arg, fmt);
    vsnprintf(&buffer[0], sizeof( buffer ), fmt, arg);
    va_end(arg);

    return( console_puts( buffer ) );
}

/*----------------------------------------------------------------------------*/

int console_kbhit( void )
{
    return( rxFifo.sizeUsed );
}

/*----------------------------------------------------------------------------*/

int console_getchar( void )
{
    uint32_t    prim;
    uint8_t     dat;

    if( fifoIsEmpty( &rxFifo ) )
    {
        return EOF;
    }

    prim = __get_PRIMASK();
    __disable_irq();

    dat = fifoReadCircular( &rxFifo );

    if (!prim)
    {
        __enable_irq();
    }

    return dat;

}
