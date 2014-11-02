/****************************************************************************
 *
 * intersect_omp.c
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
#include <assert.h>
#include <string.h>
#include <time.h>
#include "int_tree.h"
#include "timing.h"

/**
 * Brute force approach: test each update against each subscription.
 * Returns the number of (upd, subs) matches.
 */
size_t count_intersections( const struct interval* subscriptions, size_t m,
			    const struct interval* updates, size_t n )
{
    size_t result = 0;
    const struct interval* i;
    const struct interval* j;
#pragma omp parallel for private(i) reduction(+:result)
    for ( i = subscriptions; i < subscriptions + m; ++i ) {
	for ( j = updates; j < updates + n; ++j ) {
	    result += intersect( i, j );
	}
    }
    return result;
}

/**
 * allocates and returns a vector of n randomly initialized
 * intervals. The caller owns the returned pointer, and is therefore
 * responsible for freeing the allocated memory.
 */
struct interval* mk_random_interval( size_t n )
{
    struct interval* result = (struct interval*)malloc( n * sizeof(result[0]) );
    int i;
    for ( i=0; i<n; ++i ) {
	result[i].lower = 10000.0*random() / (float)RAND_MAX;
	result[i].upper = result[i].lower + 50.0*random() / (float)RAND_MAX;
	result[i].id = i;
    }
    return result;
}

/**
 * Count all intersections among intervals stored in the interval tree
 * |subscriptions| and those contained in the n-element vector |upd|.
 */
size_t count_intersections_avl( const struct int_tree* subscriptions,
				const struct interval* upd, size_t n )
{
    size_t result = 0;
    const struct interval* u;
#pragma omp parallel for private(u) schedule(dynamic) reduction(+:result)
    for ( u = upd; u < upd + n; ++u ) {
	size_t count = int_tree_count_intersect( subscriptions, u );
	result += count;
    }
    return result;
}

/**
 * Print n intervals stored in vector c. For debugging purposes only.
 */
void print_intervals( const struct interval* v, size_t n )
{
    size_t i = 0;
    for ( i=0; i<n; ++i ) {
	printf("(%3d) %f -- %f\n",v[i].id, v[i].lower, v[i].upper );
    }
}

int main( void )
{
    const size_t s = 250000;
    const size_t u = 250000;
    size_t i, count_bruteforce, count_avl;
    struct timer timing_bruteforce, timing_create_avl, timing_query_avl;

    /* initialize timers */
    timing_init( &timing_bruteforce );
    timing_init( &timing_create_avl );
    timing_init( &timing_query_avl );

    srandom( 19 );

    struct interval* upds = mk_random_interval( u );
    struct interval* subs = mk_random_interval( s );

    timing_start( &timing_bruteforce );
    count_bruteforce = count_intersections( subs, s, upds, u );
    timing_stop( &timing_bruteforce );

    printf("Bruteforce: %u matches, %.3fs\n", count_bruteforce, timing_get_average( &timing_bruteforce ) );

    /* Create an interval tree with the subscriptions */
    timing_start( &timing_create_avl );
    struct int_tree* tree = int_tree_create();
    for (i=0; i<s; ++i) {
	int_tree_insert( tree, &subs[i] );
    }
    timing_stop( &timing_create_avl );

    printf("Tree height = %u, size = %u\n", tree->root->height, tree->size );
    timing_start( &timing_query_avl );
    count_avl = count_intersections_avl( tree, upds, u );
    timing_stop( &timing_query_avl );
    printf("AVL       : %u matches, init = %.3fs, query = %.3fs\n", count_avl, timing_get_average( &timing_create_avl ), timing_get_average( &timing_query_avl ) );

    return 0;
}
