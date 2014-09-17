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


#ifndef __HEADER_H
#define __HEADER_H


/** \file test_generator.h
\brief Header of the file test_generator.h

This file contains the functions used to generate data sets for testing purposes.
*/

_ERR_CODE test_generator(match_data_t *out, const _UINT updates, const _UINT subscrs, const _UINT dimensions);
_ERR_CODE test_generator_random(match_data_t *out, const _UINT updates, const _UINT subscrs, const _UINT dimensions, float alfa);


#endif // __HEADER_H
