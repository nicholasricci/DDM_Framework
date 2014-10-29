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


#ifndef __ERROR_H
#define __ERROR_H


#include "defines.h"
#include "DDMUtils.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/** \file error.h
\brief Header of file error.c

The file error.c contains the error handling functions.
*/

#define FILE_NAME_SIZE 100
#define FUNCTION_NAME_SIZE 100

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
	err_threads					= 7
} _ERR_CODE;


/** \brief Structure containing error data.
*/
typedef struct
{
	_ERR_CODE code;						///< error code
	char file[FILE_NAME_SIZE];			///< file who generated the error
	char function[FUNCTION_NAME_SIZE];	///< function who generated the error
	uint32_t line;						///< line of the file who generated the error
} _err_t;


/** \brief Sets the error global variable
 *
 * \param code const _ERR_CODE, the error code that just happened
 * \param file const char*, the file where the error occurred.
 * \param function const char*, the function where the error occurred
 * \param line const uint32_t, the line where the error occurred
 * \return _ERR_CODE, the error code provided is returned for enable function call concatenation.
 *
 */
_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const uint32_t line);


/** \brief Prints the error string that is store in the error global variable in that moment.
 *
 * \return _ERR_CODE
 *
 */
_ERR_CODE print_error_string(void);

/** \brief Prints a provided error string
 *
 * \param string char*, The string that must be print out.
 * \param force_exit bool, if it is true the execution will be aborted just after printing the error string.
 * \return void
 *
 */
void print_runtime_error_string(char * string, bool force_exit);


#endif // __ERROR_H
