#ifndef BINARY_PARTITION
#define BINARY_PARTITION

#include "DDM_input_output.h"
#include "lists.h"

typedef struct DDM_Extent_Partition{
    size_t id;
    double lower[MAX_DIMENSIONS];
    double upper[MAX_DIMENSIONS];
    int isUpdate; //1 is true 0 is false
} DDM_Extent_Partition;

void IntersectionCalculation(DDM_Extent_Partition *list_extents, size_t current_dim, list S, list U, bitmatrix result_mat, size_t subscriptions, int isUpdate);

void Partition(DDM_Extent_Partition *list_extents, size_t extents_count, size_t current_dim, double lower_bound, double upper_bound, bitmatrix result_mat, size_t subscriptions, DDM_Extent_Partition *list_extents_full, size_t subscriptions_full);

#endif // BINARY_PARTITION
