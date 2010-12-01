#define MAIN

#include "common.h"

uint8_t tail = 0, tail_len = 0;

void print_code(int16_t code){
	int cur_left = word_len, i;
#ifdef DEBUG
	static unsigned offset = 0;
#endif
	uint8_t tempbyte;

	if(tail_len != 0){
		tempbyte = tail << (8 - tail_len);
		tempbyte |= (uint8_t)(code >> (word_len - 8 + tail_len));
		putc(tempbyte, fout);
#ifdef DEBUG
		fprintf(stderr, "Written %d at %x and %x as %x\n", code, offset, offset + 1, tempbyte);
		offset++;
#endif
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
			putc((uint8_t)(code >> tail_len), fout);
#ifdef DEBUG
			if(tail_len == 0){
				fprintf(stderr, "Written %d at %x as %x\n", code, offset, (uint8_t)(code >> tail_len));
			}
			else{
				fprintf(stderr, "Written %d at %x and %x as %x\n", code, offset, offset + 1, (uint8_t)(code >> tail_len));
			}
				offset++;
#endif
			for(i = 0; i < 16 - tail_len; i++){
				code &= ~(1 << (16 - i - 1));
			}
			tail = (uint8_t)(code);
			return;
		}
	}
	else{
		tempbyte = (uint8_t)(code >> (word_len - 8));
		putc(tempbyte, fout);
		tail_len = word_len - 8;
#ifdef DEBUG
		if(tail_len == 0){
			fprintf(stderr, "Written %d at %x as %x\n", code, offset, tempbyte);
		}
		else{
			fprintf(stderr, "Written %d at %x and %x as %x\n", code, offset, offset + 1, tempbyte);
		}
		offset++;
#endif
		if(word_len > 8){
			for(i = 0; i < word_len - tail_len; i++){
				code &= ~(1 << (word_len - i - 1));
			}
			tail = (uint8_t)(code);
		}
	}
}

void print_tail_finalize(){
	putc(tail << (8 - tail_len), fout);
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


#ifdef WIN32
 	if((fin = fopen("input_test", "rb")) == NULL){
 		fprintf(stderr, "Error opening input file\n");
 	}

	if((fout = fopen("packed_test", "wb")) == NULL){
 		fprintf(stderr, "Error opening output file\n");
 	}

#else
	fin = stdin;
	fout = stdout;
#endif

	dict_init();

 	prevchar = getc(fin);
 	while((nextchar = getc(fin)) != EOF){
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

#ifdef WIN32
	fclose(fin);
	fclose(fout);
#endif
	return 0;
}
