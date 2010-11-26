#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define CODE_LENGTH 12
#define DICT_SIZE (1 << CODE_LENGTH)

int16_t** dict;

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

int main(int argc, char** argv){
	dict_init();
	
	dict_free();
	return 0;
}