#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main (void) {
	
	char input[100];
	char output[100];
	bool incomplete = true;
	bool skip = false;
	int index = 0;
	int length = 0;
	
	printf("Input strand: ");
	fgets(input,100,stdin);
	strcpy(output,input);
	for(int i = 0; input[i]!='\n'&&input[i]!='\0';i++) {
		length++;
	}
	if (length<8) 
		incomplete=false;
	while (incomplete) {
		if (length<8)
			incomplete = false;
		for (int i = 0; input[i+7]!='\0'&&input[i+7]!='\n'; i++) {
			if (input[i]=='G'&&input[i+1]=='U'&&input[i+2]=='G'&&input[i+3]=='U') {
				for (int j = i+4; input[j]!='\0'&&input[j]!='\n'; j++) {
					if (input[j]=='A'&&input[j+1]=='G'&&input[j+2]=='A'&&input[j+3]=='G') {
						for (index = 0; index < i; index++) {
							output[index]=input[index];
						}
						for (int l = j+4; input[l]!='\0'&& input[l]!='\n'; l++) {
							output[index]=input[l];
							index++;
						}
						output[index]='\0';
						strcpy(input,output);	
						skip = true;
						length = index;
						break;
					}
				}
			}
			if (skip) {
				skip = false;
				break;
			}
			if ((input[i+8]=='\0'||input[i+8]=='\n')&&(!strcmp(input,output)||length<8)) {
				incomplete=false;
				break;
			}
		}
	}
	if (output[length]=='\n') {	
		output[length]='\0';
	}
	printf("\nOutput is %s\n",output);
	
	return 0;
}