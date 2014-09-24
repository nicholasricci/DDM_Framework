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

    if ( (argc == 2 && strcmp(argv[1], "--help") == 0) ) {
	printf("Usage: %s <tot extents> <dimensions> <alpha>\n", argv[0]);
	return -1;
    }

    if ( argc >= 2 ) 
	tot_extents = atoi(argv[1]);

    if ( argc >= 3 ) {
	d = atoi(argv[2]);
	if ( d != 1 ) {
	    printf("This program only supports 1 dimension\n");
	    exit(-1);
	}
    }

    if ( argc >= 4 )
	alpha = atof(argv[3]);

    printf("%s compiled with %s\n", argv[0], CFLAGS);
    srandom(clock());

    u = s = tot_extents / 2;
    float l = alpha * Lmax / (float)(u+s);
    struct interval* upds = mk_random_interval( u, l );
    struct interval* subs = mk_random_interval( s, l );
    struct timer timing;

    timing_init( &timing );

    timing_start( &timing );
    uint32_t nmatches = ddm_matching( subs, s, upds, u );
    timing_stop( &timing );

    double total_time = timing_get_average( &timing );

    /* write results */
    //FILE* fout = fopen(FILENAME, "a");
    //if ( fout == NULL ) {
	//printf("Error creating file %s\n", FILENAME);
	//exit(-1);
    //}
    
    /* write results */
    char str[80];
    strcpy(str, argv[0]);
    strcat(str, ".txt");
    FILE* fout = fopen(str, "a");
    if ( fout == NULL ) {
	printf("Error creating file %s\n", str);
	exit(-1);
    }

    fprintf(fout, "%f\n", total_time);
    fclose(fout);

    printf("%u matches  %fs\n", nmatches, total_time );

    return 0;
}
