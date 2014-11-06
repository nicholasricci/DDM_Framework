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
*  \brief definition of DDM_INSTANCE_MAKER
*/
#define DDM_ISTANCE_MAKER_VERSION "0.5b"
#define DDM_BENCH_VERSION "0.5b"
#define __DEBUG

/**
* \brief
*/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/**
* \brief defines the info file name
This file cointains all the info about the current instance.
*/
#define INFO_FILE_NAME "info.txt"

/**
* \brief defines the result file name
this file will store all the results of the different algorithms executed on this instance.
*/
#define RESULTS_FILE_NAME "results.txt"

/**
* \brief defines the input file name
*/
#define INPUT_FILE_NAME  "input-%" PRIu32 ".txt"

/**
* \brief defines the output file name
*/
#define OUTPUT_FILE_NAME "output-%" PRIu32 ".dat"


/** \brief The name of the log file
 *
 */
#define LOG_FILENAME "log.txt"

/** \brief The name of the csv file that contains the results list
 *
 */
#define CSV_FILENAME "results.csv"

/** \brief The name of the folter that contains the execution results
 *
 */
#define RESULTS_FOLDER_NAME "results"
/**
* \brief the max length of a file line
*/
#define LINE_MAX_LENGTH 4000

/**
* \brief The size of the buffer used to concatenate strings one another.
*/
#define BUFFER_SIZE 500

/**
* \brief defines boolean type, true is 1, false is 0
*/
#define bool uint_fast8_t
#define true 1
#define false 0

/**
* \brief The maximum size of a file path
*/
#define MAX_FILE_PATH_LENGHT 400

#define MAX_FILENAME_LENGTH 100

/**
* \brief the maximum number of dimensions that can be stored (this is done to avoid a further malloc for each extent).
*/
#define MAX_DIMENSIONS 4

/**
* \brief a macro that generates a double precision random rumber between _min and _max
*/
#define BIGRANDOM(_min, _max)		( (SPACE_TYPE) (_min + ((double)_max - _min + 1) * ((rand() * ((double)RAND_MAX + 1) + rand()) / (RAND_MAX * ((double)RAND_MAX + 2) + 1))) )

/**
* \brief a macro that returns the smallest between two numbers
*/
#define MIN(_a,_b)					(_a<_b?_a:_b)

/**
* \brief a macro that returns the biggest between two numbers
*/
#define MAX(_a,_b)					(_a>_b?_a:_b)

/**
 * \brief a macro that returns the filename of a given filepath
 */
#define FILENAME(_x)				( strrchr(_x, '/') ? strrchr(_x, '/') + 1 : _x )

/**
 * \brief definition for error handling
 */

#define FILE_NAME_SIZE 100
#define FUNCTION_NAME_SIZE 100

/**
 * \brief DEFINITION OF BITVECTOR
 */

/**
\brief If defined a 64 unsigned integer is used as base element.
Else a 32 bit integer is used instead
*/
#define BITVECTOR_64
#ifdef BITVECTOR_64

    typedef uint64_t BITVECTOR_TYPE;
    #define MASK_TYPE uint64_t
    #define FULL_MASK UINT64_MAX
    #define BIT_LENGTH 64
    #define VECTOR_MASK UINT64_C(0x0000000000000001)
    /** \brief The value of the most significant bit in an element of the bit vector.
    */
    #define BITVEC_ELEM_MAX_BIT			0x8000000000000000

#else
    typedef uint32_t BITVECTOR_TYPE;
    #define MASK_TYPE uint32_t
    #define FULL_MASK UINT32_MAX
    #define BIT_LENGTH 32
    #define VECTOR_MASK UINT32_C(0x00000001)
    /** \brief The value of the most significant bit in an element of the bit vector.
   */
    #define BITVEC_ELEM_MAX_BIT			0x80000000
#endif

