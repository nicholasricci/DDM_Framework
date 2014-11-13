/**
 * This file is a default main with DDM_input_output library included.
 * Something to tell to use this:
 * To set a match between an update and subscription use:
 * bitmatrix_set_value(mat, updates, subscriptions, value); //value can be one, for a match, or zero, for a non-match.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "DDM_input_output.h"
#include "lists.h"

typedef struct DDM_Extent_Partition{
    uint64_t id;
    int64_t lower[MAX_DIMENSIONS];
    int64_t upper[MAX_DIMENSIONS];
    uint8_t isUpdate;
}DDM_Extent_Partition;

//Global variable for lower overhead in recursion
DDM_Extent_Partition *list_extents;
uint16_t current_dimension;
uint64_t subscriptions;

void CheckMatch(bitmatrix result, list S, list U){
    list tempS, tempU;
    DDM_Extent_Partition es, eu;

    if (S != NULL && U != NULL){
        tempS = S;
        while (tempS != NULL){
            tempU = U;
            while (tempU != NULL){
                es = list_extents[tempS->id];
                eu = list_extents[tempU->id + subscriptions];
                if (
                    ( es.lower[current_dimension] <= eu.lower[current_dimension] && eu.lower[current_dimension] <= es.upper[current_dimension] )
                    ||
                    ( es.lower[current_dimension] <= eu.upper[current_dimension] && eu.upper[current_dimension] <= es.upper[current_dimension] )
                    ||
                    ( eu.lower[current_dimension] <= es.lower[current_dimension] && es.lower[current_dimension] <= eu.upper[current_dimension] )
                    ||
                    ( eu.lower[current_dimension] <= es.upper[current_dimension] && es.upper[current_dimension] <= eu.upper[current_dimension] )
                   )
                    bitmatrix_set_value(result, eu.id, es.id, one);

                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }
}

void Partition(bitmatrix result, DDM_Extent_Partition *extents, uint64_t extents_size, double min_lower, double max_upper){
    double pivot = min_lower + ((max_upper - min_lower) / 2.0);
    list Sl, Ul, Sp, Up, Sr, Ur;
    uint64_t Csl, Cul, Csr, Cur;
    uint64_t i;
    list_element *tempS, *tempU;
    DDM_Extent_Partition es, eu;
    DDM_Extent_Partition *left_extents, *right_extents;

    if (extents_size == 1)
        return;

    //init count of elements in each list
    Csl = Cul = Csr = Cur = 0;

    //init all lists
    list_init(&Sl);
    list_init(&Ul);
    list_init(&Sp);
    list_init(&Up);
    list_init(&Sr);
    list_init(&Ur);

    //fill list with extent position in list_extents
    for (i = 0; i < extents_size; ++i){
        es = extents[i];
        if (es.isUpdate){
            if (es.upper[current_dimension] < pivot){
                list_add_by_id(&Ul, es.id);
                Cul++;
            }else if (es.lower[current_dimension] > pivot){
                list_add_by_id(&Ur, es.id);
                Cur++;
            }else{
                list_add_by_id(&Up, es.id);
            }
        }else{
            if (es.upper[current_dimension] < pivot){
                list_add_by_id(&Sl, es.id);
                Csl++;
            }else if (es.lower[current_dimension] > pivot){
                list_add_by_id(&Sr, es.id);
                Csr++;
            }else{
                list_add_by_id(&Sp, es.id);
            }
        }
    }

    //Check if Sp and Up are empty or not
    //if they aren't empty then is a match
    if (Sp != NULL && Up != NULL){
        tempS = Sp;
        while (tempS != NULL){
            tempU = Up;
            while (tempU != NULL){
                es = list_extents[tempS->id];
                eu = list_extents[tempU->id + subscriptions];
                if (
                    ( es.lower[current_dimension] <= eu.lower[current_dimension] && eu.lower[current_dimension] <= es.upper[current_dimension] )
                    ||
                    ( es.lower[current_dimension] <= eu.upper[current_dimension] && eu.upper[current_dimension] <= es.upper[current_dimension] )
                    ||
                    ( eu.lower[current_dimension] <= es.lower[current_dimension] && es.lower[current_dimension] <= eu.upper[current_dimension] )
                    ||
                    ( eu.lower[current_dimension] <= es.upper[current_dimension] && es.upper[current_dimension] <= eu.upper[current_dimension] )
                   )
                    bitmatrix_set_value(result, eu.id, es.id, one);

                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }

    //Check Sp and Up with left and right lists
    CheckMatch(result, Sp, Ul);
    CheckMatch(result, Sp, Ur);
    CheckMatch(result, Sl, Up);
    CheckMatch(result, Sr, Up);

    //fill new two vector left_index_positions and right_index_positions
    left_extents = (DDM_Extent_Partition *) malloc(sizeof(DDM_Extent_Partition) * (Csl + Cul));
    right_extents = (DDM_Extent_Partition *) malloc(sizeof(DDM_Extent_Partition) * (Csr + Cur));

    tempS = Sl;
    for (i = 0; i < Csl; ++i){
        left_extents[i].id = tempS->id;
        left_extents[i].isUpdate = 0;
        left_extents[i].lower[current_dimension] = list_extents[tempS->id].lower[current_dimension];
        left_extents[i].upper[current_dimension] = list_extents[tempS->id].upper[current_dimension];
        tempS = tempS->next;
    }
    tempU = Ul;
    for (i = Csl; i < Csl + Cul; ++i){
        left_extents[i].id = tempU->id;
        left_extents[i].isUpdate = 1;
        left_extents[i].lower[current_dimension] = list_extents[tempU->id + subscriptions].lower[current_dimension];
        left_extents[i].upper[current_dimension] = list_extents[tempU->id + subscriptions].upper[current_dimension];
        tempU = tempU->next;
    }
    tempS = Sr;
    for (i = 0; i < Csr; ++i){
        right_extents[i].id = tempS->id;
        right_extents[i].isUpdate = 0;
        right_extents[i].lower[current_dimension] = list_extents[tempS->id].lower[current_dimension];
        right_extents[i].upper[current_dimension] = list_extents[tempS->id].upper[current_dimension];
        tempS = tempS->next;
    }
    tempU = Ur;
    for (i = Csr; i < Csr + Cur; ++i){
        right_extents[i].id = tempU->id;
        right_extents[i].isUpdate = 1;
        right_extents[i].lower[current_dimension] = list_extents[tempU->id + subscriptions].lower[current_dimension];
        right_extents[i].upper[current_dimension] = list_extents[tempU->id + subscriptions].upper[current_dimension];
        tempU = tempU->next;
    }

    if (Csl != 0 && Cul != 0){
        Partition(result, left_extents, Csl + Cul, min_lower, pivot);
    }
    if (Csr != 0 && Cur != 0){
        Partition(result, right_extents, Csr + Cur, pivot, max_upper);
    }

    list_remove_all(&Sl);
    list_remove_all(&Ul);
    list_remove_all(&Sp);
    list_remove_all(&Up);
    list_remove_all(&Sr);
    list_remove_all(&Ur);
    free(left_extents);
    free(right_extents);

}

void create_and_fill_updates_and_subscriptions(DDM_Extent *list_updates, size_t updates, DDM_Extent *list_subscriptions, size_t subscriptions, size_t dimensions){
    size_t i, k;

    list_extents = (DDM_Extent_Partition*) malloc(sizeof(DDM_Extent_Partition) * (subscriptions + updates));

    for (i = 0; i < subscriptions; ++i){
        list_extents[i].id = list_subscriptions[i].id;
        list_extents[i].isUpdate = 0;
        for (k = 0; k < dimensions; ++k){
            list_extents[i].lower[k] = list_subscriptions[i].lower[k];
            list_extents[i].upper[k] = list_subscriptions[i].upper[k];
        }
    }
    for (i = subscriptions; i < (subscriptions + updates); ++i){
        list_extents[i].id = list_updates[i - subscriptions].id;
        list_extents[i].isUpdate = 1;
        for (k = 0; k < dimensions; ++k){
            list_extents[i].lower[k] = list_updates[i - subscriptions].lower[k];
            list_extents[i].upper[k] = list_updates[i - subscriptions].upper[k];
        }
    }
}

void print_list(uint64_t left_index, uint64_t right_index){
    uint64_t i;

    printf("\n");
    for (i = left_index; i <= right_index; ++i)
        printf("%"PRId64" ", list_extents[i].id);
}

int main(int argc, char *argv[])
{
    uint64_t updates;
    uint16_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

    //DDM's variables
    DDM_Input *ddm_input;

    //temporaneous result matrix
    bitmatrix temp;

    //Initialize variable of DDM
    ddm_input = DDM_Initialize_Input(argc, argv);

    //Check if the initialization of input was succesfully
    if (ddm_input == NULL)
        exit(-1);

    updates = DDM_Get_Updates(*ddm_input);
    subscriptions = DDM_Get_Subscriptions(*ddm_input);
    dimensions = DDM_Get_Dimensions(*ddm_input);
    list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
    list_updates = DDM_Get_Updates_List(*ddm_input);

    //create temporaneous matrix
    bitmatrix_init(&temp, updates, subscriptions);

    create_and_fill_updates_and_subscriptions(list_updates, updates, list_subscriptions, subscriptions, dimensions);

    DDM_Start_Timer(ddm_input);

    for (current_dimension = 0; current_dimension < dimensions; ++current_dimension){

        if (current_dimension > 0){
            //each time execute different dimension
            //reset the temp matrix
            bitmatrix_reset(temp, updates, subscriptions, zero);

            //Execute Algorithm Here **
            Partition(temp, list_extents, subscriptions + updates, LLONG_MIN, LLONG_MAX);

            //Intersect temp matrix and ddm_input->result_mat and store result in ddm_input->result_mat
            bitmatrix_and(ddm_input->result_mat, temp, updates, subscriptions);
        }else{
            //Execute algorithm and store result into ddm_input->result_mat **
            Partition(ddm_input->result_mat, list_extents, subscriptions + updates, LLONG_MIN, LLONG_MAX);
        }
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    //bitmatrix_print_matches(ddm_input->result_mat, updates, subscriptions);
    printf("\nWriting bitmatrix file\n");
    //Write result
    DDM_Write_Result(ddm_input);

    return 0;
}
