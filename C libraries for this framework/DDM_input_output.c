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
        bitmatrix_init(&ddm_input->result_mat, ddm_input->updates, ddm_input->subscriptions);
    }else
        ddm_input = NULL;

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

void DDM_Dispose_Input(DDM_Input *ddm_input){

    free(ddm_input->list_updates);
    free(ddm_input->list_subscriptions);
    bitmatrix_free(&ddm_input->result_mat, ddm_input->updates, ddm_input->subscriptions);
    free(ddm_input);
}

/******************************************
 ***************** OUTPUT *****************
 ******************************************/

void DDM_Write_Result(DDM_Input *ddm_input){

    /* write results */
    char str[1000];
    strcpy(str, ddm_input->executable_name);
    strcat(str, ".txt");
    FILE* fout = fopen(str, "a");
    if ( fout == NULL ) {
        printf("Error creating file %s\n", str);
        exit(-1);
    }

    fprintf(fout, "%f\n", ddm_input->timer.total);
    //fclose(fout);

    bitmatrix_write_file(ddm_input->result_mat, ddm_input->updates, ddm_input->subscriptions,"result_mat.bin");
    DDM_Dispose_Input(ddm_input);

    /*strcpy(strresmat, "result_mat.txt");
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

    fclose(fout);*/
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

void bitmatrix_init(bitmatrix *mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i;

    *mat = (bitmatrix)malloc(sizeof * (*mat) * updates);
    for (i = 0; i < updates; ++i)
        (*mat)[i] = (bitvector)malloc(ceill(subscriptions / (float)BIT_NUMBER) * sizeof * (*mat)[i]);
}

void bitmatrix_set_value(bitmatrix mat, uint64_t update, uint64_t subscription, mat_value value){
    //position in bitvector that we have to set the value
    uint64_t bitpos_invec = subscription / BIT_NUMBER;
    //bit position in SPACE_TYPE we have to set
    bitelem bitpos =  subscription % BIT_NUMBER;
    bitelem mask = DBIT(bitpos);
    if (value == one){
        mat[update][bitpos_invec] |= mask;
    }else{
        mat[update][bitpos_invec] &= ~mask;
    }
}

int bitmatrix_get(const bitmatrix mat, uint64_t update, uint64_t subscription){
    uint64_t bitpos_invec = subscription / BIT_NUMBER;
    bitelem bitpos = subscription % BIT_NUMBER;
    bitelem mask = DBIT(bitpos);
    if (mat[update][bitpos_invec] & mask)
        return 1;
    else
        return 0;
}

void bitmatrix_and(bitmatrix mat, const bitmatrix mask, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < ceill(subscriptions / (float)BIT_NUMBER); ++j)
            mat[i][j] &= mask[i][j];
}

void bitmatrix_or(bitmatrix mat, const bitmatrix mask, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < ceill(subscriptions / (float)BIT_NUMBER); ++j)
            mat[i][j] |= mask[i][j];
}

void bitmatrix_not(bitmatrix mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < ceill(subscriptions / (float)BIT_NUMBER); ++j)
            mat[i][j] = ~mat[i][j];
}

void bitmatrix_xor(bitmatrix mat, const bitmatrix mask, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < ceill(subscriptions / (float)BIT_NUMBER); ++j)
            mat[i][j] ^= mask[i][j];
}

void bitmatrix_reset(bitmatrix mat, uint64_t updates, uint64_t subscriptions, mat_value value){
    uint64_t i;

    for (i = 0; i < updates; ++i)
        if (value == zero)
            /*for (j = 0; j < ceil(subscriptions / (float)BIT_NUMBER); ++j)
                mat[i][j] = BITS_ZERO;*/
            memset(mat[i], BITS_ZERO, sizeof(bitelem) * ceil(subscriptions / (float)BIT_NUMBER));
        else{
            /*for (j = 0; j < ceil(subscriptions / (float)BIT_NUMBER); ++j)
                mat[i][j] = BITS_ONE;*/
            memset(mat[i], BITS_ONE, sizeof(bitelem) * ceil(subscriptions / (float)BIT_NUMBER));
        }
}

void bitmatrix_print(bitmatrix mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j)
            printf("%d ", bitmatrix_get(mat, i, j));
        printf("\n");
    }
}

uint64_t bitmatrix_count_ones(bitmatrix mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j, count = 0;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            if (bitmatrix_get(mat, i, j))
                count++;

    return count;
}

uint64_t bitmatrix_differences(bitmatrix mat1, bitmatrix mat2, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j, count = 0;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            if (bitmatrix_get(mat1, i, j) != bitmatrix_get(mat2, i, j))
                count++;

    return count;
}

void bitmatrix_read_file(bitmatrix *mat, uint64_t updates, uint64_t subscriptions, char *filename){
    FILE *fp;
    uint64_t i;
    uint64_t subs_vec = ceil(subscriptions / (float)BIT_NUMBER);

    bitmatrix_init(mat, updates, subscriptions);

    if ((fp = fopen(filename, "rb")) == NULL){
        printf("\nError to open file to read\n");
        exit(-1);
    }

    for (i = 0; i < updates; ++i){
        fread((*mat)[i], sizeof * (*mat)[i], subs_vec, fp);
    }
    fclose(fp);
}

void bitmatrix_write_file(const bitmatrix mat, uint64_t updates, uint64_t subscriptions, char *filename){
    FILE *fp;
    uint64_t i;
    uint64_t subs_vec = ceil(subscriptions / (float)BIT_NUMBER);

    if ((fp = fopen(filename, "wb")) == NULL){
        printf("\nError to open file to write\n");
        exit(-1);
    }

    for (i = 0; i < updates; ++i){
        fwrite(mat[i], sizeof * mat[i], subs_vec, fp);
    }
    fclose(fp);
}

void bitmatrix_free(bitmatrix *mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i;
    bitvector vec;

    for (i = 0; i < updates; ++i){
        vec = (*mat)[i];
        free(vec);
    }
    free(*mat);
}

void bitmatrix_print_matches(const bitmatrix mat, uint64_t updates, uint64_t subscriptions){
    uint64_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            if (bitmatrix_get(mat, i, j))
                printf("(U%"PRIu64", S%"PRIu64"), \n", i, j);
}
