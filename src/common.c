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
#ifdef DEBUG
	fprintf(stderr, "Added %d %d at %d\n", c1, c2, dict_new);
#endif
	if((dict_new & (dict_new -1)) == 0){
		word_len++;
#ifdef DEBUG
		fprintf(stderr, "Word length is now %d\n", word_len);
#endif
	}
	dict_new++;
}
