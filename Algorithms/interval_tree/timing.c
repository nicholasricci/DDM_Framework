/****************************************************************************
 *
 * timing.c
 *
 * This file is part of ddm
 *
 * Copyright (C) 2013 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#include "timing.h"
#include <assert.h>

void timing_init( struct timer* t )
{
    t->total = 0;
    t->n = 0;
}

void timing_start( struct timer* t )
{
    clock_gettime(CLOCK_MONOTONIC, &(t->t_start) );
}

void timing_stop( struct timer* t )
{
    clock_gettime(CLOCK_MONOTONIC, &(t->t_stop) );
    t->n++;
    t->total += \
	(t->t_stop).tv_sec + (double)((t->t_stop).tv_nsec) / 1.0e9 - 
	((t->t_start).tv_sec + (double)((t->t_start).tv_nsec) / 1.0e9);
}

double timing_get_average( const struct timer* t )
{
    assert( t->n > 0 );
    return (t->total / t->n );
}
