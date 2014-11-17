#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

void bruteforce1D(bitmatrix result, DDM_Extent *upds, uint64_t updates, DDM_Extent *subs, uint64_t subscriptions, uint16_t current_dim){
    uint64_t i, j;

    for (i = 0; i < updates; ++i){
#pragma omp parallel for private(j)
        for (j = 0; j < subscriptions; ++j){
            if (
                (upds[i].lower[current_dim] <= subs[j].lower[current_dim]
                && subs[j].lower[current_dim] <= upds[i].upper[current_dim])
                ||
                (upds[i].lower[current_dim] <= subs[j].upper[current_dim]
                && subs[j].upper[current_dim] <= upds[i].upper[current_dim])
                ||
                (subs[j].lower[current_dim] <= upds[i].lower[current_dim]
                && upds[i].lower[current_dim] <= subs[j].upper[current_dim])
                ||
                (subs[j].lower[current_dim] <= upds[i].upper[current_dim]
                && upds[i].upper[current_dim] <= subs[j].upper[current_dim])
                ){
                continue;
            }else{
                bitmatrix_set_value(result, i, j, 0);
            }
        }
    }
}

void write_test(DDM_Extent *upds, uint64_t updates, DDM_Extent *subs, uint64_t subscriptions, uint16_t dim){
    FILE *f;
    uint64_t i;
    uint16_t j;
    char test[1000];

    f = fopen("test.txt", "w+");
    sprintf(test, "#Subscriptions <id> <D1 edges> [<D2 edges>]...\n");
    fprintf(f, test);
    for (i = 0; i < subscriptions; ++i){
        sprintf(test, "%"PRIu64"", subs[i].id);
        for (j = 0; j < dim; ++j){
            sprintf(test, "%s %"PRId64" %"PRId64"", test, subs[i].lower[j], subs[i].upper[j]);
        }
        sprintf(test, "%s\n", test);
        fprintf(f, test);
    }
    sprintf(test, "#Updates <id> <D1 edges> [<D2 edges>]...\n");
    fprintf(f, test);
    for (i = 0; i < updates; ++i){
        sprintf(test, "%"PRIu64"", upds[i].id);
        for (j = 0; j < dim; ++j){
            sprintf(test, "%s %"PRId64" %"PRId64"", test, upds[i].lower[j], upds[i].upper[j]);
        }
        sprintf(test, "%s\n", test);
        fprintf(f, test);
    }
    fclose(f);
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

    bitmatrix_reset(ddm_input->result_mat, updates, subscriptions, one);

    DDM_Start_Timer(ddm_input);

    for (k = 0; k < dimensions; ++k){

        //Execute Algorithm Here
        //BruteForce
        bruteforce1D(ddm_input->result_mat, list_updates, updates, list_subscriptions, subscriptions, k);

        //printf("\nd: %d, nmatches: %"PRIu64"\n", k, count_ones_matrix(ddm_input->result_mat, updates, subscriptions));
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    //bitmatrix_print_matches(ddm_input->result_mat, updates, subscriptions);
    
    //Write result
    DDM_Write_Result(ddm_input);

    return 0;
}
