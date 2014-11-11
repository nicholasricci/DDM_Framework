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
        sprintf(ddm_input->executable_name, "%s", argv[0]);
        sprintf(ddm_input->type_test, "%s", argv[1]);
        if (strncmp(argv[1], "alfa", 4) == 0){
            ddm_input->extents = atoi(argv[2]);
            ddm_input->dimensions = atoi(argv[3]);
            ddm_input->alfa = atof(argv[4]);
            ddm_input->updates = (size_t) (ddm_input->extents / 2);
            ddm_input->subscriptions = (size_t) (ddm_input->extents / 2);
            sprintf(alfa_name, "ALFA_%s_%"PRIu64"_%d", argv[4], ddm_input->extents, ddm_input->dimensions);
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
                fgets(line, LINE_MAX_LENGTH, file_input);
                for (i = 0; i < ddm_input->subscriptions; ++i){
                    fgets(line, LINE_MAX_LENGTH, file_input);
                    sscanf(line, "%"PRIu64"%n", &(ddm_input->list_subscriptions[i].id), &nchar);
                    addr_line = line + nchar;
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sscanf(addr_line, " %"PRId64" %"PRId64"%n", &(ddm_input->list_subscriptions[i].lower[j]), &(ddm_input->list_subscriptions[i].upper[j]), &nchar);
                        addr_line += nchar;
                    }
                    //printf("%zu\n", ddm_input->list_subscriptions[i].id);
                }

                fgets(line, LINE_MAX_LENGTH, file_input);
                for (i = 0; i < ddm_input->updates; ++i){
                    fgets(line, LINE_MAX_LENGTH, file_input);
                    sscanf(line, "%"PRIu64"%n", &(ddm_input->list_updates[i].id), &nchar);
                    addr_line = line + nchar;
                    for (j = 0; j < ddm_input->dimensions; ++j){
                        sscanf(addr_line, " %"PRId64" %"PRId64"%n", &(ddm_input->list_updates[i].lower[j]), &(ddm_input->list_updates[i].upper[j]), &nchar);
                        addr_line += nchar;
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

    if (ddm_input != NULL){
      ddm_input->result_mat = create_result_matrix(ddm_input->updates, ddm_input->subscriptions);
      if (ddm_input->result_mat == NULL)
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

uint64_t DDM_Get_Extents(DDM_Input ddm_input){

    if (ddm_input.extents != 0)
        return ddm_input.extents;
    return -1;
}

uint16_t DDM_Get_Dimensions(DDM_Input ddm_input){

    if (ddm_input.dimensions != 0)
        return ddm_input.dimensions;
    return -1;
}

float DDM_Get_Alfa(DDM_Input ddm_input){

    if (ddm_input.alfa != 0)
        return ddm_input.alfa;
    return -1;
}

uint64_t DDM_Get_Updates(DDM_Input ddm_input){

    if (ddm_input.updates != 0)
        return ddm_input.updates;
    return -1;
}

uint64_t DDM_Get_Subscriptions(DDM_Input ddm_input){

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

void DDM_Set_Matrix_Update_Subscription(DDM_Input *ddm_input, uint64_t update, uint64_t subscription){
    ddm_input->result_mat[update][subscription] = 1;
}

void DDM_And_Operation_Between_Matrices(DDM_Input *ddm_input, uint_fast8_t **mask, uint64_t updates, uint64_t subscriptions){
    and_operation_between_matrices(ddm_input->result_mat, mask, updates, subscriptions);
}

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

void DDM_Write_Result(DDM_Input ddm_input){
    uint64_t i, j;

    /* write results */
    char str[100], strresmat[100];
    strcpy(str, ddm_input.executable_name);
    strcat(str, ".txt");
    FILE* fout = fopen(str, "a");
    if ( fout == NULL ) {
        printf("Error creating file %s\n", str);
        exit(-1);
    }

    fprintf(fout, "%f\n", ddm_input.timer.total);
    fclose(fout);

    strcpy(strresmat, "result_mat.txt");
    fout = fopen(strresmat, "w+");
    if (fout == NULL){
        printf("Error creating file %s\n", strresmat);
        exit(-1);
    }

    //write result matrix
    for (i = 0; i < ddm_input.updates; ++i){
        for (j = 0; j < ddm_input.subscriptions; ++j){
            fprintf(fout, "%d ", ddm_input.result_mat[i][j]);
        }
        fprintf(fout, "\n");
    }

    fclose(fout);
}

uint64_t DDM_Count_Matches(DDM_Input *ddm_input){
    return count_ones_matrix(ddm_input->result_mat, ddm_input->updates, ddm_input->subscriptions);
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
      sprintf(test, "%"PRIu64"", ddm_input.list_subscriptions[i].id);
      for (j = 0; j < ddm_input.dimensions; ++j){
	  sprintf(test, "%s %"PRId64" %"PRId64"", test, ddm_input.list_subscriptions[i].lower[j], ddm_input.list_subscriptions[i].upper[j]);
      }
      printf("%s\n", test);
  }
  printf("\nupdates\n");
  for (i = 0; i < ddm_input.updates; ++i){
      sprintf(test, "%"PRIu64"", ddm_input.list_updates[i].id);
      for (j = 0; j < ddm_input.dimensions; ++j){
	  sprintf(test, "%s %"PRId64" %"PRId64"", test, ddm_input.list_updates[i].lower[j], ddm_input.list_updates[i].upper[j]);
      }
      printf("%s\n", test);
  }
}

/******************************************
 ************* RESULT MATRIX **************
 ******************************************/

uint_fast8_t** create_result_matrix(uint64_t updates, uint64_t subscriptions){
    uint64_t i;
    uint_fast8_t **result_mat;

    result_mat = (uint_fast8_t **) malloc(sizeof(uint_fast8_t *) * updates);
    if (result_mat == NULL)
        return NULL;
    for (i = 0; i < updates; ++i){
        result_mat[i] = (uint_fast8_t *) malloc(sizeof(uint_fast8_t) * subscriptions);
        if (result_mat[i] == NULL)
            return NULL;
    }

    return result_mat;
}

void reset_mat(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions, mat_value value){
    uint64_t i, j;
    uint_fast8_t val = (value == zero) ? 0 : 1;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            mat[i][j] = val;
        }
    }
}

void and_operation_between_matrices(uint_fast8_t **mat, uint_fast8_t **mask, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            mat[i][j] *= mask[i][j];
        }
    }
}

