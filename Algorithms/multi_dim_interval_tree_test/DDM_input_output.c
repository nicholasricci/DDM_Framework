/**
 *This library is reccomended for who want to use your algorithms inside this DDM framework.
 *To use this library needs to copy this files .c and .h into your folder and linked in your make file.
 *In this library contains the function for the input and the output.
 *The output: it creates a file .txt with executable name of the program and leave the file in the bin folder.
 *The input: check if parameters contain correct value and return extents, alfa and dimension.
 */

/******************************************
 **************** INCLUDE *****************
 ******************************************/

#include "DDM_input_output.h"

/******************************************
 ***************** INPUT ******************
 ******************************************/

DDM_Input* DDM_Initialize_Input(int argc, char* argv[]){
    DIR *dir;
    FILE *file_input;
    char test[1000], filetest[1000];
    DDM_Input *ddm_input = (DDM_Input *) malloc(sizeof(DDM_Input));
    int i, j, nchar;
    char line[LINE_MAX_LENGTH];
    char *addr_line;
    char alfa_name[100];
    //char **tokens;

    if (argc == 5){
        sprintf(ddm_input->executable_name, argv[0]);
        sprintf(ddm_input->type_test, "%s", argv[1]);
        if (strncmp(argv[1], "alfa", 4) == 0){
            ddm_input->extents = atoi(argv[2]);
            ddm_input->dimensions = atoi(argv[3]);
            ddm_input->alfa = atof(argv[4]);
            ddm_input->updates = (size_t) (ddm_input->extents / 2);
            ddm_input->subscriptions = (size_t) (ddm_input->extents / 2);
            sprintf(alfa_name, "ALFA_%s_%zu_%zu", argv[4], ddm_input->extents, ddm_input->dimensions);
            //printf("\n%s\n", alfa_name);
            sprintf(test, FOLDER_CHECK, alfa_name);
            sprintf(filetest, FILE_CHECK, alfa_name, TEST_INPUT);
        }else{
            ddm_input->dimensions = atoi(argv[2]);
            ddm_input->updates = atoi(argv[3]);
            ddm_input->subscriptions = atoi(argv[4]);
            ddm_input->extents = ddm_input->updates + ddm_input->subscriptions;
            ddm_input->alfa = 0.0;
            sprintf(test, FOLDER_CHECK, argv[1]);
            sprintf(filetest, FILE_CHECK, argv[1], TEST_INPUT);
        }
        if ((dir = opendir(test)) != NULL){
            //Directory Exists
            //Check if file input-0 exists
            if ((file_input = fopen(filetest, "r")) != NULL){
                //printf("\nFile %s exists!\n", TEST_INPUT);
                ddm_input->list_subscriptions = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->subscriptions);
                ddm_input->list_updates = (DDM_Extent *) malloc(sizeof(DDM_Extent) * ddm_input->updates);
                fgets(line, sizeof line, file_input);
                for (i = 0; i < ddm_input->subscriptions; ++i){
                    fgets(line, sizeof line, file_input);
                    sscanf(line, "%zu%n", &(ddm_input->list_subscriptions[i].id), &nchar);
                    addr_line = line + nchar;
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sscanf(addr_line, " %lf %lf %n", &(ddm_input->list_subscriptions[i].lower[j]), &(ddm_input->list_subscriptions[i].upper[j]), &nchar);
                        addr_line = line + nchar;
                    }
                    //printf("%zu\n", ddm_input->list_subscriptions[i].id);
                }

                fgets(line, sizeof line, file_input);
                for (i = 0; i < ddm_input->updates; ++i){
                    fgets(line, sizeof line, file_input);
                    sscanf(line, "%zu%n", &(ddm_input->list_updates[i].id), &nchar);
                    addr_line = line + nchar;
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sscanf(addr_line, " %lf %lf %n", &(ddm_input->list_updates[i].lower[j]), &(ddm_input->list_updates[i].upper[j]), &nchar);
                        addr_line = line + nchar;
                    }
                }

                //TEST
                /*for (i = 0; i < ddm_input->subscriptions; ++i){
                    sprintf(test, "%d ", ddm_input->list_subscriptions[i].id);
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sprintf(test, "%s %lf %lf ", test, ddm_input->list_subscriptions[i].lower[j], ddm_input->list_subscriptions[i].upper[j]);
                    }
                    printf("%s\n", test);
                }
                for (i = 0; i < ddm_input->updates; ++i){
                    sprintf(test, "%d ", ddm_input->list_updates[i].id);
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sprintf(test, "%s %lf %lf ", test, ddm_input->list_updates[i].lower[j], ddm_input->list_updates[i].upper[j]);
                    }
                    printf("%s\n", test);
                }*/
                fclose(file_input);
            }else{
                printf("\nFile info or input doesn't exist!\n");
                free(ddm_input);
                ddm_input = NULL;
            }
            //Close directory
            closedir(dir);
        }else if (ENOENT == errno){
            printf("\nDirectory doesn't exists!\n");
            free(ddm_input);
            ddm_input = NULL;
        }else{
            printf("\nSome error to open directory!\n");
            free(ddm_input);
            ddm_input = NULL;
        }
    }else
        ddm_input = NULL;

    if ((init_bit_matrix(&(ddm_input->result), ddm_input->updates, ddm_input->subscriptions, one)) != err_none){
        free(ddm_input->list_subscriptions);
        free(ddm_input->list_updates);
        ddm_input = NULL;
    }

    return ddm_input;
}