#define BIT_VEC_WIDTH(_bits)		( (BIT_LENGTH + _bits - 1) / BIT_LENGTH )
    /** \brief Returns the element number which contains the nth bit of the bit vector.
*/
#define BIT_TO_POS(_n)				( _n / BIT_LENGTH )
    /** \brief Returns the bit position in the given element of the bit vector.
*/
#define BIT_POS_IN_VEC(_n, _elem)	( _n - (_elem * BIT_LENGTH) )

/** \brief Returns the value of the nth bit in descending order (bit 0 == 2^31 or 2^63).
*/
#define DBIT(_n)					( BITVEC_ELEM_MAX_BIT >> _n )

/** \brief Returns the value of the nth bit in ascending order (bit 0 == 1).
*/
#define ABIT(_n)					( 0x00000001 << _n )

/** \brief Sets to true the true bits in the mask.
*/
#define BIT_SET(_y, _mask)			( _y |= (_mask) )

/** \brief Checks whether the nth bit is set (ascending order).
*/
#define BIT_ACHECK(_y, _n)			( _y & (0x00000001 << _n) ) /*TODO NOTA nel file di mandrioli è l'opposto*/

/** \brief Checks whether the nth bit is set (descending order).
*/
#define BIT_DCHECK(_y, _n)			( _y & (BITVEC_ELEM_MAX_BIT >> _n) )

/** \brief Sets to false the true bits in the mask.
*/
#define BIT_CLEAR(_y, _mask)		( _y &= ~(_mask) )


/** \brief Flips the bits (true becomes false, false becomes true) that are true in the mask.
*/
#define BIT_FLIP(_y, _mask)			( _y ^= (_mask) )

#define POS_TO_MASK(_pos) ((MASK_TYPE)((VECTOR_MASK)<<(_pos % BIT_LENGTH)))

typedef BITVECTOR_TYPE* bitvector;
typedef bitvector* bitmatrix;

/******************************************
 ****************** ENUM ******************
 ******************************************/

/**
 * \brief Enum for error codes.
 */
typedef enum
{
	err_none					= 0,
	err_unhandled				= 1,
	err_generic					= 2,
	err_alloc					= 3,
	err_file					= 4,
	err_invalid_input			= 5,
	err_too_many_dim			= 6,
	err_threads					= 7
} _ERR_CODE;

typedef enum InitialValue{
zero,   /**< Every element of the bitmatrix is set to zero */
one,    /**< Every element of the bitmatrix is set to one */
uninitialized  /**< Every element of the bitmatrix has a random value  */
}InitialValue;

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
  double lower[MAX_DIMENSIONS];
  double upper[MAX_DIMENSIONS];
}DDM_Extent;

/** \brief structure for manage input in algorithm */
typedef struct DDM_Input{

  /*variables useful for any kind of test*/
  char type_test[100];
  char executable_name[100];
  size_t dimensions;
  size_t extents;
  size_t updates;
  size_t subscriptions;

  /*variables useful for alfa test*/
  float alfa;

  /*variables useful for other kind of test*/
  DDM_Extent *list_updates;
  DDM_Extent *list_subscriptions;

  /*variable useful for timer*/
  DDM_Timer timer;

  /*variable useful to store result*/
  bitmatrix result;

}DDM_Input;


/**
 * \brief Definition of structures for error
 */

/**
 *\brief Structure containing error data.
 */
typedef struct
{
	_ERR_CODE code;						///< error code
	char file[FILE_NAME_SIZE];			///< file who generated the error
	char function[FUNCTION_NAME_SIZE];	///< function who generated the error
	uint32_t line;						///< line of the file who generated the error
} _err_t;

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

/**
 * \brief Set bit to 1 for update row and subscription column
 * \param ddm_input structure variable DDM_Input
 * \param update_row is update id
 * \param subscription_column is subscription id
 */
void DDM_Set_Bit_Mat_Result(DDM_Input *ddm_input, const uint64_t update_row, const uint64_t subscription_column);

/**
 * \brief Execute the AND operation between two matrix and set the result in bitmatrix inside ddm_input
 * \param ddm_input structure variable DDM_Input
 * \param mask is a bitvector
 */
