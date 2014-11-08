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


#ifndef __MATCHING_H
#define __MATCHING_H


/** \file matching.h
\brief Header of file matching.c

The file matching.c contains the main matching algorithm functions.
*/


_ERR_CODE sort_matching(const match_data_t data, const bitmatrix out);
void sort_matching_1D(const list_ptr ep_list, const bitmatrix out, const bitvector subscr_set_before, const bitvector subscr_set_after, const _UINT size_update, const _UINT size_subscr);


#endif // __MATCHING_H
