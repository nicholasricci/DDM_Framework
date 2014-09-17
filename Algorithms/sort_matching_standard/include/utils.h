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


#ifndef __UTILS_H
#define __UTILS_H


/** \file utils.h
\brief Header of the file utils.c

The file utils.c contains various utilities functions.
*/


_ERR_CODE create_bit_matrix(bitmatrix *out, const _UINT size_update, const _UINT size_subscr);

void vector_bitwise_not(const bitvector vec, const _UINT size);
void vector_bitwise_and(const bitvector result, const bitvector mask, const _UINT size);
void vector_bitwise_or(const bitvector result, const bitvector mask, const _UINT size);

void set_endpoints_list(const match_data_t data, const list_ptr out, const _UINT dimension);

INLINE void sort_list(const list_ptr ep_list, const _UINT size);

#ifdef __VERBOSE
void print_bitmatrix(const bitmatrix in, const _UINT size_update, const _UINT size_subscr);
#endif // __VERBOSE


#endif // __UTILS_H