int DDM_Is_Alfa_Test(DDM_Input ddm_input){

  if ((strncmp(DDM_Get_Test_Type(ddm_input), "alfa", 4)) == 0)
    return 1;
  return 0;
}

char* DDM_Get_Test_Type(DDM_Input ddm_input){
  char *tmp = (char *) malloc(sizeof(char) * 100);
  if (ddm_input.type_test != NULL){
    sprintf(tmp, "%s", ddm_input.type_test);
    return tmp;
  }
  return NULL;
}

size_t DDM_Get_Extents(DDM_Input ddm_input){

  if (ddm_input.extents != 0)
    return ddm_input.extents;
  return -1;
}

size_t DDM_Get_Dimensions(DDM_Input ddm_input){

  if (ddm_input.dimensions != 0)
    return ddm_input.dimensions;
  return -1;
}

float DDM_Get_Alfa(DDM_Input ddm_input){

  if (ddm_input.alfa != 0)
    return ddm_input.alfa;
  return -1;
}

size_t DDM_Get_Updates(DDM_Input ddm_input){

  if (ddm_input.updates != 0)
    return ddm_input.updates;
  return -1;
}

size_t DDM_Get_Subscriptions(DDM_Input ddm_input){

  if (ddm_input.subscriptions != 0)
    return ddm_input.subscriptions;
  return -1;
}

DDM_Extent* DDM_Get_Updates_List(DDM_Input ddm_input){
  if (ddm_input.list_updates != NULL)
    return ddm_input.list_updates;
  return NULL;
}

DDM_Extent* DDM_Get_Subscriptions_List(DDM_Input ddm_input){
  if (ddm_input.list_subscriptions != NULL)
    return ddm_input.list_subscriptions;
  return NULL;
}

void DDM_Set_Bit_Mat_Result(DDM_Input *ddm_input, const uint64_t update_row, const uint64_t subscription_column){
    set_bit_mat(ddm_input->result, update_row, subscription_column);
}

void DDM_AND_Op_With_Bitmatrix(DDM_Input *ddm_input, const bitmatrix mask){
    uint64_t i;

    for (i = 0; i < (uint64_t)ddm_input->updates; ++i)
        vector_bitwise_and(ddm_input->result[i], mask[i], (uint64_t)ddm_input->subscriptions);
}

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

void DDM_Write_Result(DDM_Input ddm_input){
  /* write results */
  char str[100];
  strcpy(str, ddm_input.executable_name);
  strcat(str, ".txt");
  FILE* fout = fopen(str, "a");
  if ( fout == NULL ) {
      printf("Error creating file %s\n", str);
      exit(-1);
  }

  fprintf(fout, "%f\n", ddm_input.timer.total);
  fclose(fout);
}

uint64_t DDM_Count_Matches(DDM_Input *ddm_input, const uint64_t updates, const uint64_t subscriptions){
    return count_ones(ddm_input->result, updates, subscriptions);
}

