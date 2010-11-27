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

void print_string(int16_t number){
	if(number < 256){
		putchar(number);
	}
	else{
		if(dict[number] == NULL){
			fprintf(stderr, "You failed\n");
			exit(1);
		}
		print_string(dict[number][0]);
		putchar(dict[number][1]);
	}
}

int main(int argc, char** argv){
	int16_t next_code;

	fin = fopen("testoutput", "rb");
	if(fin == NULL){
		fprintf(stderr, "Cannot open file\n");
		exit(1);
	}

	dict_init();

	if((next_code = get_code()) == EOF){
		dict_free();
		return 0;
	}
	print_string(next_code);
	dict_add(next_code, -1);
	while((next_code = get_code()) != EOF){
		if(dict_new <= DICT_SIZE){
			dict[dict_new - 1][1] = next_code;
		}
		print_string(next_code);
		dict_add(next_code, -1);
	}

	dict_free();
	fclose(fin);
	return 0;
}
