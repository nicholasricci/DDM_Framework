#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "DDM_input_output.h"
#include "cflags.h"
#include "interval.h"

extern const char* FILENAME;
extern const char* CFLAGS;

const float Lmax = 1.0e6; /* dimension length */

uint32_t ddm_matching( const struct interval* subscriptions, size_t n,
		       const struct interval* updates, size_t m,
		       size_t dimensions );

/**
 * allocates and returns a vector of n random intervals of length
 * |l|. The lower bound of each interval is randomly placed in [0,
 * Lmax-l). The caller owns the returned pointer, and is therefore
 * responsible for freeing the allocated memory.
 */
struct interval* mk_random_interval( size_t n, float l, size_t dimensions )
{
    struct interval* result = (struct interval*)malloc( n * sizeof(result[0]) );
    size_t i;
    int k;
    for ( i=0; i<n; ++i ) {
      result[i].id = i;
      for ( k=0; k<dimensions; ++k){
	result[i].lower[k] = (Lmax-l)*random() / ((float)RAND_MAX + 1);
	result[i].upper[k] = result[i].lower[k] + l;
      }
    }
    return result;
}

int main(int argc, char *argv[]){
  size_t tot_extents = 100000;
    size_t d = 1; /* number of dimensions, bust be 1 */
    size_t u; /* updates */
    size_t s; /* subscriptions */
    float alpha = 0.01; /* overlap ratio */
    struct interval *upds, *subs; 
    DDM_Extent *list_updates, *list_subscriptions;
    DDM_Timer ddm_timer;
    DDM_Input *ddm_input;
    int i, k;

    if ( (argc == 2 && strcmp(argv[1], "--help") == 0) ) {
	printf("Usage: %s <tot extents> <dimensions> <alpha>\n", argv[0]);
	return -1;
    }
    
    ddm_input = DDM_Initialize_Input(argc, argv);
    tot_extents = DDM_Get_Extents(*ddm_input);
    u = DDM_Get_Updates(*ddm_input);
    s = DDM_Get_Subscriptions(*ddm_input);
    d = DDM_Get_Dimensions(*ddm_input); 
    /*if ( d != 1 ) {
	printf("This program only supports 1 dimension\n");
	exit(-1);
    }*/
    
    if (DDM_Is_Alfa_Test(*ddm_input)){
      /*Alfa Test*/
      alpha = DDM_Get_Alfa(*ddm_input);

      printf("%s compiled with %s\n", argv[0], CFLAGS);
      srandom(clock());
      
      //u = s = tot_extents / 2;
      float l = alpha * Lmax / (float)(u+s);
      upds = mk_random_interval( u, l, d );
      subs = mk_random_interval( s, l, d );
      
    }else{
      /*Other Kind of Test*/
      
      subs = (struct interval*)malloc( s * sizeof(struct interval) );
      upds = (struct interval*)malloc( u * sizeof(struct interval) );
      list_subscriptions = DDM_Get_Subscriptions_List(*ddm_input);
      list_updates = DDM_Get_Updates_List(*ddm_input);
      for(i = 0; i < s; i++) //For each subscription extent
      {
	subs[i].id = list_subscriptions[i].id;
	for (k = 0; k < d; ++k){
	  subs[i].lower[k] = list_subscriptions[i].lower[k];
	  subs[i].upper[k] = list_subscriptions[i].upper[k];
	}
      }
      for(i = 0; i < u; i++)
      {
	upds[i].id = list_updates[i].id;
	for (k = 0; k < d; ++k){
	  upds[i].lower[k] = list_updates[i].lower[k];
	  upds[i].upper[k] = list_updates[i].upper[k];
	}
      }
    }
    
    //struct timer timing;

    //timing_init( &timing );

    DDM_Start_Timer(&ddm_timer);
    //timing_start( &timing );
    uint32_t nmatches = ddm_matching( subs, s, upds, u, d );
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
