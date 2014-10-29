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


#include "error.h"


/** \file error.c
* \brief File containing the error handling functions.
* This file is derived from error.c included in SortMatching by Marco Mandrioli (licensed under GNU Affero General Public License)
*/


/**
* \brief Array of error strings.
* \remarks The indices must be matched with the ERR_CODE values.
*/
static const char *err_strings[] =
{
	"No error",
	"An unhandled error occurred",
	"A generic error occurred",
	"An error occurred while allocating memory",
	"An error occurred while creating/opening a file",
	"Bad input",
	"The problem cant have more than the max number of dimensions (see define MAX_DIMENSIONS in defines.h)",
	"An error occurred while handling threads"
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
_ERR_CODE set_error(const _ERR_CODE code, const char *file, const char *function, const uint32_t line)
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


/**
* \brief Prints the error message.
* \retval error code
*/
_ERR_CODE print_error_string()
{
	if (ERR_VAR.code == err_none || ERR_VAR.code == err_unhandled)
		printf("%s", err_strings[ERR_VAR.code]);
	else
	{
		fprintf(stderr, "\n\n******************** ERROR **********************\n");
		fprintf(stderr, "%s:%d: in function '%s': error %d: %s\n", ERR_VAR.file, ERR_VAR.line, ERR_VAR.function, ERR_VAR.code, err_strings[ERR_VAR.code]);
		exit(EXIT_FAILURE);
	}
	return ERR_VAR.code;
}

/**
 * \brief Prints an error message on stderr
 * This method prints an error string on stderr.
 * \param string the error string to print out
 * \param force_exit a boolean value. If it's true the exectuion is halted immediately after printing the message, else the execution continues.
 */
void print_runtime_error_string(char * string, bool force_exit)
{
    char buffer[BUFFER_SIZE + 14];
	fprintf(stderr, "\n\n******************** ERROR **********************\n");
	fprintf(stderr,"%s\n", string);
	if(force_exit)
    {
        strcpy(buffer, "FATAL ERROR: ");
        strcat(buffer, string);
        WriteLogLine(buffer);
        exit(EXIT_FAILURE);
    }
    else
    {
        strcpy(buffer, "WARNING: ");
        strcat(buffer,string);
        WriteLogLine(buffer);
    }
}
