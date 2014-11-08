/*
 * SortMatching
 * Copyright 2012 Marco Mandrioli
 *
 * This file is part of SortMatching.
 *
 * SortMatching is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SortMatching is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with SortMatching.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "../include/types.h"

#include "../include/matching.h"
#include "../include/test_generator.h"
#include "../include/utils.h"
#include "../include/error.h"
#include "../include/DDM_input_output.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __TEST
#include <time.h>
#endif // __TEST


/** \mainpage
\author Marco Mandrioli

\section Intro1 Introdction

\section Intro2 Version
*/


/**file main.c
\brief File containing the main function.
*/


/** \brief Main function.
*/
int main(int argc, char *argv[])
{
	bitmatrix result;
	match_data_t data;
	float alfa;
#ifdef __TEST
	FILE *fout;
	char fname[FILE_NAME_SIZE];
	clock_t start, end;
#endif // __TEST
	uint64_t updates, subscriptions;
    uint16_t dimensions;
    DDM_Extent *list_updates, *list_subscriptions;

	//Indexes
    uint16_t k;
    uint64_t i;
    
    //DDM's variables
    DDM_Input *ddm_input;

	//Initialize variable of DDM
    ddm_input = DDM_Initialize_Input(argc, argv);

    //Check if the initialization of input was succesfully
    if (ddm_input == NULL)
        exit(-1);

	if ((argc == 2 && strcmp(argv[1], "--help") == 0))
	{
		printf("\nSYNOPSIS:\n\n");
		printf("%s <extents> <dimensions> <alfa>\n\n", FILENAME(argv[0]));
		
		return (int)err_none;
	}

	updates = DDM_Get_Updates(*ddm_input);
	subscriptions = DDM_Get_Subscriptions(*ddm_input);
    dimensions = DDM_Get_Dimensions(*ddm_input);
    list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
    list_updates = DDM_Get_Updates_List(*ddm_input);
    alfa = DDM_Get_Alfa(*ddm_input);
	
#ifdef __RANDOM_SET
	// generate a random data set
	if (test_generator_random(&data, updates, subscriptions, dimensions, alfa) != err_none)
#else // __RANDOM_SET
	// generate a fixed data set
	if (test_generator(&data, updates, subscrs, dimensions) != err_none)
#endif // __RANDOM_SET
		return (int)print_error_string();

	data.size_update = updates;
	data.size_subscr = subscriptions;
	data.dimensions = dimensions;

	for (k = 0; k < dimensions; ++k){
		for (i = 0; i < updates; ++i){
			data.update[i].id = list_updates[i].id;
			data.update[i].endpoints[k].lower = list_updates[i].lower[k];
			data.update[i].endpoints[k].upper = list_updates[i].upper[k];
		}
		for (i = 0; i < subscriptions; ++i){
			data.subscr[i].id = list_subscriptions[i].id;
			data.subscr[i].endpoints[k].lower = list_subscriptions[i].lower[k];
			data.subscr[i].endpoints[k].upper = list_subscriptions[i].upper[k];
		}
	}

#ifdef __TEST
	// start test timer
	//start = clock();
	DDM_Start_Timer(ddm_input);
#endif // __TEST

	// allocate the result bit matrix
	if (create_bit_matrix(&result, data.size_update, data.size_subscr) != err_none)
		return (int)print_error_string();

	// main algorithm
	if (sort_matching(data, result) != err_none)
		return (int)print_error_string();

#ifdef __TEST
	// stop test timer
	DDM_Stop_Timer(ddm_input);
	//end = clock();

	DDM_Write_Result(*ddm_input);

#ifdef __DEBUG
	getchar();
#endif // __DEBUG
#endif // __TEST

#ifdef __VERBOSE
	// print the result bit matrix
	print_bitmatrix(result, data.size_update, data.size_subscr);

#ifdef __DEBUG
	getchar();
#endif // __DEBUG
#endif // __VERBOSE

#ifndef __NOFREE
	// free memory
	free(*result);
	free(result);
	free(data.update);
	free(data.subscr);
#endif // __NOFREE

	return (int)err_none;
}