/******************************************
 ***************** TIMER ******************
 ******************************************/

void DDM_Start_Timer(DDM_Input *ddm_input){
    ddm_input->timer.start = clock();
    ddm_input->timer.total = 0;
}

void DDM_Stop_Timer(DDM_Input *ddm_input){
    ddm_input->timer.end = clock();
    ddm_input->timer.total = ((float)(ddm_input->timer.end - ddm_input->timer.start)) / CLOCKS_PER_SEC;
}

float DDM_Get_Total_Time(DDM_Input ddm_input){
  return ddm_input.timer.total;
}

/******************************************
 ***************** UTILS ******************
 ******************************************/

void print_list_updates_and_subscriptions(DDM_Input ddm_input){
  char test[1000];
  size_t i, j;

  printf("subscriptions\n");
  for (i = 0; i < ddm_input.subscriptions; ++i){
      sprintf(test, "%zu ", ddm_input.list_subscriptions[i].id);
      for (j = 0; j < ddm_input.dimensions; ++j){
	  sprintf(test, "%s %lf %lf ", test, ddm_input.list_subscriptions[i].lower[j], ddm_input.list_subscriptions[i].upper[j]);
      }
      printf("%s\n", test);
  }
  printf("\nupdates\n");
  for (i = 0; i < ddm_input.updates; ++i){
      sprintf(test, "%zu ", ddm_input.list_updates[i].id);
      for (j = 0; j < ddm_input.dimensions; ++j){
	  sprintf(test, "%s %lf %lf ", test, ddm_input.list_updates[i].lower[j], ddm_input.list_updates[i].upper[j]);
      }
      printf("%s\n", test);
  }
}

/******************************************
 **************** LOG FILE ****************
 ******************************************/

void WriteLogLine(char * string)
{
    FILE * fp;
    time_t now;
    fp = fopen(LOG_FILENAME,"a+"); //Opens output file in append mode.
	if(fp == NULL)
	{
        fprintf(stderr,"*** Error ***\nCannot append on log file file %s.\n", LOG_FILENAME);
        exit(EXIT_FAILURE);
	}
	time(&now);
	fprintf(fp,"%s: ", ctime(&now));
	fprintf(fp, "%s", string);
	fprintf(fp, "\n----------------------------\n");
    fclose(fp);
}

/******************************************
 ***************** ERROR ******************
 ******************************************/

/**
* \brief File containing the error handling functions.
* This file is derived from error.c included in SortMatching by Marco Mandrioli (licensed under GNU Affero General Public License)
*/


/**
* \brief Array of error strings.
* \remarks The indices must be matched with the ERR_CODE values.
*/
static const char *err_strings[] =
{
	"No error",
	"An unhandled error occurred",
	"A generic error occurred",
	"An error occurred while allocating memory",
	"An error occurred while creating/opening a file",
	"Bad input",
	"The problem cant have more than the max number of dimensions (see define MAX_DIMENSIONS in defines.h)",
	"An error occurred while handling threads"
};


/** \brief The global error variable.
*/
static _err_t ERR_VAR;


/** \brief Sets the global variable and its info to the actual error.

\param code the code of the error
\param file string containing the name of the file in which the error was generated
\param function string containing the name of the function in which the error was generated
\param file the number of the line in which the error was generated

\retval error code
*/
_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const uint32_t line)
{
	if (code != err_none)
	{
		ERR_VAR.code = code;
		if (sprintf(ERR_VAR.file, "%s", file) < 0)
			return err_unhandled;
		if (sprintf(ERR_VAR.function, "%s", function) < 0)
			return err_unhandled;
		ERR_VAR.line = line;
	}

	return code;
}


/**
* \brief Prints the error message.
* \retval error code
*/
_ERR_CODE print_error_string()
{
	if (ERR_VAR.code == err_none || ERR_VAR.code == err_unhandled)
		printf("%s", err_strings[ERR_VAR.code]);
	else
	{
		fprintf(stderr, "\n\n******************** ERROR **********************\n");
		fprintf(stderr, "%s:%d: in function '%s': error %d: %s\n", ERR_VAR.file, ERR_VAR.line, ERR_VAR.function, ERR_VAR.code, err_strings[ERR_VAR.code]);
		exit(EXIT_FAILURE);
	}
	return ERR_VAR.code;
}

