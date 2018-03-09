#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) { 

	char* input = (char*)malloc(100*sizeof(char));
	char* anagram = (char*)malloc(100*sizeof(char));
	char* new_input = (char*)malloc(100*sizeof(char));
	char* new_anagram = (char*)malloc(100*sizeof(char));

	int index = 0;

	printf("Enter line: ");
	fgets(input,100,stdin);
	printf("Enter anagram: ");
	fgets(anagram,100,stdin);
	printf("\n");
	
	if (strlen(input)!=strlen(anagram)) {
		printf("Not an anagram.");
		free(input);
		free(anagram);
		return 0;
	}

	for (int i = 0; i < strlen(input); i++) {
		if (
	}
}