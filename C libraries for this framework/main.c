/**
 * This file is a default main with DDM_input_output library included.
 * Something to tell to use this:
 * To set a match between an update and subscription use:
 * bitmatrix_set_value(mat, updates, subscriptions, value); //value can be one, for a match, or zero, for a non-match.
 */

#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

int main(int argc, char *argv[])
{
    uint64_t updates, subscriptions;
    uint16_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

    //DDM's variables
    DDM_Input *ddm_input;

    //Indexes
    uint16_t k;

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

    DDM_Start_Timer(ddm_input);

    for (k = 0; k < dimensions; ++k){
        if (k > 0){
            //each time execute different dimension
            //reset the temp matrix
            bitmatrix_reset(temp, updates, subscriptions, zero);

            //Execute Algorithm Here **

            //Intersect temp matrix and ddm_input->result_mat and store result in ddm_input->result_mat
            bitmatrix_and(ddm_input->result_mat, temp, updates, subscriptions);
        }else{
            //Execute algorithm and store result into ddm_input->result_mat **
        }
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    //Write result
    DDM_Write_Result(*ddm_input);

    return 0;
}