/**
 * \brief Prints an error message on stderr
 * This method prints an error string on stderr.
 * \param string the error string to print out
 * \param force_exit a boolean value. If it's true the exectuion is halted immediately after printing the message, else the execution continues.
 */
void print_runtime_error_string(char * string, bool force_exit)
{
    char buffer[BUFFER_SIZE + 14];
	fprintf(stderr, "\n\n******************** ERROR **********************\n");
	fprintf(stderr,"%s\n", string);
	if(force_exit)
    {
        strcpy(buffer, "FATAL ERROR: ");
        strcat(buffer, string);
        WriteLogLine(buffer);
        exit(EXIT_FAILURE);
    }
    else
    {
        strcpy(buffer, "WARNING: ");
        strcat(buffer,string);
        WriteLogLine(buffer);
    }
}

/******************************************
 *************** BITVECTOR ****************
 ******************************************/

bitvector init_bitvector(uint64_t size)
{
    bitvector vector = (bitvector) calloc(sizeof(BITVECTOR_TYPE), BIT_VEC_WIDTH(size));
    return vector;
}

void free_vector(bitvector *vector)
{
    free(*vector);
    *vector=NULL;
}

void reset_vector(bitvector vector, const uint64_t size)
{
    memset(vector, 0x00, BIT_VEC_WIDTH(size) * sizeof(BITVECTOR_TYPE));
}

void set_vector(bitvector vector, const uint64_t size)
{
    memset(vector, 0xff, BIT_VEC_WIDTH(size) * sizeof(BITVECTOR_TYPE));
}

void negate_vector(bitvector vector, const uint64_t size)
{
    uint64_t i;
    for(i=0; i<BIT_VEC_WIDTH(size); i++)
    {
        vector[i] = ~vector[i];
    }
}

void set_element(bitvector vector, const uint64_t pos)
{
    BIT_SET(vector[BIT_TO_POS(pos)],DBIT(BIT_POS_IN_VEC(pos,BIT_TO_POS(pos))));
}

void reset_element(bitvector vector, const uint64_t pos)
{
    BIT_CLEAR(vector[BIT_TO_POS(pos)],DBIT(BIT_POS_IN_VEC(pos,BIT_TO_POS(pos))));
}

void negate_element(bitvector vector, const uint64_t pos)
{
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(pos);
    BIT_FLIP(vector[bit_pos], DBIT(BIT_POS_IN_VEC(pos,bit_pos)));
}

void copy_vector(bitvector vdest, bitvector vsource, const uint64_t size)
{
    memccpy(vdest, vsource, BIT_VEC_WIDTH(size), sizeof(BITVECTOR_TYPE));
}

bool isSet(bitvector vector, const uint64_t pos)
{
    return BIT_DCHECK(vector[BIT_TO_POS(pos)], pos) != 0;
}

void print_bitvector(bitvector vector, const uint64_t size)
{
    uint64_t j, bit; //TODO tipo giusto?
	uint8_t element[BIT_LENGTH + 1];
	BITVECTOR_TYPE val;
	BITVECTOR_TYPE mask;
    printf("\n________BITVECTOR _________\n");
	uint64_t line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(size);

	// escape the string
	element[BIT_LENGTH] = '\0';
		// for each element in the line
		for (j = 0; j < line_width; j++)
		{
			mask = BITVEC_ELEM_MAX_BIT;
			val = vector[j];

			// for each bit in the element
			for (bit = 0; bit < BIT_LENGTH; bit++)
			{
				// if the actual bit is TRUE, set the char to '1', else set it to '0'
				element[bit] = (val & mask ? '1' : '0');
				// shift right
				mask >>= 1;
			}

			// print the string
			printf("%s ", element);
		}
}

/** \brief Bitwise AND of two bit vectors.

It can also be used to do the bitwise AND of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_and(const bitvector result, const bitvector mask, const uint64_t size)
{
	uint64_t i;

	for (i = 0; i < size; i++){
		result[i] &= mask[i];
	}
}

/** \brief Bitwise OR of two bit vectors.

It can also be used to do the bitwise OR of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_or(const bitvector result, const bitvector mask, const uint64_t size)
{
	uint64_t i;

	for (i = 0; i < size; i++)
		result[i] |= mask[i];
}

/******************************************
 *************** BITMATRIX ****************
 ******************************************/

