#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"
#include <stdlib.h>
#include <math.h>

#define PARTS 2

int64_t MAXUINT64 = pow(2, 32) - 1;
int64_t MINUINT64 = -pow(2, 32) + 1;

int compare_extents(const void* a, const void* b)   // comparison function
{
    DDM_Extent e1 = *(DDM_Extent*) (a);
    DDM_Extent e2 = *(DDM_Extent*)(b);
    if(e1.upper < e2.upper) return -1;
    if(e1.upper > e2.upper) return 1;
    return 0;
}

void grid_sort_2(uint_fast8_t **result, DDM_Extent *upds, uint64_t updates, DDM_Extent *subs, uint64_t subscriptions, uint16_t current_dim){

    int64_t p_lower[PARTS];
    int64_t p_upper[PARTS];
    int i;
    int64_t step = (MAXUINT64 - MINUINT64) / PARTS;
    int64_t start = MINUINT64;
    uint64_t end_parts_updates;
    uint64_t end_parts_subscriptions;
    uint64_t k, j;

    //create any part
    for (i = 0; i < PARTS; ++i){
        p_lower[i] = start;
        p_upper[i] = start + step;
        start += step;
    }

    //order updates and subscriptions
    qsort(upds, updates, sizeof(DDM_Extent), compare_extents);
    qsort(subs, subscriptions, sizeof(DDM_Extent), compare_extents);

    for (i = 0; i < PARTS; ++i){
        //search index of updates and subscriptions that end the current part
        for (k = 0; k < updates; ++k)
            if (upds[k].lower[current_dim] > p_upper[i])
                break;
        end_parts_updates = k - 1;
        for (k = 0; k < subscriptions; ++k)
            if (subs[k].lower[current_dim] > p_upper[i])
                break;
        end_parts_subscriptions = k -1;

        //now set all matrix result
        for (k = 0; k < updates; ++k){
            for (j = 0; j < subscriptions; ++j)
                if (current_dim == 0)
                    set_value_mat(result, k, j, one);
        }
    }
}

int main(int argc, char *argv[])
{
    uint64_t updates, subscriptions;
    uint16_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

    //DDM's variables
    DDM_Input *ddm_input;

    //Indexes
    uint16_t k;

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

    //reset ddm_input->result_mat to one, so you can do the and operation between
    //temp(init 0) and ddm_input->result_mat(init 1)
    reset_mat(ddm_input->result_mat, updates, subscriptions, zero);

    DDM_Start_Timer(ddm_input);

    for (k = 0; k < dimensions; ++k){

        //Execute Algorithm Here

    }

    DDM_Stop_Timer(ddm_input);

    //Write result
    DDM_Write_Result(*ddm_input);

    printf("\nnmatches: %"PRIu64"\n", DDM_Count_Matches(ddm_input));

    return 0;
}
