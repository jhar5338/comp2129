#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void) {
	char* input = malloc(100*sizeof(char));
	printf("Enter strand: ");
	if(fgets(input,100,stdin)==NULL||!strcmp(input,"")) {
		printf("\n\nNo strand provided.\n");
		free(input);
		return 0;
	}
	printf("\n");
	printf("Complementary strand is ");
	for (int i = 0; input[i]!='\n'&&input[i]!='\0'; i++) {
		if (input[i]=='A')
			putchar('T');
		else if (input[i]=='T')
			putchar('A');
		else if (input[i]=='a')
			putchar('t');
		else if (input[i]=='t')
			putchar('a');
		else if (input[i]=='C')
			putchar('G');
		else if (input[i]=='G')
			putchar('C');
		else if (input[i]=='c')
			putchar('g');
		else if (input[i]=='g')
			putchar('c');
		else
			putchar('x');
	}
	putchar('\n');
	free(input);
	return 0;
}