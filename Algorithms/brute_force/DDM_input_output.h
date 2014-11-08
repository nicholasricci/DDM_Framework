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
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

/******************************************
 ************** DEFINITIONS ***************
 ******************************************/

/** \brief definition of the name of info and input files in TestsInstances -> <NameTest> */
#define TEST_INFO "info.txt"
#define TEST_INPUT "input-0.txt"

/** \brief definition of path to check folder TestsInstances instances exists and file at their inside */
#define FOLDER_CHECK "../../../TestsInstances/%s"
#define FILE_CHECK "../../../TestsInstances/%s/%s"


/**
* \brief the max length of a file line
*/
#define LINE_MAX_LENGTH 4000

/**
* \brief the maximum number of dimensions that can be stored (this is done to avoid a further malloc for each extent).
*/
#define MAX_DIMENSIONS 4

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
  uint64_t id;
  int64_t lower[MAX_DIMENSIONS];
  int64_t upper[MAX_DIMENSIONS];
}DDM_Extent;

/** \brief structure for manage input in algorithm */
typedef struct DDM_Input{

  /*variables useful for any kind of test*/
  char type_test[100];
  char executable_name[100];
  uint16_t dimensions;
  uint64_t extents;
  uint64_t updates;
  uint64_t subscriptions;

  /*variables useful for alfa test*/
  float alfa;

  /*variables useful for other kind of test*/
  DDM_Extent *list_updates;
  DDM_Extent *list_subscriptions;

  /*variable useful for timer*/
  DDM_Timer timer;

  /*variable useful to store result*/
  uint_fast8_t **result_mat;

}DDM_Input;

/******************************************
 ****************** ENUM ******************
 ******************************************/

 typedef enum{
    zero,
    one
 } mat_value;

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
uint64_t DDM_Get_Extents(DDM_Input ddm_input);

/**
 * \brief Return the number of dimensions used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
uint16_t DDM_Get_Dimensions(DDM_Input ddm_input);

/**
 * \brief Return the alfa overlapping degree used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
float DDM_Get_Alfa(DDM_Input ddm_input);

/**
 * \brief Return the number of updates used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
uint64_t DDM_Get_Updates(DDM_Input ddm_input);

/**
 * \brief Return the number of subscriptions used in this execution
 * \param ddm_input read-only structure variable DDM_Input
 */
uint64_t DDM_Get_Subscriptions(DDM_Input ddm_input);

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

/**
 *  \brief This function set matrix cell to 1 at update row and subscription column.
 *  \param ddm_input: to manage the matrix,
 *  \param update: row where you want to set,
 *  \param subscription: column where you want to set.
 */
void DDM_Set_Matrix_Update_Subscription(DDM_Input *ddm_input, uint64_t update, uint64_t subscription);

/**
 *  \brief This function do the AND operation between ddm_input->result_mat and mask and store the result in ddm_input->result_mat
 *  \param ddm_input: to manage the matrix,
 *  \param mask: to do the and operation,
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions.
 */
void DDM_And_Operation_Between_Matrices(DDM_Input *ddm_input, uint_fast8_t **mask, uint64_t updates, uint64_t subscriptions);

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

/**
 * \brief Write time of execution in a file called argv[0], name of executable and write the matrix in result_mat.txt file
 * \param ddm_input: to store the matrix and the total time
 */
void DDM_Write_Result(DDM_Input ddm_input);

/**
 * \brief Function that count matches found.
 * \param ddm_input: to store the matrix and the total time
 */
uint64_t DDM_Count_Matches(DDM_Input *ddm_input);

/******************************************
 ***************** TIMER ******************
 ******************************************/

/**
 * \brief Initialize the structure DDM_Timer and start timer
 * \param ddm_timer structure for manage execution time
 */
void DDM_Start_Timer(DDM_Input *ddm_input);

/**
 * \brief Stop timer and calculate the total time
 * \param ddm_timer structure for manage execution time
 */
void DDM_Stop_Timer(DDM_Input *ddm_input);

/**
 * \brief Return the value of execution's time
 * \param ddm_timer structure for manage execution time
 */
float DDM_Get_Total_Time(DDM_Input ddm_input);

/******************************************
 ***************** UTILS ******************
 ******************************************/

void print_list_updates_and_subscriptions(DDM_Input ddm_input);

/******************************************
 ************* RESULT MATRIX **************
 ******************************************/

/**
 *  \brief Function to create a matrix with updates rows and subscriptions columns.
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions.
 */
uint_fast8_t** create_result_matrix(uint64_t updates, uint64_t subscriptions);

/**
 *  \brief Function that reset mat with an initial value: zero or one.
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions,
 *  \param value: enum type value can be zero or one.
 */
void reset_mat(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions, mat_value value);

/**
 *  \brief Function that do an AND operation between matrix and his mask, result in mat.
 *  \param mat: matrix which store the result,
 *  \param mask: matrix which do the AND operation,
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions.
 */
void and_operation_between_matrices(uint_fast8_t **mat, uint_fast8_t **mask, uint64_t updates, uint64_t subscriptions);

/**
 *  \brief set value of matrix cell with zero or one
 *  \param mat: matrix variable
 *  \param update: row to set
 *  \param subscription: column to set
 *  \param value: value to set, zero or one
 */
void set_value_mat(uint_fast8_t **mat, uint64_t update, uint64_t subscription, mat_value value);

/**
 *  \brief Function that print matrix.
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions.
 */
void print_matrix(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions);

 /**
 *  \brief Function that return the number of ones in matrix
 *  \param updates: number of updates,
 *  \param subscriptions: number of subscriptions.
 */
uint64_t count_ones_matrix(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions);

#endif
