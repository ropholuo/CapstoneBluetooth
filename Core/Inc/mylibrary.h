#ifndef INC_MYLIBRARY_H_
#define INC_MYLIBRARY_H_

#include "main.h"
#include <string.h>

extern char buffer[1000];
extern char bufferarr[100];
extern char schedule[7][200];
extern uint8_t timer_count, buffer_index;
extern int apm;
extern UART_HandleTypeDef huart1;
extern char mdate[3], mtime[5], nbin[1], med[15], npill[1];

uint8_t string_compare(char array1[], char array2[], uint16_t length);
void Message_handler();
void Process_string(char arr[]);
void Manage_strings(char arr[]);
void Arrange_strings(char arr[]);
void Indivial_med(char arr[], char target[]);

#endif /* INC_MYLIBRARY_H_ */
