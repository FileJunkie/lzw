#define MAIN

#include "common.h"

uint8_t tail = 0, tail_len = 0;

void print_code(int16_t code){
	int cur_left = word_len, i;
	uint8_t tempbyte;

	if(tail_len != 0){
		tempbyte = tail << (8 - tail_len);
		tempbyte |= (uint8_t)(code >> (word_len - 8 + tail_len));
		putchar(tempbyte);
		cur_left -= 8 - tail_len;
		if(cur_left < 8){
			tail_len = cur_left;
			for(i = 0; i < 16 - tail_len; i++){
				code &= ~(1 << (16 - i - 1));
			}
			tail = (uint8_t)(code);
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
		tempbyte = (uint8_t)(code >> (word_len - 8));
		putchar(tempbyte);
		tail_len = word_len - 8;
		if(word_len > 8){
			for(i = 0; i < word_len - tail_len; i++){
				code &= ~(1 << (word_len - i - 1));
			}
			tail = (uint8_t)(code);
		}
	}
}

void print_tail_finalize(){
	putchar(tail << (8 - tail_len));
}

int dict_search(int16_t c1, int16_t c2){
	int i;

	for(i = 0; i < dict_new; i++){
		if(dict[i][0] == c1 && dict[i][1] == c2){
			return i;
		}
	}

	return -1;
}

int main(int argc, char** argv){
 	int16_t prevchar, nextchar;
 	int dict_pos;

	dict_init();

 	prevchar = getchar();
 	while((nextchar = getchar()) != EOF){
 		dict_pos = dict_search(prevchar, nextchar);
 		if(dict_pos == -1){ // string not found in dictionary. emit last one and add new
 			print_code(prevchar);
 			dict_add(prevchar, nextchar);
 			prevchar = nextchar;
 		}
 		else{ //string found in dictionary
 			prevchar = dict_pos;
 		}
 	}
 	print_code(prevchar);
 	print_tail_finalize();

	dict_free();
	return 0;
}
