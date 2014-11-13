#include <stdio.h>
#include <stdlib.h>

#include "DDM_input_output.h"

int main(int argc, char *argv[])
{
    char write_file[] = "diff.txt";
    uint64_t updates = atoi(argv[3]), subscriptions = atoi(argv[4]), count;
    bitmatrix bf, oa;
    FILE *fp;

    bitmatrix_read_file(&bf, updates, subscriptions, argv[1]);
    bitmatrix_read_file(&oa, updates, subscriptions, argv[2]);

    count = bitmatrix_differences(bf, oa, updates, subscriptions);

    fp = fopen(write_file, "w");
    if (fp == NULL){
        printf("\nError to create file\n");
        exit(-1);
    }
    fprintf(fp, "%"PRId64"", count);
    fclose(fp);

    bitmatrix_free(&bf, updates, subscriptions);
    bitmatrix_free(&oa, updates, subscriptions);

    return 0;
}
