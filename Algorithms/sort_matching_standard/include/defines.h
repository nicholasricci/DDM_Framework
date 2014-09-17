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


#ifndef __DEFINES_H
#define __DEFINES_H


/** \file defines.h
\brief Defines and macros.

This file contains all the value defines and macros used by the algorithm.
*/


/** \brief Verbose define.

Prints the result matrix.
*/
//#define __VERBOSE


/** \brief Define for debug.
*/
//#define __DEBUG


/** \brief Define for testing.

If this is defined the program outputs algorithm's elapsed time.
*/
#define __TEST

/** \brief Define for fast close.

If this is defined the program skips all the free() calls. This should be the default behaviour if the program is doing a single matching iteration, since the freeing of memory should be delegated to the OS.
*/
#define __NOFREE


/**	\brief Define for superset version.

If this is defined the algorithm would create a superset, increasing the extents size.
*/
//#define __SUPERSET


/**	\brief Define for low memory version.

If this is defined the algorithm would use only a single bit matrix instead of two even if the problem has more than one dimension.
*/
#define __LOWMEM


/** \brief Define for enabling the creation of a random data set.

If this is undefined the program would generate a fixed data set.
*/
#define __RANDOM_SET


/** \brief Define for enabling the seeding change of rand().

If this is undefined the program would generate the same data set each time.
*/
#define __TRUERAND


/** \brief Macro for inline */
#define INLINE inline


/** Boolean true value.
*/
#define TRUE						1


/** Boolean false value.
*/
#define FALSE						0


/** \brief Maximum number of dimensions allowed.

\remarks This limitation is needed because of how the data set is stored. Removing this limitation is theoretically possible, but it means a lot of dynamic allocations.
*/
#define MAX_DIMENSIONS				3


/** \brief The number of bits in an element of the bit vector.
*/
#define BITVEC_ELEM_BITS			32


/** \brief The value of the most significant bit in an element of the bit vector.
*/
#define BITVEC_ELEM_MAX_BIT			0x80000000


/** \brief The max file name size.
*/
#define FILE_NAME_SIZE				150


/** \brief The max function name size.
*/
#define FUNCTION_NAME_SIZE			50

// by gda
#define LSIZE					1000000

/** \brief Macro for stripping the path from the file name. */
#define FILENAME(_x)				( strrchr(_x, '/') ? strrchr(_x, '/') + 1 : _x )


/** \brief Transforms the parameter in a string.
*/
#define STR(_x)						#_x


/** \brief Transforms the parameter in a string expanding macros in the input before.
*/
#define TOSTR(_x)					STR(_x)


/** \brief Generates a random number between _min and _max with standard rand() precision.
*/
#define RANDOM(_min, _max)			( (SPACE_TYPE) (_min + ((double)_max - _min + 1) * ((double)rand() / ((double)RAND_MAX + 1))) )


/** \brief Generates a random number between _min and _max with double rand() precision.
*/
#define BIGRANDOM(_min, _max)		( (SPACE_TYPE) (_min + ((double)_max - _min + 1) * ((rand() * ((double)RAND_MAX + 1) + rand()) / (RAND_MAX * ((double)RAND_MAX + 2) + 1))) )


/** \brief Macro for minimum.
*/
#define MIN(_a, _b)					( (_a < _b) ? _a : _b )


/** \brief Macro for maximum.
*/
#define MAX(_a, _b)					( (_a > _b) ? _a : _b )


/**	\brief Calculates the number of elements of a bit vector from the number of bits.
*/
#define BIT_VEC_WIDTH(_bits)		( (BITVEC_ELEM_BITS + _bits - 1) / BITVEC_ELEM_BITS )


/** \brief Returns the element number which contains the nth bit of the bit vector.
*/
#define BIT_TO_POS(_n)				( _n / BITVEC_ELEM_BITS )


/** \brief Returns the bit position in the given element of the bit vector.
*/
#define BIT_POS_IN_VEC(_n, _elem)	( _n - (_elem * BITVEC_ELEM_BITS) )


/** \brief Returns the value of the nth bit in ascending order (bit 0 == 1).
*/
#define ABIT(_n)					( 0x00000001 << _n )


/** \brief Returns the value of the nth bit in descending order (bit 0 == 2^31).
*/
#define DBIT(_n)					( BITVEC_ELEM_MAX_BIT >> _n )


/** \brief Sets to true the true bits in the mask.
*/
#define BIT_SET(_y, _mask)			( _y |= (_mask) )


/** \brief Sets to false the true bits in the mask.
*/
#define BIT_CLEAR(_y, _mask)		( _y &= ~(_mask) )


/** \brief Flips the bits (true becomes false, false becomes true) that are true in the mask.
*/
#define BIT_FLIP(_y, _mask)			( _y ^= (_mask) )


/** \brief Checks whether the nth bit is set (ascending order).
*/
#define BIT_ACHECK(_y, _n)			( _y & (0x00000001 >> _n) )


/** \brief Checks whether the nth bit is set (descending order).
*/
#define BIT_DCHECK(_y, _n)			( _y & (BITVEC_ELEM_MAX_BIT >> _n) )


#endif // __DEFINES_H
