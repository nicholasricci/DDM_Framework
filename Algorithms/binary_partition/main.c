#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"
#include "binary_partition.h"
#include "limits.h"

#define LOWER_BOUND INT_MIN
#define UPPER_BOUND INT_MAX

DDM_Extent_Partition* create_and_fill_updates_and_subscriptions(DDM_Extent *list_updates, size_t updates, DDM_Extent *list_subscriptions, size_t subscriptions, size_t dimensions){
    size_t i, k;
    DDM_Extent_Partition *list_extents = (DDM_Extent_Partition*) malloc(sizeof(DDM_Extent_Partition) * (subscriptions + updates));

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

    return list_extents;
}

int main(int argc, char *argv[])
{
    //Binary Partition Variables
    uint64_t updates, subscriptions;
    uint64_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;
    DDM_Extent_Partition *list_extents;
    bitmatrix temp_mat;
    uint16_t k;

    //DDM's variables
    DDM_Input *ddm_input;

    //Check if the initialization of input was succesfully
    if ((ddm_input = DDM_Initialize_Input(argc, argv)) == NULL)
        exit(-1);

    updates = DDM_Get_Updates(*ddm_input);
    subscriptions = DDM_Get_Subscriptions(*ddm_input);
    dimensions = DDM_Get_Dimensions(*ddm_input);
    list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
    list_updates = DDM_Get_Updates_List(*ddm_input);


    //initialize bitmatrix for result
    bitmatrix_init(&ddm_input->result_mat, updates, subscriptions);
    bitmatrix_init(&temp_mat, updates, subscriptions);

    //fill list extents with before updates and after subscriptions
    list_extents = create_and_fill_updates_and_subscriptions(list_updates, updates, list_subscriptions, subscriptions, dimensions);

    //TEST lists

    //print_list_updates_and_subscriptions(*ddm_input);

    DDM_Start_Timer(ddm_input);

    //Execute Algorithm Here
    for (k = 0; k < dimensions; ++k){
        if (k == 0)
            //set result in result_mat
            Partition(list_extents, (subscriptions + updates), k, LOWER_BOUND, UPPER_BOUND, ddm_input->result_mat, subscriptions, list_extents, subscriptions);
        else{
            bitmatrix_reset(temp_mat, updates, subscriptions, zero);
            //set result in temp_mat and do the AND operation between matrix
            //partition
            Partition(list_extents, (subscriptions + updates), k, LOWER_BOUND, UPPER_BOUND, temp_mat, subscriptions, list_extents, subscriptions);
            bitmatrix_and(ddm_input->result_mat, temp_mat, updates, subscriptions);
        }
    }

    DDM_Stop_Timer(ddm_input);
    bitmatrix_free(&temp_mat, updates, subscriptions);
    //Write result
    //DDM_Write_Result(*ddm_input);
    printf("\nnmatches: %"PRId64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    return 0;
}
