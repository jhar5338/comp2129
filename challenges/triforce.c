#include <stdio.h>
#include <stdlib.h>

void top_triangle(int height,int row);
void bot_triangles(int height,int row);
void spaces(int spaces);
void underscores(int underscores);

void invalid (void) {
	printf("Invalid height.\n");
	exit(0);
}

int main (void) {
	
	char input[4];
	
	printf("Enter height: ");
	fgets(input,4,stdin);
	printf("\n");
	
	int height = atoi(input);
	
	if(height<2||height>20) 
		invalid();
	
	int row = height+height-1;
	
	top_triangle(height,row);
	row -= height;
	bot_triangles(height,row);
	
	return 0;
}

void top_triangle(int height,int row) {
	
	for (int i = 0; i < height-1; i++,row--) {
		spaces(row);
		printf("/");
		spaces(i*2);
		printf("\\\n");
	}
	spaces(height);
	printf("/");
	underscores((height-1)*2);
	printf("\\\n");
}

void bot_triangles(int height,int row) {
	
	for (int i = 0; i < height-1; i++,row--) {
		spaces(row);
		printf("/");
		spaces(i*2);
		printf("\\");
		spaces((height-i-1)*2);
		printf("/");
		spaces(i*2);
		printf("\\\n");
	}
	printf("/");
	underscores((height-1)*2);
	printf("\\/");
	underscores((height-1)*2);
	printf("\\\n");
}

void spaces(int spaces) {
	for (int i = 0;i < spaces; i++) {
		printf(" ");
	}
}

void underscores(int underscores) {
	for (int i = 0;i < underscores; i++) {
		printf("_");
	}
}