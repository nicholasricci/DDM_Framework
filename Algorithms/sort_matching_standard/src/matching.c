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

#include "../include/utils.h"
#include "../include/error.h"

#include <stdlib.h>
#include <string.h>


/** \file matching.c
\brief File containing the main matching algorithm functions.
*/


/** \brief One-dimensional matching.

This function performs the sort matching on a single dimension.

\param ep_list the endpoints list
\param out the matrix that is going to keep the result of the matching
\param subscr_set_before the array to be used as the the set of "before" subscriptions
\param subscr_set_after the array to be used as the the set of "after" subscriptions
\param size_update the number of update extents
\param size_subscr the number of subscription extents
*/
void sort_matching_1D(const list_ptr ep_list, const bitmatrix out, const bitvector subscr_set_before, const bitvector subscr_set_after, const _UINT size_update, const _UINT size_subscr)
{
	_UINT i;
	_UINT bit_pos;
	_UINT line_width;
	_UINT list_size;
	_UINT update_ep_count;

	// two endpoints for each extent
	list_size = (size_update + size_subscr) * 2;
	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(size_subscr);
	// number of endpoints of update extents
	update_ep_count = size_update * 2;

	// sort the endpoints list
	sort_list(ep_list, list_size);

	// set no subscription extent to "before"
	memset(subscr_set_before, 0x00, line_width * sizeof(bitvec_elem));
	// set all the subscription extents to "after"
	memset(subscr_set_after, 0xFF, line_width * sizeof(bitvec_elem));

	// for each endpoint in the list, but stops when all update extents endpoints are processed
	// (only when processing update extents endpoints the algorithm writes on the matching matrix)
	for (i = 0; update_ep_count > 0; i++)
	{
		// if it's the endpoint of a subscription extent
		if (ep_list[i].id < size_subscr)
		{
			// calculate the element in the bit vector that contains the bit
			bit_pos = BIT_TO_POS(ep_list[i].id);
			
			// if it's the lower endpoint
			if (ep_list[i].is_lower_point)
			{
				// clear the bit in the bit vector (remove the subscription extent from the "after" set)
				BIT_CLEAR(subscr_set_after[bit_pos], DBIT(BIT_POS_IN_VEC(ep_list[i].id, bit_pos)));
			}
			else // if it's the upper endpoint
			{
				// set the bit in the bit vector (add the subscription extent to the "before" set)
				BIT_SET(subscr_set_before[bit_pos], DBIT(BIT_POS_IN_VEC(ep_list[i].id, bit_pos)));
			}
		}
		else // if it's the endpoint of an update extent
		{
			update_ep_count--;
			// if it's the lower endpoint
			if (ep_list[i].is_lower_point)
			{
#ifdef __LOWMEM
				// bitwise OR (write all the subscription extents in the "before" set in the update extent's line in the bit matrix)
				vector_bitwise_or(out[ep_list[i].id - size_subscr], subscr_set_before, line_width);
#else // __LOWMEM
				// the subscription extents in the "before" set don't match with this update extent
				// (write the bits of the "before" set in the update extent's line in the bit matrix)
				memcpy(out[ep_list[i].id - size_subscr], subscr_set_before, line_width * sizeof(bitvec_elem));
#endif // __LOWMEM
			}
			else // if it's the upper endpoint
			{
				// bitwise OR (write all the subscription extents in the "after" set in the update extent's line in the bit matrix)
				vector_bitwise_or(out[ep_list[i].id - size_subscr], subscr_set_after, line_width);
			}
		}
	}
}


/** \brief Main algorithm function.

This function performs all the operations needed to feed the data one dimension at a time to the matching_1D function.

\param data the data set
\param out the output bit matrix

\retval error code
*/
_ERR_CODE sort_matching(const match_data_t data, const bitmatrix out)
{
	_UINT i;
	_UINT list_size;
	_UINT line_width;
	_UINT matrix_size;
	list_ptr ep_list;
	bitvector subscr_set_before;
	bitvector subscr_set_after;
#ifndef __LOWMEM
	bitmatrix result_tmp;
	_ERR_CODE err;
#endif // __LOWMEM

	line_width = BIT_VEC_WIDTH(data.size_subscr);
	matrix_size = data.size_update * line_width;

	if (data.dimensions < 1)
		return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);

	if (data.dimensions > MAX_DIMENSIONS)
		return set_error(err_too_many_dim, __FILE__, __FUNCTION__, __LINE__);

#ifndef __LOWMEM
	if (data.dimensions > 1)
	{
		// if more than one dimension, a temporary bit matrix is needed to store the single dimensions results
		err = create_bit_matrix(&result_tmp, data.size_update, data.size_subscr);
		if (err != err_none)
			return err;
	}
#endif // __LOWMEM

	// two endpoints for each extent
	list_size = (data.size_update + data.size_subscr) * 2;

	// allocate the "list"
	ep_list = (list_ptr)malloc(list_size * sizeof(list_t));
	
	// allocate the two subscription extents sets
	subscr_set_before = (bitvector)malloc(line_width * sizeof(bitvec_elem));
	subscr_set_after = (bitvector)malloc(line_width * sizeof(bitvec_elem));
	
	if (ep_list == NULL || subscr_set_before == NULL || subscr_set_after == NULL)
		return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);
	
	// for each dimension
	for (i = 0; i < data.dimensions; i++)
	{
		// fill the endpoints "list" with the data of the dimension to be processed
		set_endpoints_list(data, ep_list, i);

#ifdef __LOWMEM
		sort_matching_1D(ep_list, out, subscr_set_before, subscr_set_after, data.size_update, data.size_subscr);
#else // __LOWMEM
		// perform the sort matching on the actual dimension (directly on 'out' for the first dimension)
		sort_matching_1D(ep_list, (i > 0) ? result_tmp : out, subscr_set_before, subscr_set_after, data.size_update, data.size_subscr);

		// bitwise NOT of the non-matching table to obtain the matching table
		// directly on matrix 'out' for the first dimension, following times on 'result_tmp'
		vector_bitwise_not((i > 0) ? result_tmp[0] : out[0], matrix_size);
			
		// if it's not the first dimension
		if (i > 0)
		{
			// bitwise AND of the matching table
			vector_bitwise_and(out[0], result_tmp[0], matrix_size);
		}
#endif // __LOWMEM
	}

#ifndef __NOFREE
	// free memory
	free(ep_list);
	free(subscr_set_before);
	free(subscr_set_after);
#ifndef __LOWMEM
	free(*result_tmp);
	free(result_tmp);
#endif // __LOWMEM
#endif // __NOFREE

#ifdef __LOWMEM
	// bitwise NOT of the non-matching table to obtain the matching table
	vector_bitwise_not(out[0], matrix_size);
#endif // __LOWMEM
	
	return err_none;
}
