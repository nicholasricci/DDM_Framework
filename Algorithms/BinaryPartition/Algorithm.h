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


#ifndef __ALGORITHM
#define __ALGORITHM

/**
	 * \file Algorithm.h
	 * \brief It cointains the proposed algorithm implementation
	 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "types.h"
#include "error.h"
#include "DDMUtils.h"
#include "bitmatrix.h"
#include "bitvector.h"
#include "lists.h"
#include "ExtentMaker.h" //TODO, rendere visibile solo l'allocazione vuota di una instanza (separando i file)
#include "execution_time.h"
#include "DDM_input_output.h"

/**
* \brief Loads the input file in a MatchingInstance.
* This method loads a input file (whose path has been provided) in a provided MatchingInstance.
* \param instance an initialized MatchingInstance ** that will cointain the instance read from input file.
* \param filename a string that cointains the file path
* \return an error code if something went wrong,err_none otherwise.
*/
_ERR_CODE readInput(MatchingInstance ** instance, const uint_fast8_t dimension, const uint32_t subscriptio, const uint32_t updates, const char * filename);

/**
* \brief This is where your algorithm executes.
* This is the method that is called by DDMBench to make you run your algorithm.
* Just before calling it DDMBench starts the timer and when you return the timer is halted.
* \param InstanceName cointains the name of the instance you have to solve. Instance is located in ./InstanceName/INPUT_FILE_NAME
* \param result this is where you'll store your matching result. You'll have to initialize it calling create_bit_matrix_0 method.
*/
_ERR_CODE ExecuteAlgorithm(const char * InstanceName,const uint_fast8_t dimensions,const uint32_t subscription,const uint32_t updates, const uint32_t iteration,  bitmatrix ** result);

//TODO WRITE BRIEF
char * getAlgorithmName(void);

#endif //__ALGORITHM
