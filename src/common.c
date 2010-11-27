#include "common.h"

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

void dict_add(int16_t c1, int16_t c2){
	if(dict_new >= DICT_SIZE){
		return;
	}

	dict[dict_new] = malloc(sizeof(int16_t) * 2);
	if(dict[dict_new] == NULL){
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}
	dict[dict_new][0] = c1;
	dict[dict_new][1] = c2;

	if((dict_new & (dict_new -1)) == 0){
		word_len++;
	}
	dict_new++;
}
