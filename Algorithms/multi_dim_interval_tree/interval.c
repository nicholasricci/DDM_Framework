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

int intersect( const struct interval* x, const struct interval* y, const size_t dimensions )
{
    int k;
    int isValid = 1;
    for (k = 0; k < dimensions; ++k){
      if (
	  (x->lower[k] <= y->lower[k] && y->lower[k] <= x->upper[k])
	  ||
	  (x->lower[k] <= y->upper[k] && y->upper[k] <= x->upper[k])
	  ||
	  (y->lower[k] <= x->lower[k] && x->lower[k] <= y->upper[k])
	  ||
	  (y->lower[k] <= x->upper[k] && x->upper[k] <= y->upper[k])
	 ){
	  continue;
	}else{
	  isValid = 0;
	}
    }
    return isValid;
}
