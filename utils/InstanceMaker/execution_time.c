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


#include "execution_time.h"

static time_struct elapsed_times;

_ERR_CODE time_init (int iteration_count)
{
    elapsed_times.clocks = (double*) malloc(sizeof(double)*iteration_count);
    if(elapsed_times.clocks == NULL)
        return set_error(err_alloc,__FILE__, __FUNCTION__, __LINE__);
    elapsed_times.size = iteration_count;
    elapsed_times.total_time = 0.0;
    elapsed_times.total_time_2 = 0.0;
    return err_none;
}
void start_time(int iteration)
{
    if(iteration < elapsed_times.size)
    {
        elapsed_times.clocks[iteration] = omp_get_wtime();
    }
}


void stop_time(int iteration)
{
    if(iteration < elapsed_times.size)
    {
        elapsed_times.clocks[iteration] = omp_get_wtime() - elapsed_times.clocks[iteration];
        elapsed_times.total_time += elapsed_times.clocks[iteration];
        elapsed_times.total_time_2 = elapsed_times.total_time_2 + (elapsed_times.clocks[iteration] * elapsed_times.clocks[iteration] );
    }
}

double get_totalTime()
{
    return elapsed_times.total_time;
}

double get_totalTime2()
{
    return elapsed_times.total_time_2;
}

double get_time(int iteration)
{
    return elapsed_times.clocks[iteration];
}

void time_free()
{
    free(elapsed_times.clocks);
}
