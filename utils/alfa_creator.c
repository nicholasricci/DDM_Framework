#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

const float Lmax = 1.0e6; /* dimension length */

int main(int argc, char *argv[]){
 
  //argv[1] is extent
  //argv[2] is dimension
  //argv[3] is alfa
  //argv[4] is filename
  uint64_t i;
  uint16_t k;
  uint64_t extents = atoi(argv[1]);
  uint64_t updates = extents / 2;
  uint64_t subscriptions = extents / 2;
  uint16_t dimensions = atoi(argv[2]);
  float alfa = atof(argv[3]);
  float l = alfa * Lmax / ( (float) updates + subscriptions );
  char to_write[1000];
  int64_t lower, upper;
  FILE *file_input;
  
  srand(time(NULL));
  
  if ((file_input = fopen(argv[4], "r")) != NULL){
    fclose(file_input);
    remove(argv[4]);
  }
  if ((file_input = fopen(argv[4], "w+")) != NULL){
    sprintf(to_write, "#Subscriptions <id> <D1 edges> [<D2 edges>]...\n");
    fputs(to_write, file_input);
    for (i = 0; i < subscriptions; ++i){
      sprintf(to_write, "%"PRIu64"", i);
      for (k = 0; k < dimensions; ++k){
	lower = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	upper = lower + l;
	sprintf(to_write, "%s %"PRId64" %"PRId64"", to_write, lower, upper); 
      }
      sprintf(to_write, "%s\n", to_write);
      fputs(to_write, file_input);
    }
    sprintf(to_write, "Updates <id> <D1 edges> [<D2 edges>]...\n");
    fputs(to_write, file_input);
    for (i = 0; i < updates; ++i){
      sprintf(to_write, "%"PRIu64"", i);
      for (k = 0; k < dimensions; ++k){
	lower = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	upper = lower + l;
	sprintf(to_write, "%s %"PRId64" %"PRId64"", to_write, lower, upper); 
      }
      sprintf(to_write, "%s\n", to_write);
      fputs(to_write, file_input);
    }
    fclose(file_input);
  } 
  
  return 0;
}
