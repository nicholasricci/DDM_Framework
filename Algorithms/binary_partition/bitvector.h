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


#ifndef BITVECTOR_H_INCLUDED
#define BITVECTOR_H_INCLUDED

/** \brief This file store the header of the bitvecor.c file
 *
 * \file bitvector.h
 * \author Francesco Bedini
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

#include "defines.h"

/**
\brief If defined a 64 unsigned integer is used as base element.
Else a 32 bit integer is used instead
*/
#define BITVECTOR_64

#ifdef BITVECTOR_64

    typedef uint64_t BITVECTOR_TYPE;
    #define MASK_TYPE uint64_t
    #define FULL_MASK UINT64_MAX
    #define BIT_LENGTH 64
    #define VECTOR_MASK UINT64_C(0x0000000000000001)
    /** \brief The value of the most significant bit in an element of the bit vector.
    */
    #define BITVEC_ELEM_MAX_BIT			0x8000000000000000

#else
    typedef uint32_t BITVECTOR_TYPE;
    #define MASK_TYPE uint32_t
    #define FULL_MASK UINT32_MAX
    #define BIT_LENGTH 32
    #define VECTOR_MASK UINT32_C(0x00000001)
    /** \brief The value of the most significant bit in an element of the bit vector.
   */
    #define BITVEC_ELEM_MAX_BIT			0x80000000
#endif

#define BIT_VEC_WIDTH(_bits)		( (BIT_LENGTH + _bits - 1) / BIT_LENGTH )
    /** \brief Returns the element number which contains the nth bit of the bit vector.
*/
#define BIT_TO_POS(_n)				( _n / BIT_LENGTH )
    /** \brief Returns the bit position in the given element of the bit vector.
*/
#define BIT_POS_IN_VEC(_n, _elem)	( _n - (_elem * BIT_LENGTH) )

/** \brief Returns the value of the nth bit in descending order (bit 0 == 2^31 or 2^63).
*/
#define DBIT(_n)					( BITVEC_ELEM_MAX_BIT >> _n )

/** \brief Returns the value of the nth bit in ascending order (bit 0 == 1).
*/
#define ABIT(_n)					( 0x00000001 << _n )

/** \brief Sets to true the true bits in the mask.
*/
#define BIT_SET(_y, _mask)			( _y |= (_mask) )

/** \brief Checks whether the nth bit is set (ascending order).
*/
#define BIT_ACHECK(_y, _n)			( _y & (0x00000001 << _n) ) /*TODO NOTA nel file di mandrioli è l'opposto*/

/** \brief Checks whether the nth bit is set (descending order).
*/
#define BIT_DCHECK(_y, _n)			( _y & (BITVEC_ELEM_MAX_BIT >> _n) )

/** \brief Sets to false the true bits in the mask.
*/
#define BIT_CLEAR(_y, _mask)		( _y &= ~(_mask) )


/** \brief Flips the bits (true becomes false, false becomes true) that are true in the mask.
*/
#define BIT_FLIP(_y, _mask)			( _y ^= (_mask) )

typedef BITVECTOR_TYPE* bitvector;
typedef bitvector* bitmatrix;
/*typedef struct bitv{
    uint64_t element_count;
    uint32_t created_elements;
    BITVECTOR_TYPE * vect;
}bitvector;
*/
bitvector init_bitvector(uint64_t size);

void free_vector(bitvector * vector);

void reset_vector(bitvector vector, const uint64_t size);

void set_vector(bitvector vector, const uint64_t size);

bool isSet(bitvector vector, const uint64_t pos);

void negate_vector(bitvector vector, const uint64_t size);

void set_element(bitvector vector, const uint64_t pos);

void reset_element(bitvector vector, const uint64_t pos);

void copy_vector(bitvector vdest, bitvector vsource, const uint64_t size);

void negate_element(bitvector vector, const uint64_t pos);

void print_bitvector(bitvector vector, const uint64_t size);

void vector_bitwise_and(const bitvector result, const bitvector mask, const uint64_t size);

void vector_bitwise_or(const bitvector result, const bitvector mask, const uint64_t size);

#endif // BITVECTOR_H_INCLUDED
