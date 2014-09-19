#include "../inc/inputoutput.h"
#include "../inc/comparison.h"
#include "../inc/launcher.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  printf("number parameters: %d\n", argc);
  if ( argc != 2 ) /* argc should be 1 for correct execution */
  {
      /* We print argv[0] assuming it is the program name */
      
      printf("usage: %s filename\n", argv[0] );
  }
  else 
      printf("Right number of parameters: %s\n", argv[1]);
  
  return 0;
}
