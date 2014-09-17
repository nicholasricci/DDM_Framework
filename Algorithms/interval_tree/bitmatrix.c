/****************************************************************************
 *
 * bitmatrix.c
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
#include "bitmatrix.h"
#include <assert.h>

#define BITS_PER_WORD (8*sizeof(uint32_t))

void bitmatrix_init( struct bitmatrix* mat, size_t n, size_t m)
{
    mat->n = n;
    mat->m = (m + BITS_PER_WORD - 1) / BITS_PER_WORD; /* round the number m of columns to the next multiple of BITS_PER_WORD */
    mat->data = (uint32_t*)calloc(mat->m * mat->n, sizeof(uint32_t));
    assert( mat->data );
}

void bitmatrix_free( struct bitmatrix* mat )
{
    mat->m = mat->n = 0;
    free( mat->data );
}

void bitmatrix_set(struct bitmatrix* mat, size_t i, size_t j, int val)
{
    assert( i < mat->n );
    assert( j < mat->m * BITS_PER_WORD );
    uint32_t bitval = 0x8000000 >> (j % BITS_PER_WORD);
    bitval *= (val != 0);
    uint32_t idx = i * mat->m + j / BITS_PER_WORD;
    mat->data[idx] = ( mat->data[idx] & ~bitval ) | bitval;
}

int bitmatrix_get(const struct bitmatrix* mat, size_t i, size_t j)
{
    assert( i < mat->n );
    assert( j < mat->m * BITS_PER_WORD );
    uint32_t mask = 0x8000000 >> (j % BITS_PER_WORD);
    uint32_t idx = i * mat->m + j / BITS_PER_WORD;
    
    return ( ( mat->data[idx] & mask ) > 0 );
}
