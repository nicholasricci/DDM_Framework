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


#ifndef DDMUTILS_H_INCLUDED
#define DDMUTILS_H_INCLUDED

/**
	 * \file DDMUtils.h
	 * \author Francesco Bedini
	 * \brief Cointains utility functions
	 */

#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** \brief This function returns the first available line of a text file skipping commented lines.
A commented line is a line that starts with a #. If it reaches the end of the file it returns false.
 *
 * \param string, a string where the read line will be stored.
 * \param fp, a file pointer already initialized.
 * \return true if there is a valida line in string, false otherwise.
 *
 */

bool ReadNextString(char * string, FILE * fp);

void WriteLogLine(char * line);
#endif // DDMUTILS_H_INCLUDED
