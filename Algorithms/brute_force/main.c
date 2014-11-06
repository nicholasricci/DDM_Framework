#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

const float Lmax = 1.0e6; /* dimension length */

int count_ones_matrix(uint_fast8_t **result, size_t updates, size_t subscriptions){
    int count = 0;
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            if (result[i][j] == 1)
                count++;

    return count;
}

void reset_matrix_zero(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            result[i][j] = 0;
}

void reset_matrix_one(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            result[i][j] = 1;
}

void print_matrix(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j)
            printf("%d ", result[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    size_t dimensions;
    size_t updates;
    size_t subscriptions;
    DDM_Extent *list_updates;
    DDM_Extent *list_subscriptions;
    DDM_Input *ddm_input;
    /*Index variables*/
    size_t i, j, k;
    /*Result variable*/
    size_t nmatch;
    int isValid;
    
    uint_fast8_t **matrix, **temp;

    ddm_input = DDM_Initialize_Input(argc, argv);
    dimensions = DDM_Get_Dimensions(*ddm_input);
    updates = DDM_Get_Updates(*ddm_input);
    subscriptions = DDM_Get_Subscriptions(*ddm_input);
    list_updates = DDM_Get_Updates_List(*ddm_input);
    list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);

    //print_list_updates_and_subscriptions(*ddm_input);
    //TEST
    matrix = (uint_fast8_t **) malloc(sizeof(uint_fast8_t *) * updates);
    temp = (uint_fast8_t **) malloc(sizeof(uint_fast8_t *) * updates);
    for (i = 0; i < updates; ++i){
        matrix[i] = (uint_fast8_t *) malloc(sizeof(uint_fast8_t) * subscriptions);
        temp[i] = (uint_fast8_t *) malloc(sizeof(uint_fast8_t) * subscriptions);
    }

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            matrix[i][j] = 1;
        }
    }
    
    nmatch = 0;
    DDM_Start_Timer(ddm_input);

    //Execute algorithm
    /*for (i = 0; i < updates; ++i){
    #pragma omp parallel for private(j, isValid, k) reduction(+:nmatch)
        for (j = 0; j < subscriptions; ++j){
            isValid = 1;
            for (k = 0; k < dimensions; ++k){
                if (
                  (list_updates[i].lower[k] <= list_subscriptions[j].lower[k]
                  && list_subscriptions[j].lower[k] <= list_updates[i].upper[k])
                  ||
                  (list_updates[i].lower[k] <= list_subscriptions[j].upper[k]
                  && list_subscriptions[j].upper[k] <= list_updates[i].upper[k])
                  ||
                  (list_subscriptions[j].lower[k] <= list_updates[i].lower[k]
                  && list_updates[i].lower[k] <= list_subscriptions[j].upper[k])
                  ||
                  (list_subscriptions[j].lower[k] <= list_updates[i].upper[k]
                  && list_updates[i].upper[k] <= list_subscriptions[j].upper[k])
                  ){
                  continue;
                }else{
                  isValid = 0;
                }
            }
            if (isValid){
                DDM_Set_Bit_Mat_Result(ddm_input, i, j);
                //printf("\nU%zu, S%zu\n", list_updates[i].id, list_subscriptions[j].id);
            }
        }
    }*/
    
    
    for (k = 0; k < dimensions; ++k){
      reset_matrix_zero(temp, updates, subscriptions);
      for (i = 0; i < updates; ++i){
	for (j = 0; j < subscriptions; ++j){
	  if (
	    (list_updates[i].lower[k] <= list_subscriptions[j].lower[k]
	    && list_subscriptions[j].lower[k] <= list_updates[i].upper[k])
	    ||
	    (list_updates[i].lower[k] <= list_subscriptions[j].upper[k]
	    && list_subscriptions[j].upper[k] <= list_updates[i].upper[k])
	    ||
	    (list_subscriptions[j].lower[k] <= list_updates[i].lower[k]
	    && list_updates[i].lower[k] <= list_subscriptions[j].upper[k])
	    ||
	    (list_subscriptions[j].lower[k] <= list_updates[i].upper[k]
	    && list_updates[i].upper[k] <= list_subscriptions[j].upper[k])
	    ){
	    temp[i][j] = 1;
	  }
	}
      }
      for (i = 0; i < updates; ++i)
            for (j = 0; j < subscriptions; ++j)
                matrix[i][j] *= temp[i][j];
      printf("\n nmatches for dimension n.%zu: %zu\n", k, count_ones_matrix(temp, updates, subscriptions));
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", count_ones_matrix(matrix, updates, subscriptions));
    //nmatch = (size_t)DDM_Count_Matches(ddm_input, updates, subscriptions);
    //Write result on file
    DDM_Write_Result(*ddm_input);

    //write on screen nmatches and total time
    printf("\ntotal_time  %fs\n", DDM_Get_Total_Time(*ddm_input) );


    return 0;
}
