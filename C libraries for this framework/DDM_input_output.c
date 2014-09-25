/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

/**
 * INCLUDE
 */

#include "DDM_input_output.h"

/**
 * INPUT
 */

size_t DDM_Get_Extents(int argc, char* argv[]){
  if ( argc >= 2 ) 
    return atoi(argv[1]);
  return 0;
}

size_t DDM_Get_Dimension(int argc, char* argv[]){
  if ( argc >= 3 )
    return atoi(argv[2]);
  return 0;
}

float DDM_Get_Alfa(int argc, char* argv[]){
  if ( argc >= 4 )
    return atof(argv[3]);
  return 0;
}


/**
 * OUTPUT
 */

void DDM_Write_Result(int argc, char* argv[], double total_time){
  /* write results */
  char str[100];
  sprintf(str, "%s_%d_%d_%2.3f.txt", argv[0], DDM_Get_Extents(argc, argv), DDM_Get_Dimension(argc, argv), DDM_Get_Alfa(argc, argv));
  //strcpy(str, argv[0]);
  //strcat(str, ".txt");
  FILE* fout = fopen(str, "a");
  if ( fout == NULL ) {
      printf("Error creating file %s\n", str);
      exit(-1);
  }

  fprintf(fout, "%f\n", total_time);
  fclose(fout);
}