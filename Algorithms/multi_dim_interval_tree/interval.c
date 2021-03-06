/****************************************************************************
 *
 * interval.c
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

#include "interval.h"

int intersect( const struct interval* x, const struct interval* y, size_t current_dim )
{
    //return ( x->lower < y->upper && x->upper > y->lower );
    return (
	     ( x->lower[current_dim] <= y->lower[current_dim] && y->lower[current_dim] <= x->upper[current_dim] )
	      ||
	     ( x->lower[current_dim] <= y->upper[current_dim] && y->upper[current_dim] <= x->upper[current_dim] )
	      ||
	     ( y->lower[current_dim] <= x->lower[current_dim] && x->lower[current_dim] <= y->upper[current_dim] )
	      ||
	     ( y->lower[current_dim] <= x->upper[current_dim] && x->upper[current_dim] <= y->upper[current_dim] )
	   );
}
