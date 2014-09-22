#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <values.h>

char	*filename;
char	buffer[1024];

FILE	*input_file;

int	finished = 0;
int	iterations = 0;

float	value;
float	sum = 0;

int main(int argc, char *argv[]) {
	
        /*      Command-line parameters */
        filename = argv[1];
	
	input_file = fopen(filename, "r");
		
	while (!finished) {

		
			
		if (fgets(buffer, 1024, input_file) != NULL ) {
			
			sscanf(buffer, "%f\n", &value);
//			printf("%d\t%f\n", iterations, value);
			
			sum += value;
			iterations++;
		}	
		else	finished = 1;
	}
		
	printf("%3.4f\n", sum / iterations);
	
	fclose(input_file);
	
	return(0);	
}
