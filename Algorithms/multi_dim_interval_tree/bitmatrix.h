/****************************************************************************
 *
 * bitmatrix.h
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
#ifndef BITMATRIX_H
#define BITMATRIX_H

#include <stdlib.h>
#include <stdint.h>

struct bitmatrix {
    size_t n; /* number of rows */
    size_t m; /* number of words per row; the number of columns of the bitmatrix is therefore m*8*sizeof(uint32_t) */
    uint32_t* data; /* array of data elements */
};

/**
 * Creates a n*m bit matrix; all elements are initialized to 0
 */
void bitmatrix_init(struct bitmatrix* mat, size_t n, size_t m);

/**
 * Deallocates a bit matrix
 */ 
void bitmatrix_free(struct bitmatrix* mat);

/**
 * Set m[i,j] = 0 if val == 0, set m[i,j] = 1 otherwise
 */ 
void bitmatrix_set(struct bitmatrix* mat, size_t i, size_t j, int val);

/**
 * Returns the value of m[i,j]
 */
int bitmatrix_get(const struct bitmatrix* mat, size_t i, size_t j);

#endif
