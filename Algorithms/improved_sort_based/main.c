/**
 * This file is a default main with DDM_input_output library included.
 * Something to tell to use this:
 * To set a match between an update and subscription use:
 * bitmatrix_set_value(mat, updates, subscriptions, value); //value can be one, for a match, or zero, for a non-match.
 */

#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

typedef struct{
    uint64_t id;
    int64_t point[MAX_DIMENSIONS];
    uint8_t isLower;
    uint8_t isUpdate;
}list_element;

uint16_t current_dim;

void bitvector_print(bitvector vec, uint64_t subscriptions){
    uint64_t i;

    printf("\n");
    for (i = 0; i < subscriptions; ++i){
        uint64_t bitpos_invec = i / BIT_NUMBER;
        bitelem bitpos = i % BIT_NUMBER;
        bitelem mask = DBIT(bitpos);
        if (mask & vec[bitpos])
            printf("1 ");
        else
            printf("0 ");
    }

}

void bitvector_set_value(bitvector vec, uint64_t subscription, mat_value value){
    //position in bitvector that we have to set the value
    uint64_t bitpos_invec = subscription / BIT_NUMBER;
    //bit position in SPACE_TYPE we have to set
    bitelem bitpos =  subscription % BIT_NUMBER;
    bitelem mask = DBIT(bitpos);
    if (value == one){
        vec[bitpos_invec] |= mask;
    }else{
        vec[bitpos_invec] &= ~mask;
    }
}

void bitvector_or(bitvector vec, const bitvector mask, uint64_t subscriptions){
    uint64_t j;

    for (j = 0; j < ceill(subscriptions / (float)BIT_NUMBER); ++j)
        vec[j] |= mask[j];
}

int comparison(const void * a, const void * b){
    list_element  *p1 = a;
    list_element *p2 = b;
    if (p1->point[current_dim] < p2->point[current_dim])
        return -1;
    else if(p1->point[current_dim] > p2->point[current_dim])
        return 1;
    else
        return 0;
}

void sort_matching1D(bitmatrix result, list_element *l, uint64_t updates, uint64_t subscriptions, bitvector SubscriptionSetBefore, bitvector SubscriptionSetAfter){
    uint64_t update_size = updates * 2;
    uint64_t i;
    qsort(l, (updates + subscriptions) * 2, sizeof(list_element), comparison);

    for (i = 0; update_size > 0; ++i){
        if (!l[i].isUpdate){
            if (l[i].isLower){
                bitvector_set_value(SubscriptionSetAfter, l[i].id, zero);
            }else{
                bitvector_set_value(SubscriptionSetBefore, l[i].id, one);
            }
        }else{
            update_size--;
            if (l[i].isLower){
                bitvector_or(result[l[i].id], SubscriptionSetBefore, subscriptions);
            }else{
                bitvector_or(result[l[i].id], SubscriptionSetAfter, subscriptions);
            }
        }
    }
    bitmatrix_not(result, updates, subscriptions);
}

list_element* fill_list(DDM_Extent *list_updates, uint64_t updates, DDM_Extent *list_subscriptions, uint64_t subscriptions, uint16_t dimensions){
    uint64_t i, j, k;
    list_element *l = (list_element*) malloc(sizeof(list_element) * (updates + subscriptions) * 2);

    j = 0;
    for (i = 0; i < subscriptions; ++i){
        for (k = 0; k < dimensions; ++k){
            l[j].id = list_subscriptions[i].id;
            l[j].isLower = 1;
            l[j].point[k] = list_subscriptions[i].lower[k];
            l[j].isUpdate = 0;
            l[j+1].id = list_subscriptions[i].id;
            l[j+1].isLower = 0;
            l[j+1].point[k] = list_subscriptions[i].upper[k];
            l[j+1].isUpdate = 0;
        }
        j+=2;
    }
    for (i = 0; i < updates; ++i){
        for (k = 0; k < dimensions; ++k){
            l[j].id = list_updates[i].id;
            l[j].isLower = 1;
            l[j].point[k] = list_updates[i].lower[k];
            l[j].isUpdate = 1;
            l[j+1].id = list_updates[i].id;
            l[j+1].isLower = 0;
            l[j+1].point[k] = list_updates[i].upper[k];
            l[j+1].isUpdate = 1;
        }
        j+=2;
    }

    return l;
}

int main(int argc, char *argv[])
{
    uint64_t updates, subscriptions;
    uint16_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;
    list_element *l;
    bitvector SubscriptionSetBefore, SubscriptionSetAfter;
    uint64_t i;

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

    //create list
    l = fill_list(list_updates, updates, list_subscriptions, subscriptions, dimensions);
    SubscriptionSetBefore = (bitvector) malloc(ceill(subscriptions / (float)BIT_NUMBER) * sizeof(bitelem));
    SubscriptionSetAfter = (bitvector) malloc(ceill(subscriptions / (float)BIT_NUMBER) * sizeof(bitelem));

    DDM_Start_Timer(ddm_input);

    for (current_dim = 0; current_dim < dimensions; ++current_dim){
        /*memset(SubscriptionSetBefore, BIT_MIN_ELEM, ceill(subscriptions / (float)BIT_NUMBER) * sizeof(bitelem));
        memset(SubscriptionSetAfter, BIT_MAX_ELEM, ceill(subscriptions / (float)BIT_NUMBER) * sizeof(bitelem));*/
        for (i = 0; i < subscriptions; ++i)
            bitvector_set_value(SubscriptionSetBefore, i, zero);
        for (i = 0; i < subscriptions; ++i)
            bitvector_set_value(SubscriptionSetAfter, i, one);
        if (current_dim > 0){
            //each time execute different dimension
            //reset the temp matrix
            bitmatrix_reset(temp, updates, subscriptions, zero);

            //Execute Algorithm Here **
            sort_matching1D(temp, l, updates, subscriptions, SubscriptionSetBefore, SubscriptionSetAfter);
            //Intersect temp matrix and ddm_input->result_mat and store result in ddm_input->result_mat
            bitmatrix_and(ddm_input->result_mat, temp, updates, subscriptions);
        }else{
            //Execute algorithm and store result into ddm_input->result_mat **
            /*bitvector_print(SubscriptionSetAfter, subscriptions);
            bitvector_print(SubscriptionSetBefore, subscriptions);*/
            sort_matching1D(ddm_input->result_mat, l, updates, subscriptions, SubscriptionSetBefore, SubscriptionSetAfter);
        }
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    //Write result
    DDM_Write_Result(ddm_input);

    return 0;
}
