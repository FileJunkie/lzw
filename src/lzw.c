#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define CODE_LENGTH 12
#define DICT_SIZE (1 << CODE_LENGTH)
#define DEBUG

int16_t** dict;
short word_len = 8;

void dict_init(){
	int i;

	dict = malloc(sizeof(int16_t*) * DICT_SIZE);
	if(dict == NULL){
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}

	for(i = 0; i < 256; i++){
		dict[i] = malloc(sizeof(int16_t) * 2);
		if(dict[i] == NULL){
			fprintf(stderr, "Memory allocation error\n");
			exit(1);
		}
		dict[i][0] = i;
		dict[i][1] = -1;
	}

	for(i = 256; i < DICT_SIZE; i++){
		dict[i] = NULL;
	}
}

void dict_free(){
	int i;

	for(i = 0; i < DICT_SIZE; i++){
		free(dict[i]);
	}
	free(dict);
}

void print_code(int16_t code){
	static uint8_t tail = 0, tail_len = 0;
	int cur_left = word_len, i;
	uint8_t tempbyte;

	for(i = 0; i < 16 - word_len; i++){
		code &= ~(1 << (16 - i - 1));
	}

	if(tail_len != 0){
		tempbyte = tail << (8 - tail_len);
		tempbyte |= (uint8_t)(code >> (word_len - 8 + tail_len));
		putchar(tempbyte);
		cur_left -= 8 - tail_len;
		if(cur_left < 8){
			tail_len = cur_left;
			tail = (uint8_t)(code >> (word_len - tail_len));
			return;
		}
		else{
			tail_len = cur_left - 8;
			putchar((uint8_t)(code >> tail_len));
			for(i = 0; i < 16 - tail_len; i++){
				code &= ~(1 << (16 - i - 1));
			}
			tail = (uint8_t)(code);
			return;
		}
	}
	else{
		putchar( (uint8_t)(code >> (word_len - 8)) );
		tail_len = word_len - 8;
		if(word_len > 8){
			tail = (uint8_t)(code >> (word_len - tail_len));
		}
	}
}

int main(int argc, char** argv){
// 	int prevchar, nextchar;

	dict_init();

// 	prevchar = getchar();
// 	while((nextchar = getchar()) != EOF){
//
// 	}

	dict_free();
	return 0;
}