_ERR_CODE init_bit_matrix (bitmatrix * matrix_pointer,const uint64_t rows, const uint64_t columns, const InitialValue initial_value){
    uint64_t line_width, i;
    bitvector vec;
    *matrix_pointer = (bitmatrix ) malloc(rows * sizeof(bitvector));

    line_width = BIT_VEC_WIDTH(columns);

    if(initial_value == zero)
        vec = (bitvector)calloc(line_width * rows, sizeof(BITVECTOR_TYPE));
    else
        vec = (bitvector)malloc(line_width * rows * sizeof(BITVECTOR_TYPE));
    if(vec == NULL || *matrix_pointer == NULL)
    {
         return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);
    }

    for (i = 0; i < rows; i++)
        (*matrix_pointer)[i] = &vec[i * line_width];
    if(initial_value == one)
    {
        set_whole_bit_mat(*matrix_pointer, rows, columns);
    }

	return err_none;
}

void set_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
    BIT_SET(mat[row][bit_pos],DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}
void reset_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
    BIT_CLEAR(mat[row][bit_pos],DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}

void negate_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column){
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(column);
	BIT_FLIP(mat[row][bit_pos], DBIT(BIT_POS_IN_VEC(column,bit_pos)));
}

void reset_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    memset(*mat, UINT8_C(0x00), rows* BIT_VEC_WIDTH(columns) * sizeof(BITVECTOR_TYPE));
}

void set_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    memset(*mat, UINT8_C(0xFF), rows* BIT_VEC_WIDTH(columns) * sizeof(BITVECTOR_TYPE));
}

void negate_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
	uint64_t i, j;
	for(i=0; i<rows; i++)
	{
	    for(j=0; j < BIT_VEC_WIDTH(columns); j++)
        {
           BIT_FLIP(mat[i][j], FULL_MASK);
        }
	}
}

bool get_bit(const bitmatrix mat, const uint64_t row,const uint64_t col)
{
    return BIT_DCHECK(mat[row][BIT_TO_POS(col)], col) != 0;
}

