/***************************************************************************/ /**
   @file     cqueue.h
   @brief    Circular Queue
   @author   JACOBY, D.
  ******************************************************************************/

#ifndef UART_CQUEUE_H
#define UART_CQUEUE_H

void UART_QueueInit(void);
unsigned char UART_PushQueue(unsigned char data);
unsigned char UART_PullQueue(void);
unsigned char UART_QueueStatus(void);
#define QOVERFLOW 0xFF

#endif // UART_CQUEUE_H
