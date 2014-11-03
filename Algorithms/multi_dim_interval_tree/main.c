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
#include "bitmatrix.h"

extern const char* FILENAME;
extern const char* CFLAGS;

const float Lmax = 1.0e6; /* dimension length */

uint32_t ddm_matching( const struct interval* subscriptions, size_t n,
		       const struct interval* updates, size_t m,
		       struct bitmatrix *mat
 		    );

/**
 * allocates and returns a vector of n random intervals of length
 * |l|. The lower bound of each interval is randomly placed in [0,
 * Lmax-l). The caller owns the returned pointer, and is therefore
 * responsible for freeing the allocated memory.
 */
/*struct interval* mk_random_interval( size_t n, float l )
{
    struct interval* result = (struct interval*)malloc( n * sizeof(result[0]) );
    size_t i;
    for ( i=0; i<n; ++i ) {
	result[i].lower = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	result[i].upper = result[i].lower + l;
	result[i].id = i;
    }
    return result;
}*/

DDM_Extent* mk_random_interval( size_t n, float l, size_t dimensions){
  DDM_Extent *result = (DDM_Extent*)malloc( n * sizeof(result[0]) );
  size_t i, k;
  for ( i=0; i<n; ++i ) {
      result[i].id = i;
      for ( k=0; k<dimensions; ++k ) {
	result[i].lower[k] = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	result[i].upper[k] = result[i].lower[k] + l;
      }
  }
  return result;
}

uint32_t total_matches(struct bitmatrix mat[MAX_DIMENSION], size_t dimensions){

    uint32_t i, j, k;
    uint32_t count = 0;
    int isvalid;

    for (i = 0; i < /*(mat[0].m * */mat[0].n/*)*/; ++i){
      for (j = 0; j < mat[0].m; ++j){
        isvalid = 1;
        for (k = 0; k < dimensions; ++k){
            if (bitmatrix_get(&mat[k], i, j) == 0)
                isvalid = 0;
        }
        if (isvalid)
            count++;
      }
    }
    /*count = 0;
    for (i = 0; i < mat.n; ++i)
      for (j = 0; j < mat.m; ++j)
	if (bitmatrix_get(&mat, i, j) == 1)
	  count++;*/
    return count;
}

