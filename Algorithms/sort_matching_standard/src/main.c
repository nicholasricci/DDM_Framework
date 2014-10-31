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

\section Intro1 Introduction

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
	_INT extents;
	_INT updates;
	_INT subscrs;
	_INT dimensions;
	float alfa;
#ifdef __TEST
	//FILE *fout;
	char fname[FILE_NAME_SIZE];
	DDM_Timer ddm_timer;
	DDM_Input *ddm_input;
	DDM_Extent *list_updates, *list_subscriptions;
	int i, j;
	//clock_t start, end;
#endif // __TEST

	if ((argc == 2 && strcmp(argv[1], "--help") == 0) || argc != 5)
	{
		printf("\nSYNOPSIS:\n\n");
		printf("%s <extents> <dimensions> <alfa>\n\n", FILENAME(argv[0]));
		
		return (int)err_none;
	}
	
	ddm_input = DDM_Initialize_Input(argc, argv);
	
	extents = (_INT)DDM_Get_Extents(*ddm_input);
	if ( (extents <= 0) || (extents %2 != 0) )
		printf("\nNot a valid number of update extents.\n");
	
	updates = (_INT) DDM_Get_Updates(*ddm_input);
	subscrs = (_INT) DDM_Get_Subscriptions(*ddm_input);
	/*
	updates = (_INT) extents/2;
	subscrs = updates;
	*/
	
	dimensions = (_INT) DDM_Get_Dimensions(*ddm_input);
	if (dimensions <= 0)
	  printf("\nNot a valid number of dimensions.\n");
	
	if (DDM_Is_Alfa_Test(*ddm_input)){
	
	  /*Alfa Test*/
	  alfa = DDM_Get_Alfa(*ddm_input);
	  if (alfa <= 0)
		  printf("\nNot a valid alfa parameter.\n");
	  
	  if (extents <= 0 || dimensions <= 0 || alfa <= 0 || (extents %2 != 0))
	  {
		  set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);
		  return (int)print_error_string();
	  }

  #ifdef __RANDOM_SET
	  // generate a random data set
	  if (test_generator_random(&data, updates, subscrs, dimensions, alfa) != err_none)
  #else // __RANDOM_SET
	  // generate a fixed data set
	  if (test_generator(&data, updates, subscrs, dimensions) != err_none)
  #endif // __RANDOM_SET
		  return (int)print_error_string();

	}else{
	  /*Other Kind of Test*/
	  list_updates = (DDM_Extent *) malloc(sizeof(DDM_Extent) * updates);
	  list_subscriptions = (DDM_Extent *) malloc(sizeof(DDM_Extent) * subscrs);
	  
	  data.dimensions = dimensions;
	  data.size_update = updates;
	  data.size_subscr = subscrs;
	  data.update = (extent_t *)malloc(updates * sizeof(extent_t));
	  data.subscr = (extent_t *)malloc(subscrs * sizeof(extent_t));
	  
	  for (i = 0; i < subscrs; i++){
	    data.subscr[i].id = list_subscriptions[i].id;
	    for (j = 0; j < dimensions; j++){
	      data.subscr[i].endpoints[j].lower = list_subscriptions[i].lower[j];
	      data.subscr[i].endpoints[j].upper = list_subscriptions[i].upper[j];
	    }
	  }
	  for (i = 0; i < updates; i++){
	    data.update[i].id = list_updates[i].id;
	    for (j = 0; j < dimensions; j++){
	      data.update[i].endpoints[j].lower = list_updates[i].lower[j];
	      data.update[i].endpoints[j].upper = list_updates[i].lower[j];
	    }
	  }
	}
	
	
#ifdef __TEST
	// start test timer
	DDM_Start_Timer(&ddm_timer);
	//start = clock();
#endif // __TEST

	// allocate the result bit matrix
	if (create_bit_matrix(&result, data.size_update, data.size_subscr) != err_none)
		return (int)print_error_string();

	// main algorithm
	if (sort_matching(data, result) != err_none)
		return (int)print_error_string();

#ifdef __TEST
	// stop test timer
	DDM_Stop_Timer(&ddm_timer);
	//end = clock();

	// format output file name
	if (sprintf(fname, "%s_%d_%d_%2.3f.txt", FILENAME(argv[0]), updates + subscrs, dimensions, alfa) < 0)
	{
		set_error(err_file, __FILE__, __FUNCTION__, __LINE__);
		return (int)print_error_string();
	}
	
	// open file
	/*fout = fopen(fname, "a+");
	if (fout == NULL)
	{
		set_error(err_file, __FILE__, __FUNCTION__, __LINE__);
		return (int)print_error_string();
	}

	// print output to file
	fprintf(fout, "%f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
	
	fclose(fout);*/

	DDM_Write_Result(argv, DDM_Get_Total_Time(ddm_timer));
	//DDM_Write_Result(argv, ((float)(end - start)) / CLOCKS_PER_SEC);
	
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
