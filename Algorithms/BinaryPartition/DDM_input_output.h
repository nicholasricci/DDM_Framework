/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

#ifndef _DDM_INPUT_OUTPUT
#define _DDM_INPUT_OUTPUT

/******************************************
 **************** INCLUDE *****************
 ******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

/******************************************
 ************** DEFINITIONS ***************
 ******************************************/

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

/******************************************
 ***************** STRUCT *****************
 ******************************************/

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


/******************************************
 ***************** INPUT ******************
 ******************************************/

/**
 * \brief This function initializie the ddm_input structure. Fill in extents, updates, subscription,
 * alfa, type_test and in case of the test is different to alfa then fill also list_updates and list_subscriptions
 * with all ready extents.
 * \param argc argument count
 * \param *argv argument values
 */
DDM_Input* DDM_Initialize_Input(int argc, char* argv[]);

/** 
 * \brief This function return 1 if is an alfa test or 0 if other kind of test
 * It useful to check this type:
 * - if is equal to alfa then you must used DDM_Get_Extents, DDM_Get_Dimensions, DDM_Get_Alfa, DDM_Get_Updates, DDM_Get_Subscriptions
 * and then allocate variable for these and create random values of subscriptions and updates extents you according to alfa's values;
 * - if is different to alfa you must used DDM_Get_List_Updates, DDM_Get_List_Subscriptions to retrieve the list of updates and
 * subscriptions from test instance file and adopt this values with your internal structure
 * \param ddm_input read-only structure variable DDM_Input
 */
int DDM_Is_Alfa_Test(DDM_Input ddm_input);

/**
 * \brief This function returns the type of test if you're ready to execute.
 * \param ddm_input read-only structure variable DDM_Input
 */
char* DDM_Get_Test_Type(DDM_Input ddm_input);

/**
 * \brief Return the number of total extents used in this execution = updates + subscriptions
 * \param ddm_input read-only structure variable DDM_Input
 */
size_t DDM_Get_Extents(DDM_Input ddm_input);

/**
 * \brief Return the number of dimensions used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
size_t DDM_Get_Dimensions(DDM_Input ddm_input);

/**
 * \brief Return the alfa overlapping degree used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
float DDM_Get_Alfa(DDM_Input ddm_input);

/**
 * \brief Return the number of updates used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
size_t DDM_Get_Updates(DDM_Input ddm_input);

/**
 * \brief Return the number of subscriptions used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
size_t DDM_Get_Subscriptions(DDM_Input ddm_input);

/**
 * \brief Return the array of updates used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
DDM_Extent* DDM_Get_Updates_List(DDM_Input ddm_input);

/**
 * \brief Return the array of subscriptions used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
DDM_Extent* DDM_Get_Subscriptions_List(DDM_Input ddm_input);

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

/**
 * \brief Write time of execution in a file called argv[0], name of executable
 * \param *argv useful for the executable name
 * \param total_time contains the total time of execution.
 */
void DDM_Write_Result(char* argv[], double total_time);

/******************************************
 ***************** TIMER ******************
 ******************************************/

/**
 * \brief Initialize the structure DDM_Timer and start timer 
 * \param ddm_timer structure for manage execution time
 */
void DDM_Start_Timer(DDM_Timer *ddm_timer);

/**
 * \brief Stop timer and calculate the total time 
 * \param ddm_timer structure for manage execution time
 */
void DDM_Stop_Timer(DDM_Timer *ddm_timer);

/**
 * \brief Return the value of execution's time 
 * \param ddm_timer structure for manage execution time
 */
float DDM_Get_Total_Time(DDM_Timer ddm_timer);

#endif