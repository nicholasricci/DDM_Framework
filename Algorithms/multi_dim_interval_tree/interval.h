/****************************************************************************
 *
 * interval.h
 *
 * This file is part of DDM
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

#ifndef INTERVAL_H
#define INTERVAL_H

/**
 * This struct represents the semiopen intnerval [lower, upper)
 */
struct interval {
    int id;		/* the ID of this interval. We assume that intervals are labeled as 0, 1, ... */
    float lower;	/* the lower bound */
    float upper;	/* the upper bound */
};

/**
 * Returns 1 is intervals |x| and |y| intersect, 0 otherwise.
 */
int intersect( const struct interval* x, const struct interval* y );

#endif
