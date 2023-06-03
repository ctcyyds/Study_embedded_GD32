#ifndef __BLUETOOH__H__
#define __BLUETOOH__H__
#include <stdint.h>
void bluetooth_init();
void blue_usart_send_string(uint8_t* ucstr);
void blue_usart_send_data(uint8_t ucch);


void blue_test();
#endif