void DDM_AND_Op_With_Bitmatrix(DDM_Input *ddm_input, const bitmatrix mask);

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

/**
 * \brief Write time of execution in a file called argv[0], name of executable
 * \param *argv useful for the executable name
 * \param total_time contains the total time of execution.
 */
void DDM_Write_Result(DDM_Input ddm_input);

/**
 * \brief Count matches from bitmatrix
 * \param updates number of rows of bitmatrix
 * \param subscriptions number of columns of bitmatrix
 */
uint64_t DDM_Count_Matches(DDM_Input *ddm_input, const uint64_t updates, const uint64_t subscriptions);

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
 **************** LOG FILE ****************
 ******************************************/

void WriteLogLine(char * line);

/******************************************
 ***************** ERROR ******************
 ******************************************/

/** \brief Sets the error global variable
 *
 * \param code const _ERR_CODE, the error code that just happened
 * \param file const char*, the file where the error occurred.
 * \param function const char*, the function where the error occurred
 * \param line const uint32_t, the line where the error occurred
 * \return _ERR_CODE, the error code provided is returned for enable function call concatenation.
 *
 */
_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const uint32_t line);


/** \brief Prints the error string that is store in the error global variable in that moment.
 *
 * \return _ERR_CODE
 *
 */
_ERR_CODE print_error_string(void);

/** \brief Prints a provided error string
 *
 * \param string char*, The string that must be print out.
 * \param force_exit bool, if it is true the execution will be aborted just after printing the error string.
 * \return void
 *
 */
void print_runtime_error_string(char * string, bool force_exit);

/******************************************
 *************** BITVECTOR ****************
 ******************************************/

bitvector init_bitvector(uint64_t size);

void free_vector(bitvector * vector);

void reset_vector(bitvector vector, const uint64_t size);

void set_vector(bitvector vector, const uint64_t size);

bool isSet(bitvector vector, const uint64_t pos);

void negate_vector(bitvector vector, const uint64_t size);

void set_element(bitvector vector, const uint64_t pos);

void reset_element(bitvector vector, const uint64_t pos);

void copy_vector(bitvector vdest, bitvector vsource, const uint64_t size);

void negate_element(bitvector vector, const uint64_t pos);

void print_bitvector(bitvector vector, const uint64_t size);

void vector_bitwise_and(const bitvector result, const bitvector mask, const uint64_t size);

void vector_bitwise_or(const bitvector result, const bitvector mask, const uint64_t size);

/******************************************
 *************** BITMATRIX ****************
 ******************************************/

 /**
 * \brief initialize a bitmatrix.
 * This method initialize a bitmatrix
 * \param out a bitmatrix * where method's result will be stored.
 * \param rows the number of rows of the virtual matrix (# of update extents)
 * \param columns the number of columns of the virtual matrix (# of subscription extents)
 * \param initial_value the value that the bitmatrix cells has to be initialized to.
 */
_ERR_CODE init_bit_matrix (bitmatrix * matrix_pointer, const uint64_t rows, const uint64_t columns, const InitialValue initial_value);

/**
 * \brief set a single bitmatrix' bit to 1.
 * A single bit is set to one. This means that update extent having ID 'row' is matched with subscription extent having ID 'column'
 * \param mat the bitmatrix whose bit must be set.
 * \param row the row index of the virtual matrix to set (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to set (i.e. subscription extent's ID)
 */
