#define MAIN

#include "common.h"

int16_t get_code(){
	static uint8_t tail = 0, tail_len = 0;
	int16_t result = 0;
	int newchar, i;
#ifdef DEBUG
	static unsigned offset = 0;
#endif

	if(tail_len == 0){
		if((newchar = getc(fin)) == EOF){
			return EOF;
		}
		if(word_len == 8){
#ifdef DEBUG
		fprintf(stderr, "Got %d at %x, it is it\n", newchar, offset);
		offset++;
#endif
			return newchar;
		}
		else{
#ifdef DEBUG
		fprintf(stderr, "Got %d at %x ", newchar, offset);
		offset++;
#endif
			tail_len = 16 - word_len;
			result = newchar << (8 - tail_len);
			if((newchar = getc(fin)) == EOF){
				return EOF;
			}
#ifdef DEBUG
		fprintf(stderr, "and %d at %x ", newchar, offset);
		offset++;
#endif
			tail = newchar;
			for(i = 0; i < 16 - tail_len; i++){
				tail &= ~(1 << (15 - i));
			}

			newchar >>= tail_len;
			for(i = 0; i < 8 + tail_len; i++){
				newchar &= ~(1 << (15 - i));
			}
			result |= newchar;
#ifdef DEBUG
		fprintf(stderr, "it is %d\n", result);
#endif
			return result;
		}
	}
	else{
		result = tail << (word_len - tail_len);
		for(i = 0; i < word_len - tail_len; i++){
			result &= ~(1 << i);
		}
		if((newchar = getc(fin)) == EOF){
			return EOF;
		}
#ifdef DEBUG
		fprintf(stderr, "Got %d at %x and %x ", newchar, offset - 1, offset);
		offset++;
#endif
		if(word_len - tail_len > 8){
			tail_len = 16 - (word_len - tail_len);
			newchar <<= (8 - tail_len);
				for(i = 0; i < (8 - tail_len); i++){
				newchar &= ~(1 << i);
			}
			result |= newchar;
			if((newchar = getc(fin)) == EOF){
				return EOF;
			}
#ifdef DEBUG
			fprintf(stderr, "and %x ", offset);
			offset++;
#endif
		}
		else{
			tail_len = 8 - (word_len - tail_len);
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

#ifdef DEBUG
		fprintf(stderr, "it is %d\n", result);
#endif
		return result;
	}

	return result;
}

void print_string(int16_t number){
	if(number < 256){
		putc(number, fout);
	}
	else{
		if(dict[number] == NULL){
			fprintf(stderr, "You failed\n");
			exit(1);
		}
		print_string(dict[number][0]);
		print_string(dict[number][1]);
	}
	fflush(NULL);
}

void dict_fix(int16_t next_code){
	if(dict_new <= DICT_SIZE){
		if(next_code < 256)
			dict[dict_new - 1][1] = next_code;
		else
			dict_fix(dict[next_code][0]);
	}
}

int main(int argc, char** argv){
	int16_t next_code;

#ifdef WIN32
 	if(((fin = fopen("packed_test", "rb")) == NULL) || ((fout = fopen("unpacked_test", "wb")) == NULL)){
 		fprintf(stderr, "Error opening files\n");
 	}
#else
	fin = stdin;
	fout = stdout;
#endif

	dict_init();

	if((next_code = get_code()) == EOF){
		dict_free();
		return 0;
	}
	print_string(next_code);
	dict_add(next_code, -1);
	while((next_code = get_code()) != EOF){
		dict_fix(next_code);
		print_string(next_code);
		dict_add(next_code, -1);
	}

	dict_free();
#ifdef WIN32
	fclose(fin);
	fclose(fout);
#endif
	return 0;
}
