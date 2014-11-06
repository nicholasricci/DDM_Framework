#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

const float Lmax = 1.0e6; /* dimension length */

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

    ddm_input = DDM_Initialize_Input(argc, argv);
    dimensions = DDM_Get_Dimensions(*ddm_input);
    updates = DDM_Get_Updates(*ddm_input);
    subscriptions = DDM_Get_Subscriptions(*ddm_input);
    list_updates = DDM_Get_Updates_List(*ddm_input);
    list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);

    //print_list_updates_and_subscriptions(*ddm_input);

    nmatch = 0;
    DDM_Start_Timer(ddm_input);

    //Execute algorithm
    for (i = 0; i < updates; ++i){
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
    }

    DDM_Stop_Timer(ddm_input);

    nmatch = (size_t)DDM_Count_Matches(ddm_input, updates, subscriptions);
    //Write result on file
    DDM_Write_Result(*ddm_input);

    //write on screen nmatches and total time
    printf("%zu matches  %fs\n", nmatch, DDM_Get_Total_Time(*ddm_input) );


    return 0;
}
