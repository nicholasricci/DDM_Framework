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


#include "bitvector.h"

#define POS_TO_MASK(_pos) ((MASK_TYPE)((VECTOR_MASK)<<(_pos % BIT_LENGTH)))

bitvector init_bitvector(uint64_t size)
{
    bitvector vector = (bitvector) calloc(sizeof(BITVECTOR_TYPE), BIT_VEC_WIDTH(size));
    return vector;
}

void free_vector(bitvector *vector)
{
    free(*vector);
    *vector=NULL;
}

void reset_vector(bitvector vector, const uint64_t size)
{
    memset(vector, 0x00, BIT_VEC_WIDTH(size) * sizeof(BITVECTOR_TYPE));
}

void set_vector(bitvector vector, const uint64_t size)
{
    memset(vector, 0xff, BIT_VEC_WIDTH(size) * sizeof(BITVECTOR_TYPE));
}

void negate_vector(bitvector vector, const uint64_t size)
{
    uint64_t i;
    for(i=0; i<BIT_VEC_WIDTH(size); i++)
    {
        vector[i] = ~vector[i];
    }
}

void set_element(bitvector vector, const uint64_t pos)
{
    BIT_SET(vector[BIT_TO_POS(pos)],DBIT(BIT_POS_IN_VEC(pos,BIT_TO_POS(pos))));
}

void reset_element(bitvector vector, const uint64_t pos)
{
    BIT_CLEAR(vector[BIT_TO_POS(pos)],DBIT(BIT_POS_IN_VEC(pos,BIT_TO_POS(pos))));
}

void negate_element(bitvector vector, const uint64_t pos)
{
    uint64_t bit_pos;
    bit_pos = BIT_TO_POS(pos);
    BIT_FLIP(vector[bit_pos], DBIT(BIT_POS_IN_VEC(pos,bit_pos)));
}

void copy_vector(bitvector vdest, bitvector vsource, const uint64_t size)
{
    memccpy(vdest, vsource, BIT_VEC_WIDTH(size), sizeof(BITVECTOR_TYPE));
}

bool isSet(bitvector vector, const uint64_t pos)
{
    return BIT_DCHECK(vector[BIT_TO_POS(pos)], pos) != 0;
}

void print_bitvector(bitvector vector, const uint64_t size)
{
    uint64_t j, bit; //TODO tipo giusto?
	uint8_t element[BIT_LENGTH + 1];
	BITVECTOR_TYPE val;
	BITVECTOR_TYPE mask;
    printf("\n________BITVECTOR _________\n");
	uint64_t line_width;

	// number of elements on each line of the bit matrix
	line_width = BIT_VEC_WIDTH(size);

	// escape the string
	element[BIT_LENGTH] = '\0';
		// for each element in the line
		for (j = 0; j < line_width; j++)
		{
			mask = BITVEC_ELEM_MAX_BIT;
			val = vector[j];

			// for each bit in the element
			for (bit = 0; bit < BIT_LENGTH; bit++)
			{
				// if the actual bit is TRUE, set the char to '1', else set it to '0'
				element[bit] = (val & mask ? '1' : '0');
				// shift right
				mask >>= 1;
			}

			// print the string
			printf("%s ", element);
		}
}

/** \brief Bitwise AND of two bit vectors.

It can also be used to do the bitwise AND of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_and(const bitvector result, const bitvector mask, const uint64_t size)
{
	uint64_t i;

	for (i = 0; i < size; i++)
		result[i] &= mask[i];
}

/** \brief Bitwise OR of two bit vectors.

It can also be used to do the bitwise OR of the matrix, since it's allocated as linear memory.

\param result first vector and also resulting vector
\param mask second vector
\param size number of elements in the bit vectors
*/
void vector_bitwise_or(const bitvector result, const bitvector mask, const uint64_t size)
{
	uint64_t i;

	for (i = 0; i < size; i++)
		result[i] |= mask[i];
}
