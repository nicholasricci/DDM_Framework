#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"
#include "int_tree.h"

size_t ddm_matching( uint_fast8_t **result,
                   const struct interval* sub, size_t n,
                   const struct interval* upd, size_t m )
{
    size_t i, j;
    struct int_tree tree;
    size_t count = 0;
    int_tree_init( &tree );
    for (i=0; i<n; ++i) {
        int_tree_insert( &tree, &sub[i] );
    }

#pragma omp parallel for reduction(+:result)
    for ( j=0; j<m; ++j) {
       count += int_tree_find_intersect( result, &tree, &upd[j] );
    }
    /* we do not need to destroy the tree, since our test program
       exists right after this function. If this is not the case, you
       should call int_tree_clear( &tree ) here */

    int_tree_clear(&tree);
    return count;
}

int count_ones_matrix(uint_fast8_t **result, size_t updates, size_t subscriptions){
    int count = 0;
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            if (result[i][j] == 1)
                count++;

    return count;
}

void reset_matrix_zero(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            result[i][j] = 0;
}

void reset_matrix_one(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i)
        for (j = 0; j < subscriptions; ++j)
            result[i][j] = 1;
}

void print_matrix(uint_fast8_t **result, size_t updates, size_t subscriptions){
    size_t i, j;

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j)
            printf("%d ", result[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    size_t updates, subscriptions, dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

    //DDM's variables
    DDM_Input *ddm_input;

    //Indexes
    size_t k, i, j;

    //Interval tree variable
    struct interval *subs, *upds;
    size_t count = 0;

    //TEST
    uint_fast8_t **matrix, **temp;

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

    subs = (struct interval *) malloc(sizeof(struct interval) * subscriptions);
    upds = (struct interval *) malloc(sizeof(struct interval) * updates);

    matrix = (uint_fast8_t **) malloc(sizeof(uint_fast8_t *) * updates);
    temp = (uint_fast8_t **) malloc(sizeof(uint_fast8_t *) * updates);
    for (i = 0; i < updates; ++i){
        matrix[i] = (uint_fast8_t *) malloc(sizeof(uint_fast8_t) * subscriptions);
        temp[i] = (uint_fast8_t *) malloc(sizeof(uint_fast8_t) * subscriptions);
    }

    for (i = 0; i < updates; ++i){
        for (j = 0; j < subscriptions; ++j){
            matrix[i][j] = 1;
        }
    }

    DDM_Start_Timer(ddm_input);

    //Execute Algorithm Here

    for (k = 0; k < dimensions; ++k){
        //fill the current dimension
        for (i = 0; i < updates; ++i){
            upds[i].id = list_updates[i].id;
            upds[i].lower = list_updates[i].lower[k];
            upds[i].upper = list_updates[i].upper[k];
        }
        for (i = 0; i < subscriptions; ++i){
            subs[i].id = list_subscriptions[i].id;
            subs[i].lower = list_subscriptions[i].lower[k];
            subs[i].upper = list_subscriptions[i].upper[k];
        }
        //execute algorithm in one dimension and set the bitmatrix with update row and subscription column and set
        //bit to one with this function
        //set_bit_mat(temp, update_id, subscription_id);
        //to do and with temporaneous matrix result and the final matrix result do this

        reset_matrix_zero(temp, updates, subscriptions);

        count = ddm_matching(temp, subs, subscriptions, upds, updates);
        //print_readable_bitmatrix(temp, 0, updates, subscriptions);
        for (i = 0; i < updates; ++i)
            for (j = 0; j < subscriptions; ++j)
                matrix[i][j] *= temp[i][j];
        printf("\n nmatches for dimension n.%zu: %zu\n", k, count_ones_matrix(temp, updates, subscriptions));
    }

    DDM_Stop_Timer(ddm_input);

    printf("\nnmatches: %"PRIu64"\n", count_ones_matrix(matrix, updates, subscriptions));
    //Write result
    DDM_Write_Result(*ddm_input);

    scanf("%zu", &dimensions);

    return 0;
}
