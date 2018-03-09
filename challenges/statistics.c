#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

int main (void) {
	
	double index = 0;	
	char input[100];
	double values[500];
	
	printf("Enter data set:\n");
	while (!feof(stdin)) {
		fgets(input,100,stdin);
		if (input[0]=='\n')
			continue;
		char* token = strtok(input," ");

		while (token) {
			values[(int)index]=atof(token);
			token = strtok(NULL, " ");
			index++;
		}
	}
	printf("\n");
	if (index == 0) {
		printf("No data!\n");
		return 0;
	}
	double sum = 0;
	for(int i = 0; i < index-1; i++) {
		sum += values[i];
	}
	double mean = sum/(index-1);
	printf("Mean = %.4f\n",mean);
	
	sum = 0;
	double difference;
	for (int i = 0; i < index-1; i++) {
		difference = 0;
		difference = values[i] - mean;
		difference *= difference;
		sum += difference;
	}	
	double variance = sum/(index-1);
	printf("Variance = %.4f\n",variance);
	
	double std_deviation = sqrt(variance);
	printf("Standard deviation = %.4f\n",std_deviation);
	
	return 0;

}