_ERR_CODE read_bit_matrix (bitmatrix * mat, const char * filename, uint64_t * rows, uint64_t * columns)
{
	FILE * fp;
    uint64_t element_number = 0;
	BITVECTOR_TYPE i;
	fp = fopen(filename, "rb");
	if(fp != NULL)
	{
		if(!fread(rows,sizeof(uint64_t), 1, fp))
			return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
		if(!fread(columns,sizeof(uint64_t), 1, fp))
			return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
		element_number = BIT_VEC_WIDTH(*columns);
		if (init_bit_matrix(mat, *rows, *columns, uninitialized) != err_none)
			return print_error_string();
		for(i=0; i<*rows; i++)
		{
			if(fread((*mat)[i],sizeof(BITVECTOR_TYPE), element_number, fp) != element_number)
			{
            printf("DURANTE RIGA %"PRIu64" ", i);
		    fflush(stdout);
				perror("fread");
				if(feof(fp))
				return set_error ( err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
			}
		}
		fclose(fp);
	}
	else
	{
		return set_error(err_file, __FILE__, __FUNCTION__, __LINE__);
	}
	return err_none;
}

uint64_t count_ones(const bitmatrix mat, const uint64_t rows, const uint64_t columns)
{
    uint64_t i, j;
	uint_fast8_t count;
	BITVECTOR_TYPE res;
	uint64_t subtotal;
	uint32_t full_columns = BIT_VEC_WIDTH(columns)-1;
	subtotal = 0;
	BITVECTOR_TYPE mask = FULL_MASK << (BIT_LENGTH - (columns % BIT_LENGTH));
	//Compute hamming distance for each row except from the last one, which may not be completely occupied by meaningful data.
	for(i=0; i< rows; i++)
	{
	    for(j=0; j<full_columns; j++)
        {
            //Xor between the two values
            res =  mat[i][j];
            //Compute hamming distance for this row
            for (count = 0; res; count++)
                res &= res-1;
		subtotal += count;
        }
        //compute last cell to the right, masking the extra bits
        res = (mat[i][j]) & mask;
        for (count = 0; res; count++)
            res &= res-1;
		subtotal += count;
	}
	return subtotal;
}

void print_bit_matrix(const bitmatrix mat, const char * filename, const bool override_file, const uint64_t rows, const uint64_t columns)
{
	FILE * fp;
	BITVECTOR_TYPE i;
	uint64_t element_numb = BIT_VEC_WIDTH(columns);
	char confirm='1';
	if(!override_file){
		fp = fopen(filename,"rb");
		if(fp != NULL)
		{
			fclose(fp);
			printf("File %s already exists, would you like to overwrite it? (y|n)\n", filename);
			do{
				confirm = getchar();
			}while(confirm!='y' && confirm !='n');
			if(confirm == 'n')
				return;
		}
	}
	if((fp = fopen(filename, "wb")) != NULL)
	{
	    //Print the number of rows
		fwrite(&rows, sizeof(uint64_t), 1, fp);
		//Print the number of columns
		fwrite(&columns, sizeof(uint64_t), 1, fp);

		//Prints vector as bytes
		for(i=0; i< rows; i++)
			if(fwrite(mat[i],sizeof(BITVECTOR_TYPE),element_numb, fp) != element_numb)
			{
				set_error(err_generic,__FILE__,__FUNCTION__,__LINE__);
			}
		fclose(fp);
	}
	return;
}

bool checkSet (const bitmatrix mat1, const bitmatrix mat2, const int rows, const int columns)
{
	uint64_t i, j;
	for(i=0; i < rows; i++)
	{
	    for(j=0; j<BIT_VEC_WIDTH(columns); j++)
		if((mat1[i][j] & mat2[i][j]) != mat1[i][j])
			return false;
	}
	return true;
}

int compareBitmatix(const bitmatrix mat1, const bitmatrix mat2, const uint64_t rows, const uint64_t columns)
{
	uint64_t i, j;
	int count;
	BITVECTOR_TYPE res;
	int subtotal;
	uint32_t full_columns = BIT_VEC_WIDTH(columns)-1;
	subtotal=0;
	BITVECTOR_TYPE mask = FULL_MASK << (BIT_LENGTH - (columns % BIT_LENGTH));
	//Compute hamming distance for each row except from the last one, which may not be completely occupied by meaningful data.
	for(i=0; i< rows; i++)
	{
	    for(j=0; j<full_columns; j++)
        {
            //Xor between the two values
            res =  mat1[i][j] ^ mat2[i][j];
            //Compute hamming distance for this row
            for (count = 0; res; count++)
                res &= res-1;
		subtotal += count;
        }
        //compute last cell to the right, masking the extra bits
        res = (mat1[i][j] ^ mat2[i][j]) & mask;
        for (count = 0; res; count++)
            res &= res-1;
		subtotal += count;
	}
	return subtotal;
}

void free_bit_matrix(bitmatrix * mat){
	free(*mat);
}

void print_readable_bitmatrix(const bitmatrix in, int iteration, int rows, int columns)
{
    uint64_t i, j, bit; //TODO tipo giusto?
	uint8_t element[BIT_LENGTH + 1];
	BITVECTOR_TYPE val;
	BITVECTOR_TYPE mask;
    printf("\n________BITMATRIX %d _________\n", iteration);
	uint64_t line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(columns);

	// escape the string
	element[BIT_LENGTH] = '\0';

	// for each line (update extent)
	for (i = 0; i < rows; i++)
	{
		// for each element in the line
		for (j = 0; j < line_width; j++)
		{
			mask = BITVEC_ELEM_MAX_BIT;
			val = in[i][j];

			// for each bit in the element
			for (bit = 0; bit < BIT_LENGTH; bit++)
			{
				// if the actual bit is TRUE, set the char to '1', else set it to '0'
				element[bit] = (val & mask ? '1' : '0');
				// shift right
				mask >>= 1;
			}

			// print the string
			printf("%s ", element);
		}
		// new line
		printf("\n");
	}
}
