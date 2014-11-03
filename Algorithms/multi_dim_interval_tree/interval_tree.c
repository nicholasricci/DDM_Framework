/****************************************************************************
 *
 * interval_tree.c
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
#include "bitmatrix.h"

const char* FILENAME = "interval_tree.txt";

int set_matrix_callback( const struct interval* x, const struct interval* q, void* param )
{
    struct bitmatrix* mat = (struct bitmatrix*)param;
    bitmatrix_set( mat, x->id, q->id, 1 );
    return 0;
}

uint32_t ddm_matching( const struct interval* sub, size_t n,
		       const struct interval* upd, size_t m,
		       struct bitmatrix *mat)
{
    /*struct bitmatrix mat;
    bitmatrix_init(&mat, n, m);*/
    uint32_t result = 0;
    size_t i, j;
    struct int_tree tree;
    int_tree_init( &tree );
    for (i=0; i<n; ++i) {
	int_tree_insert( &tree, &sub[i] );
    }

#pragma omp parallel for reduction(+:result)
    for ( j=0; j<m; ++j) {
	result += int_tree_find_intersect( &tree, &upd[j], set_matrix_callback, mat );
    }
    /* we do not need to destroy the tree, since our test program
       exists right after this function. If this is not the case, you
       should call int_tree_clear( &tree ) here */
    return result;
}
