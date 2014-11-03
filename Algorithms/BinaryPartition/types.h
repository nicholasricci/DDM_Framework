/*
    This file is part of DDMTestbed, a free Data Distribution Management benchmark
    Copyright (C) 2013 Francesco Bedini

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __TYPES_H
#define __TYPES_H
/**
* \brief This file contains the data definition
* \author Francesco Bedini
* \file types.h
*/
#include <stdint.h>
#include <inttypes.h>
#include "defines.h"
#include "bitmatrix.h"

#if defined(ARC_64)
/*64 bit processor*/
#define _UINT uint64_t
#define _UINT_MAX UINT64_MAX
#define _INT int64_t
#define _INT_MAX INT64_MAX
#define _INT_MIN INT64_MIN
#define PRI_INT PRId64
#else
/*32 bit processor (or other)*/

#define _UINT uint32_t
#define _UINT_MAX UINT32_MAX
#define _INT int32_t
#define _INT_MAX INT32_MAX
#define _INT_MIN INT32_MIN
#define PRI_INT PRId32
#endif

#define SPACE_TYPE _INT
#define PRI_SPACE_TYPE PRI_INT
#define SPACE_TYPE_MIN	_INT_MIN
#define SPACE_TYPE_MIN_ABS (-1 * _INT_MIN)
#define SPACE_TYPE_MAX	_INT_MAX
#define SPACE_TYPE_RANGE _UINT_MAX //_INT_MAX - _INT_MIN
#define SPACE_TYPE_UNS _UINT

/** \brief The endpoints of an extents on one dimension
 */
typedef struct endpoints{
	SPACE_TYPE lower;/**< The lower endpoint of an extent */
	SPACE_TYPE upper;/**< Te upper endpoint of an extent */
}endpoints;


/** \brief This struct represent an extent
 */
typedef struct extent{
	uint64_t id;    /**< The ID of the extent. It is unique between extents of the same type */
	bool dirtyBit;  /**< This boolean value will be set to true if the extent has been moved in the previous iteration */
	endpoints bounds[MAX_DIMENSIONS]; /**< A vector of endpoints of the size of MAX_DIMENSION */
}extent;


/** \brief A structure used during multistep instances generation
 */
typedef struct waypoint{
    SPACE_TYPE * destination;   /**< The coordinates of this waypoint */
    bool reached;               /**< If true this waypoint has been reached and need to be generated again */
}waypoint;


/** \brief A struct that aggregates all the information about an instance.
 *
 */
typedef struct MatchingInstance{
	uint32_t subscriptions;     /**< The number of subscription extents */
	uint32_t updates;           /**< The number of update extents */
	uint64_t extent_number;     /**< The total number of extents (subscription + updates) */
	extent * extents;           /**< A pointer to an extent data structure */
	bitmatrix * matching_result;/**< A bitmatrix that will store the matching result */
	uint_fast8_t dimensions;    /**< The number of dimensions of this instance */
}MatchingInstance;


/** \brief An enum used to store the movement restrictions
 *
 */
typedef enum {
    RESTR_NONE                   = 0,   /**< Both publishers' and subscribers' extents will be moved */
    RESTR_LOCK_UPDATES           = 1,   /**< Only subscribers' extents will be moved */
    RESTR_LOCK_SUBSCRIPTION      = 2    /**< Only publishers' extents will be moved */
}movement_restriction;


#endif //__TYPES_H
