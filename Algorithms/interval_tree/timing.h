/****************************************************************************
 *
 * timing.h
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
#ifndef TIMING_H
#define TIMING_H

#include <time.h>

struct timer {
    struct timespec t_start, t_stop;
    double total;
    int n;
};

/**
 * Initializes the timing object pointed by |t|
 */
void timing_init( struct timer* t );

/**
 * Starts a new timing measure
 */
void timing_start( struct timer* t );

/**
 * Stops the currently running measure
 */
void timing_stop( struct timer* t );

/**
 * Get the average time across all measures
 */
double timing_get_average( const struct timer* t );

#endif