int main( int argc, char* argv[] )
{
    size_t tot_extents = 100000;
    size_t d = 1; /* number of dimensions, bust be 1 */
    size_t u; /* updates */
    size_t s; /* subscriptions */
    float alpha = 0.01; /* overlap ratio */
    //struct interval *upds, *subs; 
    DDM_Extent *list_updates, *list_subscriptions;
    DDM_Timer ddm_timer;
    DDM_Input *ddm_input;
    int i, k;
    uint32_t nmatches[MAX_DIMENSION];
    struct bitmatrix mat[MAX_DIMENSION]/*, tmp*/;
    struct interval **l_upds, **l_subs;
    uint32_t tot_match;
    
    ddm_input = DDM_Initialize_Input(argc, argv);
    tot_extents = DDM_Get_Extents(*ddm_input);
    u = DDM_Get_Updates(*ddm_input);
    s = DDM_Get_Subscriptions(*ddm_input);
    d = DDM_Get_Dimensions(*ddm_input); 
    
    if (DDM_Is_Alfa_Test(*ddm_input)){
      /*Alfa Test*/
      alpha = DDM_Get_Alfa(*ddm_input);

      printf("%s compiled with %s\n", argv[0], CFLAGS);
      srandom(clock());
      
      //u = s = tot_extents / 2;
      float l = alpha * Lmax / (float)(u+s);
      list_subscriptions = mk_random_interval( s, l, d );
      list_updates = mk_random_interval( u, l, d );
      
    }else{
      /*Other Kind of Test*/
      
      /*subs = (struct interval*)malloc( s * sizeof(struct interval) );
      upds = (struct interval*)malloc( u * sizeof(struct interval) );*/
      list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
      list_updates = DDM_Get_Updates_List(*ddm_input);
      /*for(i = 0; i < s; i++) //For each subscription extent
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
      }*/
    }
    
    for (i = 0; i < u; i++)
        for (k = 0; k < d; k++)
            printf("upds: id: %zu, lower[%zu]: %lf, upper[%zu]: %lf\n", list_updates[i].id, k, list_updates[i].lower[k], k, list_updates[i].upper[k]);
    for (i = 0; i < s; i++)
        for (k = 0; k < d; k++)
            printf("subs: id: %zu, lower[%zu]: %lf, upper[%zu]: %lf\n", list_subscriptions[i].id, k, list_subscriptions[i].lower[k], k, list_subscriptions[i].upper[k]);
    printf("\n");
    
    //struct timer timing;

    //timing_init( &timing );
    
    //INIT BITMATRIX
    for (i = 0; i < d; ++i){
      bitmatrix_init(&mat[i], s, u);
    }
    /*bitmatrix_init(&mat, s, u);
    bitmatrix_init(&tmp, s, u);*/
    
    //init subs and upds
    l_upds = (struct interval **)malloc(sizeof(struct interval *) * d);
    l_subs = (struct interval **)malloc(sizeof(struct interval *) * d); 
    for (i = 0; i < d; ++i){
      l_upds[i] = (struct interval *)malloc(sizeof(struct interval) * u);
      for (k = 0; k < u; ++k){
	l_upds[i][k].id = list_updates[k].id;
	l_upds[i][k].lower = list_updates[k].lower[i];
	l_upds[i][k].upper = list_updates[k].upper[i];
	printf("upds, id: %d, lower: %lf, upper: %lf\n", l_upds[i][k].id, l_upds[i][k].lower, l_upds[i][k].upper);
	//printf("upds, id: %d, lower: %lf, upper: %lf\n", list_updates[k].id, list_updates[k].lower[i], list_updates[k].upper[i]);
      }
      l_subs[i] = (struct interval *)malloc(sizeof(struct interval) * s);
      for (k = 0; k < s; ++k){
	l_subs[i][k].id = list_subscriptions[k].id;
	l_subs[i][k].lower = list_subscriptions[k].lower[i];
	l_subs[i][k].upper = list_subscriptions[k].upper[i];
	printf("subs, id: %d, lower: %lf, upper: %lf\n", l_subs[i][k].id, l_subs[i][k].lower, l_subs[i][k].upper);
	//printf("subs, id: %d, lower: %lf, upper: %lf\n", list_subscriptions[k].id, list_subscriptions[k].lower[i], list_subscriptions[k].upper[i]);
      }
    }

    DDM_Start_Timer(&ddm_timer);
    //timing_start( &timing );
    for (i = 0; i < d; ++i){
      //if (i == 0) {
	nmatches[i] = ddm_matching( *(l_subs + i), s, *(l_upds + i), u, &mat[i] );
	printf("\nd: %d, match: %d\n", i, nmatches[i]);
      /*}else{
	nmatches[i] = ddm_matching( *(l_subs + i), s, *(l_upds + i), u, &tmp );
	for ( k = 0; k < (s * u); ++k){
	  mat.data[i] &= tmp.data[i];
	}
      }*/
    }
    tot_match = total_matches(mat, d);
    //timing_stop( &timing );
    DDM_Stop_Timer(&ddm_timer);
    //tot_match = total_matches(mat);

    //double total_time = timing_get_average( &timing );

    /* write results */
    //FILE* fout = fopen(FILENAME, "a");
    //if ( fout == NULL ) {
	//printf("Error creating file %s\n", FILENAME);
	//exit(-1);
    //}
    
    DDM_Write_Result(argv, DDM_Get_Total_Time(ddm_timer));
    //DDM_Write_Result(argv, total_time);

    printf("%u matches  %fs\n", tot_match, DDM_Get_Total_Time(ddm_timer) );
    //printf("%u matches  %fs\n", nmatches, total_time );
    
    return 0;
}
