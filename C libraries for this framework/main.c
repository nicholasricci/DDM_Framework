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
    uint_fast8_t **temp;

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
    temp = create_result_matrix(updates, subscriptions);
    //reset ddm_input->result_mat to one, so you can do the and operation between
    //temp(init 0) and ddm_input->result_mat(init 1)
    reset_mat(ddm_input->result_mat, updates, subscriptions, one);

    DDM_Start_Timer(ddm_input);

    for (k = 0; k < dimensions; ++k){
        //each time execute different dimension
        //reset the temp matrix
        reset_mat(temp, updates, subscriptions, zero);

        //Execute Algorithm Here

        //Intersect temp matrix and ddm_input->result_mat and store result in ddm_input->result_mat
        DDM_And_Operation_Between_Matrices(ddm_input, temp, updates, subscriptions);
    }

    DDM_Stop_Timer(ddm_input);

    //Write result
    DDM_Write_Result(*ddm_input);

    printf("\nnmatches: %"PRIu64"\n", DDM_Count_Matches(ddm_input));

    return 0;
}
