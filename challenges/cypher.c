#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main (void) {
	char* input = malloc(100*sizeof(char));
	char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	printf("Enter key: ");
	fgets(input,100,stdin);
	int key = atoi(input);
	if (key<0||key>26) {
		printf("\nInvalid key!\n");
		free(input);
		return 0;
	}
	printf("Enter line: ");
	fgets(input,100,stdin);
	printf("\n");
	for (int i = 0; input[i]!='\n'&&input[i]!='\0'; i++) {
		for (int j = 0; j < 26; j++) {
			if (!isalpha(input[i])) {
				putchar(input[i]);
				break;
			}
			if (toupper(input[i])==alphabet[j]) {
				if ((j+key)>25) {
					if (isupper(input[i])) {
						putchar(alphabet[(j+key)-26]);
					} else {
						putchar(tolower(alphabet[(j+key)-26]));
					}
				} else {
					if (isupper(input[i])) {
						putchar(alphabet[j+key]);
					} else {
						putchar(tolower(alphabet[j+key]));
					}
				}
			}
		}
	}
	putchar('\n');
	free(input);
	return 0;
}