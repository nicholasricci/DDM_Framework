#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"
#include "int_tree.h"
#include <stdio.h>

uint64_t ddm_matching( bitmatrix result,
                   const struct interval* sub, size_t n,
                   const struct interval* upd, size_t m,
                   size_t current_dim )
{
    size_t i, j;
    struct int_tree tree;
    size_t count = 0;
    int_tree_init( &tree );
    for (i=0; i<n; ++i) {
        int_tree_insert( &tree, &sub[i], current_dim );
    }

#pragma omp parallel for reduction(+:result)
    for ( j=0; j<m; ++j) {
       count += int_tree_find_intersect( result, &tree, &upd[j], current_dim );
    }
    /* we do not need to destroy the tree, since our test program
       exists right after this function. If this is not the case, you
       should call int_tree_clear( &tree ) here */

    int_tree_clear(&tree);
    return count;
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
    bitmatrix temp;

    //DDM's variables
    DDM_Input *ddm_input;

    //Indexes
    uint64_t i;
    uint16_t k;

    //Interval tree variable
    struct interval *subs, *upds;
    uint64_t count = 0;

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


    //write_test(list_updates, updates, list_subscriptions, subscriptions, dimensions);

    subs = (struct interval *) malloc(sizeof(struct interval) * subscriptions);
    upds = (struct interval *) malloc(sizeof(struct interval) * updates);

    //create temporaneous matrix
    bitmatrix_init(&temp, updates, subscriptions);
    //reset ddm_input->result_mat to one, so you can do the and operation between
    //temp(init 0) and ddm_input->result_mat(init 1)
    bitmatrix_init(&ddm_input->result_mat, updates, subscriptions);

    //fill all data to all dimensions
    for (i = 0; i < updates; ++i){
        for (k = 0; k < dimensions; ++k){
            upds[i].id = list_updates[i].id;
            upds[i].lower[k] = list_updates[i].lower[k];
            upds[i].upper[k] = list_updates[i].upper[k];
        }
    }
    for (i = 0; i < subscriptions; ++i){
        for (k = 0; k < dimensions; ++k){
            subs[i].id = list_subscriptions[i].id;
            subs[i].lower[k] = list_subscriptions[i].lower[k];
            subs[i].upper[k] = list_subscriptions[i].upper[k];
        }
    }


    DDM_Start_Timer(ddm_input);

    for (k = 0; k < dimensions; ++k){

        if (k > 0){
        //each time execute different dimension
        //reset the temp matrix
        bitmatrix_reset(temp, updates, subscriptions, zero);
        //Execute Algorithm Here
        count = ddm_matching(temp, subs, subscriptions, upds, updates, k);

        //printf("\nd: %d, nmatches: %"PRIu64"\n", k, count_ones_matrix(temp, updates, subscriptions));

        //Intersect temp matrix and ddm_input->result_mat and store result in ddm_input->result_mat
        bitmatrix_and(ddm_input->result_mat, temp, updates, subscriptions);
        }else{
            count = ddm_matching(ddm_input->result_mat, subs, subscriptions, upds, updates, k);
        }
    }

    DDM_Stop_Timer(ddm_input);
    //bitmatrix_free(&temp, updates, subscriptions);

    printf("\nnmatches: %"PRIu64"\n", bitmatrix_count_ones(ddm_input->result_mat, updates, subscriptions));

    //Write result
    printf("\nWriting matrix result on a file\n");
    DDM_Write_Result(ddm_input);


    return 0;
}
