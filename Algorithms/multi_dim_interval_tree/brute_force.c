/****************************************************************************
 *
 * brute_force.c
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

#include <stdlib.h>
#include <stdint.h>
#include "interval.h"
#include "bitmatrix.h"

const char* FILENAME = "brute_force.txt";

/**
 * Brute force approach: test each update against each subscription.
 * Returns the number of (upd, subs) matches.
 */
uint32_t ddm_matching( const struct interval* sub, size_t n,
		       const struct interval* upd, size_t m,
		       struct bitmatrix *mat
 		    )
{
    uint32_t result = 0;
    size_t i, j;
    /*struct bitmatrix mat;
    bitmatrix_init( &mat, n, m );*/

#pragma omp parallel for private(j) reduction(+:result)
    for (i=0; i<n; ++i) {
	for (j=0; j<m; ++j) {
	    if ( intersect( &sub[i], &upd[j] ) ) {
		bitmatrix_set(&mat, i, j, 1);
		result++;
	    }
	}
    }

    return result;
}