void set_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief set a single bitmatrix' bit to 0.
 * A single bit is set to zero. This means that update extent having ID 'row' is not matched with subscription extent having ID 'column'
 * \param mat the bitmatrix whose bit must be set.
 * \param row the row index of the virtual matrix to set (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to set (i.e. subscription extent's ID)
 */
void reset_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief Complements a single bitmatrix bit.
 * A single bit is complemented (bit = 1-bit);
 * \param mat the bitmatrix whose bit must be modified.
 * \param row the row index of the virtual matrix to modify (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to modify (i.e. subscriber extent's ID)
 */
void negate_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief Reset a whole bitmatrix' bits to 0.
 * This is an optimized implementation of resetting every single bit of the bitmatrix to 0. This means that all subscription exents are not matched with any update extent
 * Please double check that the provided parameters are correct, otherwise the result will be undefined.
 * \param mat the bitmatrix whose bit must be reset.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void reset_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
 * \brief Set a whole bitmatrix bits to 1.
 * This is an optimized implementation of setting every single bit of the bitmatrix to 1. This means that all subscription exents are matched with any update extent
 * Please double check that the provided parameters are correct, otherwise the result will be undefined.
 * \param mat the bitmatrix whose bit must be set.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void set_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
 * \brief Complements a whole bitmatrix bits.
 * This is an optimized implementation of complementing every single bit of the bitmatrix.
 * \param mat the bitmatrix whose bits must be reset.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void negate_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
* \brief this procedure frees memory occupied by a bitmatrix.
* \param mat the bitmatrix * whose memory has to be freed.
*/
void free_bit_matrix(bitmatrix * mat);

/**
* \brief Retrieves the current value of a cell.
* \param mat, a bitmatrix * that points to the desired bitmatrix.
* \param row, the index of the row (update extent ID)
* \param col, the index of the column (subscriber extent ID)
* \return true if the bit is set to one, false otherwise.
*/
bool get_bit(const bitmatrix mat,const uint64_t row, const uint64_t col);
/**
 * \brief This function checks that all bits set in mat1 are also set in mat2.
 * This function is used to evaluate if mat2 (the computed_solution) is a superset of mat1 (the optimal solution)
 * \param rows, the number of rows of the virtual matrix
 * \param columns, the number of column of the virtual matrix
 * \return true if mat1 is optimal or suboptimal, false otherwise
*/
bool checkSet (const bitmatrix mat1, const bitmatrix mat2, const int rows, const int columns);

/**
 * \brief prints a bitmatrix to a binary file.
 * This procedure prints out a bitmatrix to a binary file.
 * \param mat the bitmatrix that has to be printed out
 * \param filename a string cointaining the file path
 * \param override_file a boolean value. If it's true the file will be overwritten if it already exists, if it's false the program will ask for user confirmation
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
*/
void print_bit_matrix(const bitmatrix mat, const char * filename, const bool override_file, const uint64_t rows, const uint64_t columns);

/**
* \brief this procedure reads a bitmatrix from a file.
* This procedure loads a bitmatrix from a binary file.
* \param out a bitmatrix ** where the result will be stored
* \param filename a string that cointains the file path
* \param rows a pointer where will be stored the number of rows read from the file
* \param columns a pointer where will be stored the number of columns read from the file
* \return an _ERR_CODE, err_none if everything went well
*/
_ERR_CODE read_bit_matrix (bitmatrix * mat, const char * filename, uint64_t * rows, uint64_t * columns);

/**
 * \brief This function computes the distance between to bitmatrix
 * This function counts how many bits have not the same value in both bitmatrix (bitmatrix' order doesn't matter).
 * \param matrix1, the first bitmatrix
 * \param matrix2, the second bitmatrix
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 * \return int the number of bit that don't have the same value in both bitmatrix, -1 if bitmatrix dimensions (number of rows and columns) don't match
*/
int compareBitmatix(const bitmatrix matrix1, const bitmatrix matrix2, const uint64_t rows, const uint64_t columns);

/** \brief Counts how many bits of a bitmatrix are set.
 *
 * \param   mat, the bitmatrix whose bit has to be count
 * \param   rows, the number of rows of the bitmatrix
 * \param   columns, the number of columns of the bitmatrix
 * \return  the number of bit set to one.
 *
 */

uint64_t count_ones(const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/** \brief This method prints a human readable bitmatrix on stdout
 * It is advisable to use this method on small bitmatrix only.
 *
 * \param in, the bitmatrix to be printed
 * \param   rows, the number of rows of the bitmatrix
 * \param   columns, the number of columns of the bitmatrix
 *
 */

void print_readable_bitmatrix(const bitmatrix in, int iteration, int rows, int columns);

#endif
