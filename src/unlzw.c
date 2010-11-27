#define MAIN

#include "common.h"

FILE* fin;

int16_t get_code(){
	static uint8_t tail = 0, tail_len = 0;
	int16_t result = 0;
	int newchar, i;

	if(tail_len == 0){
		if((newchar = getc(fin)) == EOF){
			return EOF;
		}
		if(word_len == 8){
			return newchar;
		}
		else{
			tail_len = 16 - word_len;
			result = newchar << (8 - tail_len);
			if((newchar = getc(fin)) == EOF){
				fprintf(stderr, "Bit alignment error\n");
				exit(1);
			}
			tail = newchar;
			for(i = 0; i < 16 - tail_len; i++){
				tail &= ~(1 << (15 - i));
			}

			newchar >>= tail_len;
			for(i = 0; i < 8 + tail_len; i++){
				newchar &= ~(1 << (15 - i));
			}
			result |= newchar;
			return result;
		}
	}
	else{
		result = tail << (word_len - tail_len);
		for(i = 0; i < word_len - tail_len; i++){
			result &= ~(1 << i);
		}
		if((newchar = getc(fin)) == EOF){
			fprintf(stderr, "Bit alignment error\n");
			exit(1);
		}
		tail_len = 8 - (word_len - tail_len);
		tail = newchar;
		for(i = 0; i < 16 - tail_len; i++){
			tail &= ~(1 << (15 - i));
		}

		newchar >>= tail_len;
		for(i = 0; i < 8 + tail_len; i++){
			newchar &= ~(1 << (15 - i));
		}
		result |= newchar;
		return result;
	}

	return result;
}

int main(int argc, char** argv){

	fin = fopen("testoutput", "rb");
	if(fin == NULL){
		fprintf(stderr, "Cannot open file\n");
		exit(1);
	}

	dict_init();

	putchar(get_code());
	word_len++;
	putchar(get_code());
	putchar(get_code());
	putchar(get_code());

	dict_free();
	fclose(fin);
	return 0;
}
