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
 
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atoi(argv[2]);
  
  return 0;
}

size_t DDM_Get_Dimension(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atoi(argv[3]);
  return 0;
}

float DDM_Get_Alfa(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return atof(argv[4]);
  return 0;
}

size_t DDM_Get_Updates(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return (size_t) (atoi(argv[2]) / 2);
  return 0;
}

size_t DDM_Get_Subscriptions(int argc, char* argv[]){
  
  if (argc == 5 && strncmp(argv[1], "alfa", 4) == 0)
    return (size_t) (atoi(argv[2]) / 2);
  return 0;
}

/**
 * OUTPUT
 */

void DDM_Write_Result(char* argv[], double total_time){
  /* write results */
  char str[100];
  strcpy(str, argv[0]);
  strcat(str, ".txt");
  FILE* fout = fopen(str, "a");
  if ( fout == NULL ) {
      printf("Error creating file %s\n", str);
      exit(-1);
  }

  fprintf(fout, "%f\n", total_time);
  fclose(fout);
}

/**
 * TIMER
 */

void DDM_Start_Timer(DDM_Timer *ddm_timer){
  ddm_timer->start = clock();
  ddm_timer->total = 0;
}

void DDM_Stop_Timer(DDM_Timer *ddm_timer){
  ddm_timer->end = clock();
  ddm_timer->total = ((float)(ddm_timer->end - ddm_timer->start)) / CLOCKS_PER_SEC;
}

float DDM_Get_Total_Time(DDM_Timer ddm_timer){
  return ddm_timer.total;
}