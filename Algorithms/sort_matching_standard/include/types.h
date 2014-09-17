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


#ifndef __TYPES_H
#define __TYPES_H


#include <stdint.h>

#include "defines.h"


/** \file types.h
\brief Data types definitions.

This file contains all the data types definitions. The simple types are to avoid different data sizes on different platforms.
*/


/* Data types */
#define _BOOL		uint_fast8_t
#define _BYTE		uint8_t
#define _UINT		uint32_t
#define _INT		int32_t


/** \brief Data type of endpoints coordinates.
*/
#define SPACE_TYPE_SELECT	4
/*
 * 1	int32_t
 * 2	int64_t
 * 3	float
 * 4	double
*/


/* Defines min, max and minimum increment of the active SPACE_TYPE */
#if SPACE_TYPE_SELECT == 1
#define SPACE_TYPE		int32_t
#define SPACE_TYPE_MIN	INT32_MIN
#define SPACE_TYPE_MAX	INT32_MAX
#define SPACE_TYPE_INC	1
#elif SPACE_TYPE_SELECT == 2
#define SPACE_TYPE		int64_t
#define SPACE_TYPE_MIN	INT64_MIN
#define SPACE_TYPE_MAX	INT64_MAX
#define SPACE_TYPE_INC	1
#elif SPACE_TYPE_SELECT == 3
#include <float.h>
#define SPACE_TYPE		float
#define SPACE_TYPE_MIN	FLT_MIN
#define SPACE_TYPE_MAX	FLT_MAX
#define SPACE_TYPE_INC	FLT_EPSILON
#elif SPACE_TYPE_SELECT == 4
#include <float.h>
#define SPACE_TYPE		double
#define SPACE_TYPE_MIN	FLT_MIN
#define SPACE_TYPE_MAX	FLT_MAX
#define SPACE_TYPE_INC	DBL_EPSILON
#endif // SPACE_TYPE


/** \brief An element of the bit vector used for storing the matches.
*/
typedef uint32_t		bitvec_elem;


/** \brief The bit vector used for storing the matches.

The bit vector is generally used only for allocating the bit matrix.
*/
typedef bitvec_elem*	bitvector;


/** \brief The bit matrix used for storing the matches.

The lines of the bit matrix correspond to the update extents, while the single bits on each line represent the subscription extents.
*/
typedef bitvector*		bitmatrix;


/** \brief The endpoints of an extent in a given dimension.
*/
typedef struct
{
	SPACE_TYPE	lower;		///< lower bound point
	SPACE_TYPE	upper;		///< upper bound point
} endpoints_t;


/** \brief The structure representing an extent.

Each element of the array represents a dimension.
*/
typedef struct
{
	_UINT		id;								///< identifier of the extent
	endpoints_t endpoints[MAX_DIMENSIONS];		///< array containing the endpoints of the extent for each dimension
} extent_t;


/** \brief The problem data set.

This structures contains all the data needed for the matching algorithm to check the matches.
*/
typedef struct
{
	_UINT		dimensions;			///< number of dimensions

	extent_t	*update;			///< array containing update extents data
	_UINT		size_update;		///< number of update extents
	
	extent_t	*subscr;			///< array containing subscription extents data
	_UINT		size_subscr;		///< number of subscription extents
} match_data_t;


/** \brief An element of the list of endpoints.

The identifier goes from 0 to size_subscr - 1 for subscriptions and from size_subscr to size_subscr + size_update - 1 for updates.
*/
typedef struct
{
	_UINT		id;					///< identifier of the extent this endpoint belongs to
	_BOOL		is_lower_point;		///< is this the lower bound point of the extent?
	SPACE_TYPE	point;				///< position of the point in space
} list_t;


/** \brief Type definition for list_t pointer.
*/
typedef list_t* list_ptr;


/** \brief Enum for error codes.
*/
typedef enum 
{
	err_none					= 0,
	err_unhandled				= 1,
	err_generic					= 2,
	err_alloc					= 3,
	err_file					= 4,
	err_invalid_input			= 5,
	err_too_many_dim			= 6,
	err_threads					= 7,
	err_opencl					= 8,
	err_opencl_device_not_found	= 9,
	err_opencl_file				= 10
} _ERR_CODE;


/** \brief Structure containing error data.
*/
typedef struct
{
	_ERR_CODE code;						///< error code
	char file[FILE_NAME_SIZE];			///< file who generated the error
	char function[FUNCTION_NAME_SIZE];	///< function who generated the error
	_UINT line;							///< line of the file who generated the error
} _err_t;


#endif // __TYPES_H
