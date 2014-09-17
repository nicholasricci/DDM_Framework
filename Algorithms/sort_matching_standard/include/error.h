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


#ifndef __ERROR_H
#define __ERROR_H


/** \file error.h
\brief Header of file error.c

The file error.c contains the error handling functions.
*/


/** \brief Global external error variable.
*/
extern _err_t ERR_VAR;


_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const _UINT line);
_ERR_CODE print_error_string();


#endif // __ERROR_H
