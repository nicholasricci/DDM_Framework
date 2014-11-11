#include "binary_partition.h"

void IntersectionCalculation(DDM_Extent_Partition *list_extents, size_t current_dim, list S, list U, bitmatrix result_mat, size_t subscriptions, int isUpdate){
    list_element *tempS;
    list_element *tempU;

    if (S != NULL && U != NULL){
        tempS = S;
        while (tempS != NULL){
            tempU = U;
            while (tempU != NULL){
                if (isUpdate){
                    if (list_extents[tempS->id + subscriptions].upper[current_dim] > list_extents[tempU->id].lower[current_dim])

                        bitmatrix_set_value(result_mat, tempS->id, tempU->id, one);
                }else
                    if (list_extents[tempS->id].upper[current_dim] > list_extents[tempU->id + subscriptions].lower[current_dim])
                        bitmatrix_set_value(result_mat, tempU->id, tempS->id, one);
                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }
}

void Partition(DDM_Extent_Partition *list_extents, size_t extents_count, size_t current_dim, double lower_bound, double upper_bound, bitmatrix result_mat, size_t subscriptions, DDM_Extent_Partition *list_extents_full, size_t subscriptions_full){
    DDM_Extent_Partition *list_right_extents, *list_left_extents;
    double p;
    list Ul, Sl, Ur, Sr, Up, Sp;
    size_t Cul, Csl, Cur, Csr;
    size_t i;
    list_element * tempS;
    list_element * tempU;

    //if it's called with one extents return
    if (extents_count <= 1)
        return;

    //chose pivot
    p = (double) lower_bound + (upper_bound - lower_bound)/2.0;

    //init all lists
    list_init(&Ul);
    list_init(&Sl);
    list_init(&Ur);
    list_init(&Sr);
    list_init(&Up);
    list_init(&Sp);

    Cul = Csl = Cur = Csr = 0;

    //Divide extents into three list: left, right, pivot
    for (i = 0; i < extents_count; ++i){
        if (list_extents[i].isUpdate){
            //if is an update extent
            if (list_extents[i].upper[current_dim] <= p){
                list_add_by_id(&Ul, list_extents[i].id);
                Cul++;
            }else if (list_extents[i].lower[current_dim] > p){
                list_add_by_id(&Ur, list_extents[i].id);
                Cur++;
            }else{
                list_add_by_id(&Up, list_extents[i].id);
            }
        }else{
            if (list_extents[i].upper[current_dim] <= p){
                list_add_by_id(&Sl, list_extents[i].id);
                Csl++;
            }else if (list_extents[i].lower[current_dim] > p){
                list_add_by_id(&Sr, list_extents[i].id);
                Csr++;
            }else{
                list_add_by_id(&Sp, list_extents[i].id);
            }
        }
    }

    //Set Matched the elements in Sp and Up
    //list_print(Up, "Update pivot");
    //list_print(Sp, "Subscription pivot");
    tempS = Sp;
    if(Sp != NULL && Up != NULL)
    {
        while(tempS != NULL)
        {
            tempU = Up;
            while(tempU != NULL)
            {
                bitmatrix_set_value(result_mat, tempU->id, tempS->id, one);
                //set_bit_mat(*result_mat, tempU->id, tempS->id);
                tempU = tempU->next;
            }
            tempS = tempS->next;
        }
    }

    //Check match
    /*IntersectionCalculation(list_extents, current_dim, Sl, Up, result_mat, subscriptions, 0);
    IntersectionCalculation(list_extents, current_dim, Up, Sr, result_mat, subscriptions, 1);
    IntersectionCalculation(list_extents, current_dim, Ul, Sp, result_mat, subscriptions, 1);
    IntersectionCalculation(list_extents, current_dim, Sp, Ur, result_mat, subscriptions, 0);*/
    IntersectionCalculation(list_extents_full, current_dim, Sl, Up, result_mat, subscriptions_full, 0);
    IntersectionCalculation(list_extents_full, current_dim, Up, Sr, result_mat, subscriptions_full, 1);
    IntersectionCalculation(list_extents_full, current_dim, Ul, Sp, result_mat, subscriptions_full, 1);
    IntersectionCalculation(list_extents_full, current_dim, Sp, Ur, result_mat, subscriptions_full, 0);

    //init and fill left and right list
    list_right_extents = (DDM_Extent_Partition*)malloc(sizeof(DDM_Extent_Partition) * (Cur + Csr));
    list_left_extents = (DDM_Extent_Partition*)malloc(sizeof(DDM_Extent_Partition) * (Cul + Csl));

    tempS = Sl;
    for (i = 0; i < Csl; ++i){
        list_left_extents[i].id = list_extents_full[tempS->id].id;
        list_left_extents[i].isUpdate = list_extents_full[tempS->id].isUpdate;
        list_left_extents[i].lower[current_dim] = list_extents_full[tempS->id].lower[current_dim];
        list_left_extents[i].upper[current_dim] = list_extents_full[tempS->id].upper[current_dim];
        tempS = tempS->next;
    }
    tempU = Ul;
    for (i = Csl; i < (Csl + Cul); ++i){
        list_left_extents[i].id = list_extents_full[tempU->id + subscriptions_full].id;
        list_left_extents[i].isUpdate = list_extents_full[tempU->id + subscriptions_full].isUpdate;
        list_left_extents[i].lower[current_dim] = list_extents_full[tempU->id + subscriptions_full].lower[current_dim];
        list_left_extents[i].upper[current_dim] = list_extents_full[tempU->id + subscriptions_full].upper[current_dim];
        tempU = tempU->next;
    }
    tempS = Sr;
    for (i = 0; i < Csr; ++i){
        list_right_extents[i].id = list_extents_full[tempS->id].id;
        list_right_extents[i].isUpdate = list_extents_full[tempS->id].isUpdate;
        list_right_extents[i].lower[current_dim] = list_extents_full[tempS->id].lower[current_dim];
        list_right_extents[i].upper[current_dim] = list_extents_full[tempS->id].upper[current_dim];
        tempS = tempS->next;
    }
    tempU = Ur;
    for (i = Csr; i < (Csr + Cur); ++i){
        list_right_extents[i].id = list_extents_full[tempU->id + subscriptions_full].id;
        list_right_extents[i].isUpdate = list_extents_full[tempU->id + subscriptions_full].isUpdate;
        list_right_extents[i].lower[current_dim] = list_extents_full[tempU->id + subscriptions_full].lower[current_dim];
        list_right_extents[i].upper[current_dim] = list_extents_full[tempU->id + subscriptions_full].upper[current_dim];
        tempU = tempU->next;
    }

    //Repartitioning
    if (Cul != 0 && Csl != 0){
        Partition(list_left_extents, (Cul + Csl), current_dim, lower_bound, p, result_mat, Csl, list_extents_full, subscriptions_full);
    }
    if (Cur != 0 && Csr != 0){
        Partition(list_right_extents, (Cur + Csr), current_dim, p, upper_bound, result_mat, Csr, list_extents_full, subscriptions_full);
    }

    free(list_right_extents);
    free(list_left_extents);
    list_remove_all(&Sl);
    list_remove_all(&Ul);
    list_remove_all(&Sp);
    list_remove_all(&Up);
    list_remove_all(&Sr);
    list_remove_all(&Ur);
}
