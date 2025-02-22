/**
 * @file       $itemname$.c
 * @brief
 * @author     $username$
 * @date       Created on $date$
 * @copyright  NEP, Montreal, Qc
 * <h2><center>&copy; COPYRIGHT $year$ NEP, Montreal, Quebec, Canada</center></h2>
 * <h2><center>&copy; All rights reserved.</center></h2><br/><br/>
 * <center>This document contains confidential and proprietary information
 * of NEPTRONIC (subject to a non-disclosure agreement) and shall</center>
 * <center>not be disclosed, distributed, or reproduced in whole or in
 * part, without the prior written authorization of NEPTRONIC.</center>
 */

/* Header guard ------------------------------------------------------------- */
#ifndef __CONSOLEIO_H
#define __CONSOLEIO_H

/* Includes ----------------------------------------------------------------- */

#include <stdbool.h>

/* Feature Enabler ---------------------------------------------------------- */
/* Define ------------------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */
/* Exported enum ------------------------------------------------------------ */
/* Exported struct ---------------------------------------------------------- */
/* Global variables --------------------------------------------------------- */
/* Exported function prototypes --------------------------------------------- */

/**
* @fn       bool consoleioInitialize( UART_HandleTypeDef *usart, bool rxenable )
* @brief    Initialize a usart i/o port.
* @param    *usart: A usart handle as defined by stn32 HAL.
* @param    rxenable: If true, rx interrupt will be enable.
* @return   Always true.
*
* For tx interrupt is managed when writing to the output.
* If required, rx can be disable if the user does not wabt an application to
* receive anything.
*/
bool consoleioInitialize( UART_HandleTypeDef *usart, bool rxenable );

/**
* @fn       int console_putchar( int c )
* @brief    Put a character for transmission.
* @param    c: Character to transmit.
* @return   EOF if the transmitt Q is full, c otherwise.
*
* c must be a 0-255 value as the console only manage 8 bits data.
*/
int console_putchar( int c );

/**
* @fn       int console_puts( const char *s )
* @brief    Put a string for transmission.
* @param    *s: Pointer to a NUL terminated string.
* @return   Number of bytes written to the tx Q.
*/
int console_puts( const char *s );

/**
* @fn       int console_printf( const char *fmt, ... )
* @brief    Print a formatted output for transmission.
* @param    *fmt: Data format.
* @param    Required information to print.
* @return   Number of bytes written to the tx Q.
*
* @see  printf formatting.
*/
int console_printf ( const char *fmt, ... );

/**
* @fn       int console_kbhit( void )
* @brief    Return the number of character in the rx Q.
* @return   Number of bytes written to the tx Q.
*/
int console_kbhit( void );

/**
* @fn       int console_getchar( void )
* @brief    Return the number of character in the rx Q.
* @return   Number of characters or EOF if the input Q is empty.
*/
int console_getchar( void );

#endif /* __CONSOLEIO_H */
