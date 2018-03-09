#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] )  {
	
	if(argc > 3) {
		printf("Too many arguments\n");
		return 0;
	} else if (argc == 1) {
		printf("No arguments\n");
		return 0;
	} else if (argc < 3) {
		printf("Not enough arguments\n");
		return 0;
	} else if (strlen(argv[1])!=strlen(argv[2])){
		printf("Invalid arguments\n");
		return 0;
	}
	
	char* find = (char*)malloc(100*sizeof(char));
	char* replace = (char*)malloc(100*sizeof(char));
	char* in = (char*)malloc(100*sizeof(char));
	strcpy(find,argv[1]);
	strcpy(replace,argv[2]);
	
	char** input = (char**)malloc(1000*sizeof(char*));
	int index = 0;
	while(!feof(stdin)) {
		input[index] = (char*)malloc(100*sizeof(char));
		fgets(in,100,stdin);
		strcpy(input[index],in);
		index++;
	}
	char* curr = (char*)malloc(100*sizeof(char));
	for (int i = 0; i < index; i++) {
		for (int j = 0; j < strlen(input[i]); j++) {
			strcpy(curr,input[i]);
			for (int k = 0; k < strlen(find); k++) {
				if (curr[j]==find[k]) {
					curr[j] = replace[k];
				}
			}
			strcpy(input[i],curr);
		}
	}
	for (int i = 0; i < index-1; i++) 
		printf("%s",input[i]);
	
	for (int i = 0; i < index; i++) 
		free(input[i]);
	
	free(in);
	free(curr);
	free(find);
	free(replace);
	free(input);
	return 0;
}