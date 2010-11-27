#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define CODE_LENGTH 12
#define DICT_SIZE (1 << CODE_LENGTH)

#ifndef MAIN
int16_t** dict;
int dict_new = 256;
short word_len = 8;
#else
extern int16_t** dict;
extern int dict_new;
extern short word_len;
#endif

void dict_init();
void dict_free();
void dict_add(int16_t, int16_t);

#endif
