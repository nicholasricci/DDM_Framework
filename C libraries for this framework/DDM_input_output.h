/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

#ifndef _DDM_INPUT_OUTPUT
#define _DDM_INPUT_OUTPUT

/**
 * INCLUDE
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

/**
 * DEFINITION
 */

/** \brief definition of the max dimension can be used inside algorithms */
#define MAX_DIMENSION 5

/** \brief definition of the name of info and input files in TestsInstances -> <NameTest> */
#define TEST_INFO "info.txt"
#define TEST_INPUT "input-0.txt"

/** \brief definition of path to check folder TestsInstances instances exists and file at their inside */
#define FOLDER_CHECK "../../../TestsInstances/%s"
#define FILE_CHECK "../../../TestsInstances/%s/%s"

/** \brief definition of max length of one line in text file */
#define LINE_MAX_LENGTH 4000

/**
 * STRUCT
 */

/** \brief structure for keep timer of execution of algorithm */
typedef struct DDM_Timer{
  clock_t start, end;
  float total;
} DDM_Timer;

/** \brief structure for an extent */
typedef struct DDM_Extent{
  size_t id;
  double lower[MAX_DIMENSION];
  double upper[MAX_DIMENSION];
}DDM_Extent;

/** \brief structure for manage input in algorithm */
typedef struct DDM_Input{
  
  /*variables useful for any kind of test*/
  char type_test[100];
  size_t dimensions;
  size_t extents;
  size_t updates;
  size_t subscriptions;
  
  /*variables useful for alfa test*/
  float alfa;
  
  /*variables useful for other kind of test*/
  DDM_Extent *list_updates;
  DDM_Extent *list_subscriptions;
  
}DDM_Input;


/**
 * INPUT
 */

//TODO finish this implementation better as you can and in a coherent way 
//to reach a good compromise between my framework and another.
DDM_Input* Initialize_DDM_Input(int argc, char* argv[]);

size_t DDM_Get_Extents(int argc, char* argv[]);

size_t DDM_Get_Dimension(int argc, char* argv[]);

float DDM_Get_Alfa(int argc, char* argv[]);

size_t DDM_Get_Updates(int argc, char* argv[]);

size_t DDM_Get_Subscriptions(int argc, char* argv[]);


/**
 * OUTPUT
 */

void DDM_Write_Result(char* argv[], double total_time);

/**
 * TIMER
 */

void DDM_Start_Timer(DDM_Timer *ddm_timer);

void DDM_Stop_Timer(DDM_Timer *ddm_timer);

float DDM_Get_Total_Time(DDM_Timer ddm_timer);

#endif