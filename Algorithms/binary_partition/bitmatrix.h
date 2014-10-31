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


#ifndef __BITMATRIX_H
#define __BITMATRIX_H

/**
* \file bitmatrix.h
* \author Francesco Bedini
* \brief Cointains the definition of bitmatrix data structure
* bitmatrix is a 2d marix of bool, stored as bits (in an unsigned integer vector).
* This file is derived from error.c included in SortMatching by Marco Mandrioli (licensed under GNU Affero General Public License)
*/

#include <stdint.h>
#include "bitvector.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include "defines.h"
#include "error.h"

/** \brief An enum that states the possible initial value of a bitmatrix
 */

typedef enum InitialValue{
zero,   /**< Every element of the bitmatrix is set to zero */
one,    /**< Every element of the bitmatrix is set to one */
uninitialized  /**< Every element of the bitmatrix has a random value  */
}InitialValue;

/**
 * \brief initialize a bitmatrix.
 * This method initialize a bitmatrix
 * \param out a bitmatrix * where method's result will be stored.
 * \param rows the number of rows of the virtual matrix (# of update extents)
 * \param columns the number of columns of the virtual matrix (# of subscription extents)
 * \param initial_value the value that the bitmatrix cells has to be initialized to.
 */
_ERR_CODE init_bit_matrix (bitmatrix * matrix_pointer, const uint64_t rows, const uint64_t columns, const InitialValue initial_value);

/**
 * \brief set a single bitmatrix' bit to 1.
 * A single bit is set to one. This means that update extent having ID 'row' is matched with subscription extent having ID 'column'
 * \param mat the bitmatrix whose bit must be set.
 * \param row the row index of the virtual matrix to set (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to set (i.e. subscription extent's ID)
 */
void set_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief set a single bitmatrix' bit to 0.
 * A single bit is set to zero. This means that update extent having ID 'row' is not matched with subscription extent having ID 'column'
 * \param mat the bitmatrix whose bit must be set.
 * \param row the row index of the virtual matrix to set (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to set (i.e. subscription extent's ID)
 */
void reset_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief Complements a single bitmatrix bit.
 * A single bit is complemented (bit = 1-bit);
 * \param mat the bitmatrix whose bit must be modified.
 * \param row the row index of the virtual matrix to modify (i.e. update extent's ID)
 * \param column the column index of the virtual matrix to modify (i.e. subscriber extent's ID)
 */
void negate_bit_mat (const bitmatrix mat, const uint64_t row, const uint64_t column);

/**
 * \brief Reset a whole bitmatrix' bits to 0.
 * This is an optimized implementation of resetting every single bit of the bitmatrix to 0. This means that all subscription exents are not matched with any update extent
 * Please double check that the provided parameters are correct, otherwise the result will be undefined.
 * \param mat the bitmatrix whose bit must be reset.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void reset_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
 * \brief Set a whole bitmatrix bits to 1.
 * This is an optimized implementation of setting every single bit of the bitmatrix to 1. This means that all subscription exents are matched with any update extent
 * Please double check that the provided parameters are correct, otherwise the result will be undefined.
 * \param mat the bitmatrix whose bit must be set.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void set_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
 * \brief Complements a whole bitmatrix bits.
 * This is an optimized implementation of complementing every single bit of the bitmatrix.
 * \param mat the bitmatrix whose bits must be reset.
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 */
void negate_whole_bit_mat (const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/**
* \brief this procedure frees memory occupied by a bitmatrix.
* \param mat the bitmatrix * whose memory has to be freed.
*/
void free_bit_matrix(bitmatrix * mat);

/**
* \brief Retrieves the current value of a cell.
* \param mat, a bitmatrix * that points to the desired bitmatrix.
* \param row, the index of the row (update extent ID)
* \param col, the index of the column (subscriber extent ID)
* \return true if the bit is set to one, false otherwise.
*/
bool get_bit(const bitmatrix mat,const uint64_t row, const uint64_t col);
/**
 * \brief This function checks that all bits set in mat1 are also set in mat2.
 * This function is used to evaluate if mat2 (the computed_solution) is a superset of mat1 (the optimal solution)
 * \param rows, the number of rows of the virtual matrix
 * \param columns, the number of column of the virtual matrix
 * \return true if mat1 is optimal or suboptimal, false otherwise
*/
bool checkSet (const bitmatrix mat1, const bitmatrix mat2, const int rows, const int columns);

/**
 * \brief prints a bitmatrix to a binary file.
 * This procedure prints out a bitmatrix to a binary file.
 * \param mat the bitmatrix that has to be printed out
 * \param filename a string cointaining the file path
 * \param override_file a boolean value. If it's true the file will be overwritten if it already exists, if it's false the program will ask for user confirmation
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
*/
void print_bit_matrix(const bitmatrix mat, const char * filename, const bool override_file, const uint64_t rows, const uint64_t columns);

/**
* \brief this procedure reads a bitmatrix from a file.
* This procedure loads a bitmatrix from a binary file.
* \param out a bitmatrix ** where the result will be stored
* \param filename a string that cointains the file path
* \param rows a pointer where will be stored the number of rows read from the file
* \param columns a pointer where will be stored the number of columns read from the file
* \return an _ERR_CODE, err_none if everything went well
*/
_ERR_CODE read_bit_matrix (bitmatrix * mat, const char * filename, uint64_t * rows, uint64_t * columns);

/**
 * \brief This function computes the distance between to bitmatrix
 * This function counts how many bits have not the same value in both bitmatrix (bitmatrix' order doesn't matter).
 * \param matrix1, the first bitmatrix
 * \param matrix2, the second bitmatrix
 * \param rows the number of rows of the virtual matrix
 * \param columns the number of column of the virtual matrix
 * \return int the number of bit that don't have the same value in both bitmatrix, -1 if bitmatrix dimensions (number of rows and columns) don't match
*/
int compareBitmatix(const bitmatrix matrix1, const bitmatrix matrix2, const uint64_t rows, const uint64_t columns);

/** \brief Counts how many bits of a bitmatrix are set.
 *
 * \param   mat, the bitmatrix whose bit has to be count
 * \param   rows, the number of rows of the bitmatrix
 * \param   columns, the number of columns of the bitmatrix
 * \return  the number of bit set to one.
 *
 */

uint64_t count_ones(const bitmatrix mat, const uint64_t rows, const uint64_t columns);

/** \brief This method prints a human readable bitmatrix on stdout
 * It is advisable to use this method on small bitmatrix only.
 *
 * \param in, the bitmatrix to be printed
 * \param   rows, the number of rows of the bitmatrix
 * \param   columns, the number of columns of the bitmatrix
 *
 */

void print_readable_bitmatrix(const bitmatrix in, int iteration, int rows, int columns);

#endif //__BITMATRIX_H
