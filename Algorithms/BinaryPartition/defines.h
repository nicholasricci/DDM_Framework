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


/**
 * \file defines.h
 * \author: Francesco Bedini
 * \brief this file contains all defines used by DDMBenchm and DDMIstanceMaker
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdint.h>
#include <inttypes.h>

#define DDM_ISTANCE_MAKER_VERSION "0.5b"
#define DDM_BENCH_VERSION "0.5b"
#define __DEBUG

/**
* \brief
*/

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

/**
* \brief defines the info file name
This file cointains all the info about the current instance.
*/
#define INFO_FILE_NAME "info.txt"

/**
* \brief defines the result file name
this file will store all the results of the different algorithms executed on this instance.
*/
#define RESULTS_FILE_NAME "results.txt"

/**
* \brief defines the input file name
*/
#define INPUT_FILE_NAME  "input-%" PRIu32 ".txt"

/**
* \brief defines the output file name
*/
#define OUTPUT_FILE_NAME "output-%" PRIu32 ".dat"


/** \brief The name of the log file
 *
 */
#define LOG_FILENAME "log.txt"

/** \brief The name of the csv file that contains the results list
 *
 */
#define CSV_FILENAME "results.csv"

/** \brief The name of the folter that contains the execution results
 *
 */
#define RESULTS_FOLDER_NAME "results"
/**
* \brief the max length of a file line
*/
#define LINE_MAX_LENGTH 4000

/**
* \brief The size of the buffer used to concatenate strings one another.
*/
#define BUFFER_SIZE 500

/**
* \brief defines boolean type, true is 1, false is 0
*/
#define bool uint_fast8_t
#define true 1
#define false 0

/**
* \brief The maximum size of a file path
*/
#define MAX_FILE_PATH_LENGHT 400

#define MAX_FILENAME_LENGTH 100

/**
* \brief if ARC_64 is defined 64 bit data will be used
*/
#define ARC_64 1

/**
* \brief the version number of this bench
*/
#define PROGRAM_VERSION "0.5"

/**
* \brief the maximum number of dimensions that can be stored (this is done to avoid a further malloc for each extent).
*/
#define MAX_DIMENSIONS 4

/**
* \brief a macro that generates a double precision random rumber between _min and _max
*/
#define BIGRANDOM(_min, _max)		( (SPACE_TYPE) (_min + ((double)_max - _min + 1) * ((rand() * ((double)RAND_MAX + 1) + rand()) / (RAND_MAX * ((double)RAND_MAX + 2) + 1))) )

/**
* \brief a macro that returns the smallest between two numbers
*/
#define MIN(_a,_b)					(_a<_b?_a:_b)

/**
* \brief a macro that returns the biggest between two numbers
*/
#define MAX(_a,_b)					(_a>_b?_a:_b)

/**
 * \brief a macro that returns the filename of a given filepath
 */
#define FILENAME(_x)				( strrchr(_x, '/') ? strrchr(_x, '/') + 1 : _x )

#endif /* DEFINES_H_ */