void set_value_mat(uint_fast8_t **mat, uint64_t update, uint64_t subscription, mat_value value){

    mat[update][subscription] = (value == zero) ? 0 : 1;
}

void print_matrix(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

uint64_t count_ones_matrix(uint_fast8_t **mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;
    uint64_t count = 0;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            if (mat[i][j] == 1) count++;
        }
    }

    return count;
}

/******************************************
 *************** BIT MATRIX ***************
 ******************************************/

void bitmatrix_init(bitmatrix *mat, uint64_t updates, uint64_t subscriptions)
{
    mat->updates = updates;
    mat->subscriptions = (subscriptions + BITS_PER_WORD - 1) / BITS_PER_WORD; /* round the number m of columns to the next multiple of BITS_PER_WORD */
    mat->data = (uint32_t*)calloc(mat->subscriptions * mat->updates, sizeof(uint32_t));
    if (mat->data == NULL){
        printf("\nError on bitmatrix allocation\n");
        exit(-1);
    }
}

void bitmatrix_free(bitmatrix *mat )
{
    mat->subscriptions = mat->updates = 0;
    free( mat->data );
}

void bitmatrix_set(bitmatrix *mat, uint64_t i, uint64_t j, int val)
{
    assert( i < mat->updates );
    assert( j < mat->subscriptions * BITS_PER_WORD );
    uint32_t bitval = 0x8000000 >> (j % BITS_PER_WORD);
    bitval *= (val != 0);
    uint32_t idx = i * mat->subscriptions + j / BITS_PER_WORD;
    mat->data[idx] = ( mat->data[idx] & ~bitval ) | bitval;
}

int bitmatrix_get(const bitmatrix *mat, uint64_t i, uint64_t j)
{
    assert( i < mat->updates );
    assert( j < mat->subscriptions* BITS_PER_WORD );
    uint32_t mask = 0x8000000 >> (j % BITS_PER_WORD);
    uint32_t idx = i * mat->subscriptions + j / BITS_PER_WORD;

    return ( ( mat->data[idx] & mask ) > 0 );
}

void bitmatrix_and(bitmatrix *mat, const bitmatrix *mask){
    uint64_t i, j;

    for (i = 0; i < mat->updates; ++i)
        for (j = 0; j < mat->subscriptions; ++j)
            bitmatrix_set(mat, i, j, (bitmatrix_get(mat, i, j) & bitmatrix_get(mask, i, j)));
}

uint64_t bitmatrix_count_ones(const bitmatrix *mat){
    uint64_t i, j;
    uint64_t count = 0;

    for (i = 0; i < mat->updates; ++i)
        for (j = 0; j < mat->subscriptions; ++j)
            if (bitmatrix_get(mat, i, j))
                count++;

    return count;
}
