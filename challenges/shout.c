#include <stdio.h>
#include <ctype.h>

int main (void) {
	
	int index = 0;	
	char str[100];
	char c;	
	
	for(int i = 0;!feof(stdin);i++) {		
		c = getchar();
		if (isalpha(c))
			str[i] = toupper(c);
		
		else str[i] = c;
		index++;		
	}	
		
	for(int i = 0;i<index-1;i++) 
		putchar(str[i]);		
		
	return 0;

}