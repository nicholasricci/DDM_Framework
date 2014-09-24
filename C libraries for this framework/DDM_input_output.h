/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

#ifndef _DDM_INPUT_OUTPUT
#define _DDM_INPUT_OUTPUT
#endif

/**
 * INPUT
 */

size_t DDM_Get_Extents(int argc, char* argv[]);

size_t DDM_Get_Dimension(int argc, char* argv[]);

float DDM_Get_Alfa(int argc, char* argv[]);


/**
 * OUTPUT
 */

void DDM_Write_Result(char* argv[]);