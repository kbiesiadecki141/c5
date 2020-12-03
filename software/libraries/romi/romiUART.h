/*
	Original Author:		Jeff C. Jensen
    Rewritten by: Joshua Adkins, Neal Jackson
    Revised:	2018-08-06
*/

#ifndef _ROMI_UART_H
#define _ROMI_UART_H

#include <stddef.h>

extern  int32_t romiReadFeedbackPacket(uint8_t* packetBuffer, uint8_t len);

#endif
