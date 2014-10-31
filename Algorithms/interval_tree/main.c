/****************************************************************************
 *
 * main.c
 *
 * This file is part of DDM
 *
 * Copyright (C) 2013 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "timing.h"
#include "interval.h"
#include "cflags.h"
#include "DDM_input_output.h"

extern const char* FILENAME;
extern const char* CFLAGS;

const float Lmax = 1.0e6; /* dimension length */

uint32_t ddm_matching( const struct interval* subscriptions, size_t n,
		       const struct interval* updates, size_t m );

/**
 * allocates and returns a vector of n random intervals of length
 * |l|. The lower bound of each interval is randomly placed in [0,
 * Lmax-l). The caller owns the returned pointer, and is therefore
 * responsible for freeing the allocated memory.
 */
struct interval* mk_random_interval( size_t n, float l )
{
    struct interval* result = (struct interval*)malloc( n * sizeof(result[0]) );
    size_t i;
    for ( i=0; i<n; ++i ) {
	result[i].lower = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	result[i].upper = result[i].lower + l;
	result[i].id = i;
    }
    return result;
}

int main( int argc, char* argv[] )
{
    size_t tot_extents = 100000;
    size_t d = 1; /* number of dimensions, bust be 1 */
    size_t u; /* updates */
    size_t s; /* subscriptions */
    float alpha = 0.01; /* overlap ratio */
    struct interval *upds, *subs; 
    DDM_Extent *list_updates, *list_subscriptions;
    DDM_Timer ddm_timer;
    DDM_Input *ddm_input;
    int i;

    if ( (argc == 2 && strcmp(argv[1], "--help") == 0) ) {
	printf("Usage: %s <tot extents> <dimensions> <alpha>\n", argv[0]);
	return -1;
    }
    
    ddm_input = DDM_Initialize_Input(argc, argv);
    tot_extents = DDM_Get_Extents(*ddm_input);
    u = DDM_Get_Updates(*ddm_input);
    s = DDM_Get_Subscriptions(*ddm_input);
    d = DDM_Get_Dimensions(*ddm_input); 
    if ( d != 1 ) {
	printf("This program only supports 1 dimension\n");
	exit(-1);
    }
    
    if (DDM_Is_Alfa_Test(*ddm_input)){
      /*Alfa Test*/
      alpha = DDM_Get_Alfa(*ddm_input);

      printf("%s compiled with %s\n", argv[0], CFLAGS);
      srandom(clock());
      
      //u = s = tot_extents / 2;
      float l = alpha * Lmax / (float)(u+s);
      upds = mk_random_interval( u, l );
      subs = mk_random_interval( s, l );
      
    }else{
      /*Other Kind of Test*/
      
      subs = (struct interval*)malloc( s * sizeof(struct interval) );
      upds = (struct interval*)malloc( u * sizeof(struct interval) );
      list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
      list_updates = DDM_Get_Updates_List(*ddm_input);
      for(i = 0; i < s; i++) //For each subscription extent
      {
	subs[i].id = list_subscriptions[i].id;
	subs[i].lower = list_subscriptions[i].lower[0];
	subs[i].upper = list_subscriptions[i].upper[0];
      }
      for(i = 0; i < u; i++)
      {
	upds[i].id = list_updates[i].id;
	upds[i].lower = list_updates[i].lower[0];
	upds[i].upper = list_updates[i].upper[0];
      }
    }
    
    //struct timer timing;

    //timing_init( &timing );

    DDM_Start_Timer(&ddm_timer);
    //timing_start( &timing );
    uint32_t nmatches = ddm_matching( subs, s, upds, u );
    //timing_stop( &timing );
    DDM_Stop_Timer(&ddm_timer);

    //double total_time = timing_get_average( &timing );

    /* write results */
    //FILE* fout = fopen(FILENAME, "a");
    //if ( fout == NULL ) {
	//printf("Error creating file %s\n", FILENAME);
	//exit(-1);
    //}
    
    DDM_Write_Result(argv, DDM_Get_Total_Time(ddm_timer));
    //DDM_Write_Result(argv, total_time);

    printf("%u matches  %fs\n", nmatches, DDM_Get_Total_Time(ddm_timer) );
    //printf("%u matches  %fs\n", nmatches, total_time );
    
    return 0;
}
