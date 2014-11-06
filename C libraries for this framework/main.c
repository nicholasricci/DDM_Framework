#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

int main(int argc, char *argv[])
{
    size_t updates, subscriptions, dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

    //DDM's variables
    DDM_Input *ddm_input;
    DDM_Timer ddm_timer;

    //Indexes
    size_t k;

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

    DDM_Start_Timer(ddm_input);

    //Execute Algorithm Here

    bitmatrix temp;
    //initialize bitmatrix
    init_bit_matrix(&temp, updates, subscriptions, uninitialized);
    for (k = 0; k < dimensions; ++k){
        //reset the temp matrix
        reset_whole_bit_mat(temp, updates, subscriptions);
        //execute algorithm in one dimension and set the bitmatrix with update row and subscription column and set
        //bit to one with this function
        //set_bit_mat(temp, update_id, subscription_id);
        //to do and with temporaneous matrix result and the final matrix result do this
        DDM_AND_Op_With_Bitmatrix(ddm_input, temp);
    }

    DDM_Stop_Timer(ddm_input);

    //Write result
    DDM_Write_Result(*ddm_input);

    return 0;
}
