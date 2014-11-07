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


#include "../include/types.h"

#include <stdio.h>
#include <string.h>


/** \file error.c
\brief File containing the error handling functions.
*/


/** \brief Array of error strings.

\remarks The indices must be matched with the ERR_CODE values.
*/
static const char *err_strings[] =
{
	"No error",
	"An unhandled error occurred",
	"A generic error occurred",
	"An error occurred while allocating memory",
	"An error occurred while creating/opening a file",
	"Bad input",
	TOSTR(The problem cant have more than MAX_DIMENSIONS dimensions),
	"An error occurred while handling threads",
	"An error occurred in one of the OpenCL routines",
	TOSTR(No platform/device from VIDEO_CARD_VENDOR found),
	"An error occurred while processing the file containing the OpenCL program"
};


/** \brief The global error variable.
*/
static _err_t ERR_VAR;


/** \brief Sets the global variable and its info to the actual error.

\param code the code of the error
\param file string containing the name of the file in which the error was generated
\param function string containing the name of the function in which the error was generated
\param file the number of the line in which the error was generated

\retval error code
*/
_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const _UINT line)
{
	if (code != err_none)
	{
		ERR_VAR.code = code;
		if (sprintf(ERR_VAR.file, "%s", file) < 0)
			return err_unhandled;
		if (sprintf(ERR_VAR.function, "%s", function) < 0)
			return err_unhandled;
		ERR_VAR.line = line;
	}

	return code;
}


/** \brief Prints the error message.

\retval error code
*/
_ERR_CODE print_error_string()
{
	if (ERR_VAR.code == err_none || ERR_VAR.code == err_unhandled)
		printf("%s", err_strings[ERR_VAR.code]);
	else 
		printf("%s:%d: in function '%s': error %d: %s", ERR_VAR.file, ERR_VAR.line, ERR_VAR.function, ERR_VAR.code, err_strings[ERR_VAR.code]);

#ifdef __DEBUG
	getchar();
#endif // __DEBUG

	return ERR_VAR.code;
}
