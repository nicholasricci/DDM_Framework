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

#include "../include/error.h"

#include <stdlib.h>


/** \file utils.c
\brief File containing various utilities functions.
*/


/** \brief Allocates a bit matrix of given height and width.

\param out pointer to the memory to be allocated
\param size_update the height of the matrix
\param size_subscr the \i minimum width in bits of the matrix

\retval error code
*/
_ERR_CODE create_bit_matrix(bitmatrix *out, const _UINT size_update, const _UINT size_subscr)
{
	_UINT i;
	bitvector vec;
	_UINT line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(size_subscr);

	// allocate one big vector and a column vector
#ifdef __LOWMEM
	vec = (bitvector)calloc(line_width * size_update, sizeof(bitvec_elem));
#else // __LOWMEM
	vec = (bitvector)malloc(line_width * size_update * sizeof(bitvec_elem));
#endif // __LOWMEM
	*out = (bitmatrix)malloc(size_update * sizeof(bitvector));
	if (vec == NULL || *out == NULL)
		return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);

	// set each element of the column vector to point to the first element of each line in the big vector
	for (i = 0; i < size_update; i++)
		(*out)[i] = &vec[i * line_width];

	return err_none;
}


/** \brief Bitwise NOT of a bit vector.

It can also be used to do the bitwise NOT of the matrix, since it's allocated as linear memory.

\param mat bit vector to be inverted
\param size number of elements of the bit vector
*/
void vector_bitwise_not(const bitvector vec, const _UINT size)
{
	_UINT i;

	for (i = 0; i < size; i++)
		vec[i] = ~vec[i];
}


/** \brief Bitwise AND of two bit vectors.

It can also be used to do the bitwise AND of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_and(const bitvector result, const bitvector mask, const _UINT size)
{
	_UINT i;

	for (i = 0; i < size; i++)
		result[i] &= mask[i];
}


/** \brief Bitwise OR of two bit vectors.

It can also be used to do the bitwise OR of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_or(const bitvector result, const bitvector mask, const _UINT size)
{
	_UINT i;

	for (i = 0; i < size; i++)
		result[i] |= mask[i];
}


/** \brief Fills the endpoints list with the values for a given dimension.

\param data the data set.
\param out the pointer to the list to be filled
\param dimension the number of the dimension to be processed
*/
void set_endpoints_list(const match_data_t data, const list_ptr out, const _UINT dimension)
{
	_UINT i, count;

	count = 0;

	// for each subscription extent
	for (i = 0; i < data.size_subscr; i++)
	{
		out[count].id = i;
		out[count].is_lower_point = TRUE;

#ifdef __SUPERSET
		if (data.subscr[i].endpoints[dimension].lower > SPACE_TYPE_MIN)
			out[count++].point = data.subscr[i].endpoints[dimension].lower - SPACE_TYPE_INC;
		else
#endif // __SUPERSET
			out[count++].point = data.subscr[i].endpoints[dimension].lower;

		out[count].id = i;
		out[count].is_lower_point = FALSE;

#ifdef __SUPERSET
		if (data.subscr[i].endpoints[dimension].upper < SPACE_TYPE_MAX)
			out[count++].point = data.subscr[i].endpoints[dimension].upper + SPACE_TYPE_INC;
		else
#endif // __SUPERSET
			out[count++].point = data.subscr[i].endpoints[dimension].upper;
	}

	// for each update extent
	for (i = 0; i < data.size_update; i++)
	{
		// IDs of update extents in the "list" follow the IDs of subscription extents
		// this way is possible to distinguish subscription and update extents without having to store another variable
		out[count].id = i + data.size_subscr;
		out[count].is_lower_point = TRUE;

#ifdef __SUPERSET
		if (data.update[i].endpoints[dimension].lower > SPACE_TYPE_MIN)
			out[count++].point = data.update[i].endpoints[dimension].lower - SPACE_TYPE_INC;
		else
#endif // __SUPERSET
			out[count++].point = data.update[i].endpoints[dimension].lower;

		out[count].id = i + data.size_subscr;
		out[count].is_lower_point = FALSE;

#ifdef __SUPERSET
		if (data.update[i].endpoints[dimension].upper < SPACE_TYPE_MAX)
			out[count++].point = data.update[i].endpoints[dimension].upper + SPACE_TYPE_INC;
		else
#endif // __SUPERSET
			out[count++].point = data.update[i].endpoints[dimension].upper;
	}
}


/** \brief Rule for qsort() ordering.

\remarks If two extents with zero-width have the same coordinates, they should overlap.

\param a pointer to the first element to compare
\param b pointer to the second element to compare

\retval -1 if a < b or a == b and a is lower point
\retval 1 if a > b or a == b and a is upper point
*/
_INT compare_endpoints(const void *a, const void *b)
{
	SPACE_TYPE x = (*(list_ptr)a).point;
	SPACE_TYPE y = (*(list_ptr)b).point;

#ifdef __SUPERSET
	return (x < y) ? -1 : (x > y) ? 1 : 0;
#else // __SUPERSET
	return (x < y) ? -1 : (x > y) ? 1 : ((*(list_ptr)a).is_lower_point) ? -1 : 1;
#endif // __SUPERSET
}


/** \brief Sorting function.

This function only performs a call to stdlib.h's qsort() function.

\param ep_list the endpoints list to be ordered
\param size the size of the list
*/
INLINE void sort_list(const list_ptr ep_list, const _UINT size)
{
	qsort(ep_list, size, sizeof(list_t), compare_endpoints);
}


#ifdef __VERBOSE
/** \brief Printing function.

This function prints the bit matrix.

\param in the bit matrix to be printed
\param size_update the number of update extents (height of the matrix)
\param size_subscr the number of subscription extents (\i minimum width in bits of the matrix)
*/
#include <stdio.h>
void print_bitmatrix(const bitmatrix in, const _UINT size_update, const _UINT size_subscr)
{
	_UINT i, j, bit;
	_BYTE element[BITVEC_ELEM_BITS + 1];
	bitvec_elem val;
	bitvec_elem mask;
	_UINT line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(size_subscr);

	// escape the string
	element[BITVEC_ELEM_BITS] = '\0';

	// for each line (update extent)
	for (i = 0; i < size_update; i++)
	{
		// for each element in the line
		for (j = 0; j < line_width; j++)
		{
			mask = BITVEC_ELEM_MAX_BIT;
			val = in[i][j];

			// for each bit in the element
			for (bit = 0; bit < BITVEC_ELEM_BITS; bit++)
			{
				// if the actual bit is TRUE, set the char to '1', else set it to '0'
				element[bit] = (val & mask ? '1' : '0');
				// shift right
				mask >>= 1;
			}

			// print the string
			printf("%s ", element);
		}

		// new line
		printf("\n");
	}
}
#endif // __VERBOSE
