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


#ifndef EXECUTION_TIME_H_INCLUDED
#define EXECUTION_TIME_H_INCLUDED
/** \brief This file includes all the methods related to the elapsed time.
 *
 * \file execution_time.h
 * \author Francesco Bedini
 *
 */


#include "error.h"
#include <stdlib.h>
#include <omp.h>

typedef struct elapsed {
double * clocks;
double total_time;
double total_time_2; //Used to compute standard deviation
int size;
}time_struct;

/** \brief Initialize the time data structure
 *
 * \param iteration_count, number of iteration of this instance.
 * \return an error code
 *
 */

_ERR_CODE time_init (int iteration_count);

/** \brief Starts the timer associated with the given parameter
 *
 * \param iteration, the iteration number whose timer must be started.
 *
 */
void start_time(int iteration);

/** \brief Stops the timer associated with the given parameter
 *
 * \param iteration, the iteration number whose timer must be stopped.
 *
 */
void stop_time(int iteration);

/** \brief Returns the sum of the time taken to solve all the instances
 *
 */
double get_totalTime();

/** \brief Returns the sum of the time raised to the second taken to solve all the instances.
This value is used to compute the standard deviation.
 *
 */
double get_totalTime2();

/** \brief Returns elapsed time of a given iteration
 *  \param iteration, the iteration number whose timer must be returned.
 *  \return the elapsed time as a double value
 *
 */
double get_time(int iteration);

/** \brief Frees the memory occupied by the time data structures
 *
 */

void time_free();

#endif // EXECUTION_TIME_H_INCLUDED
