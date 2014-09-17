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
#include <time.h>


/** \file test_generator.c
\brief Functions for test generation.

This file contains all the functions needed to build test data sets.
*/


/** \brief Generates a data set.

\param out pointer to the structure that is going to store the data set
\param updates number of update extents to be generated
\param subscrs number of subscription extents to be generated
\param dimensions number of dimensions of the problem

\retval error code
*/
_ERR_CODE test_generator(match_data_t *out, const _UINT updates, const _UINT subscrs, const _UINT dimensions)
{
	_UINT i, j;

	if (dimensions < 1)
		return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);

	if (dimensions > MAX_DIMENSIONS)
		return set_error(err_too_many_dim, __FILE__, __FUNCTION__, __LINE__);

	// set the number of dimensions
	out->dimensions = dimensions;

	// set the number of update and subscription extents
	out->size_update = updates;
	out->size_subscr = subscrs;

	// allocate the structures for the update and subscription extents
	out->update = (extent_t *)malloc(updates * sizeof(extent_t));
	out->subscr = (extent_t *)malloc(subscrs * sizeof(extent_t));
	if (out->update == NULL || out->subscr == NULL)
		return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);

	// for each dimension
	for (i = 0; i < dimensions; i++)
	{
		// for each update extent
		for (j = 0; j < updates; j++)
		{
			out->update[j].id = j;
			
			out->update[j].endpoints[i].lower = 0;
			out->update[j].endpoints[i].upper = 0;

			//out->update[j].endpoints[i].lower = 10 * (j + 1) + 1 + ((i + 1) * 50);
			//out->update[j].endpoints[i].upper = 20 * (j + 1) - 1 + ((i + 1) * 50);

			//out->update[j].endpoints[i].lower = ((double)(10 * (j + 1) + 1 + ((i + 1) * 50))) / 10000;
			//out->update[j].endpoints[i].upper = ((double)(20 * (j + 1) - 1 + ((i + 1) * 50))) / 10000;
		}

		// for each subscription extent
		for (j = 0; j < subscrs; j++)
		{
			out->subscr[j].id = j;

			out->subscr[j].endpoints[i].lower = 0;
			out->subscr[j].endpoints[i].upper = 0;
			
			//out->subscr[j].endpoints[i].lower = 15 * (j + 1);
			//out->subscr[j].endpoints[i].upper = 30 * (j + 1);

			//out->subscr[j].endpoints[i].lower = ((double)(15 * (j + 1))) / 10000;
			//out->subscr[j].endpoints[i].upper = ((double)(30 * (j + 1))) / 10000;
		}
	}

	return err_none;
}


/** \brief Generates a random data set.

\param out pointer to the structure that is going to store the data set
\param updates number of update extents to be generated
\param subscrs number of subscription extents to be generated
\param dimensions number of dimensions of the problem

\retval error code
*/
_ERR_CODE test_generator_random(match_data_t *out, const _UINT updates, const _UINT subscrs, const _UINT dimensions, float alfa)
{
	_UINT i, j;
	SPACE_TYPE a, b;
	float segment_size;

	if (dimensions < 1)
		return set_error(err_invalid_input, __FILE__, __FUNCTION__, __LINE__);

	if (dimensions > MAX_DIMENSIONS)
		return set_error(err_too_many_dim, __FILE__, __FUNCTION__, __LINE__);

#ifdef __TRUERAND
	srand((unsigned int)time(NULL));
#endif // __TRUERAND

	// set the number of dimensions
	out->dimensions = dimensions;

	// set the number of update and subscription extents
	out->size_update = updates;
	out->size_subscr = subscrs;

	// allocate the structures for the update and subscription extents
	out->update = (extent_t *)malloc(updates * sizeof(extent_t));
	out->subscr = (extent_t *)malloc(subscrs * sizeof(extent_t));
	if (out->update == NULL || out->subscr == NULL)
		return set_error(err_alloc, __FILE__, __FUNCTION__, __LINE__);

	segment_size = (alfa * LSIZE) / (updates + subscrs);
	
	// for each dimension
	for (i = 0; i < dimensions; i++)
	{
		// for each update extent
		for (j = 0; j < updates; j++)
		{
			out->update[j].id = j;
			a = BIGRANDOM(0, (LSIZE - segment_size));
//			b = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
			b = a + segment_size;

			out->update[j].endpoints[i].lower = MIN(a, b);
			out->update[j].endpoints[i].upper = MAX(a, b);
		}

		// for each subscription extent
		for (j = 0; j < subscrs; j++)
		{
			out->subscr[j].id = j;
//			a = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
//			b = BIGRANDOM(SPACE_TYPE_MIN, SPACE_TYPE_MAX);
			a = BIGRANDOM(0, (LSIZE - segment_size));			
			b = a + segment_size;
			
			out->subscr[j].endpoints[i].lower = MIN(a, b);
			out->subscr[j].endpoints[i].upper = MAX(a, b);
		}
	}

	return err_none;
}
