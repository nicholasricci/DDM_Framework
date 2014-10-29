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


#ifndef __EXTENTMAKER_H
#define __EXTENTMAKER_H

/**
*   \brief This file contains the functions used to generate an instance
*   \author Francesco Bedini
*   \file ExtentMaker.h
*/

#include "defines.h"
#include "DDMUtils.h"
#include "error.h"
#include "types.h"

#include <time.h>
#include <stdio.h>

/**
* \brief generates a random extent set.
* This procedure generates a random MatchingInstance from given parameters. Extents are randomly generated.
* \param out a MatchingInstance ** that will store method's output.
* \param subscriptions the number of subscription extents
* \param updates the number of update extents
* \param dimensions the number of dimensions
* \param avg_ext_size the average size of the extents. 0 is for random size.
* \return an _ERR_CODE, err_none if everything went well.
*/
_ERR_CODE generate_random_extent_set(MatchingInstance ** out,  const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions, const uint64_t avg_ext_size);

/**
* \brief generates a fixed extent set from a given random seed.
* This procedure generates a MatchingInstance from given parameters. Extents are "randomly" generated from a given seed.
* \param out a MatchingInstance ** that will store method's output.
* \param seed the seed that will be used by the random function.
* \param subscriptions the number of subscription extents
* \param updates the number of update extents
* \param dimensions the number of dimensions
* \param avg_ext_size the average size of the extents. 0 is for random size.
* \return an _ERR_CODE, err_none if everything went well.
*/
_ERR_CODE generate_fixed_extent_set(MatchingInstance ** out, const unsigned int seed, const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions, const uint64_t avg_ext_size);

/**
* \brief generates an empty extent set from of given dimensions.
* This procedure generates an empty MatchingInstance from given parameters. It just allocs memory occupied by dynamic data structures. It's called by the other function as initialization.
* \param out a MatchingInstance ** that will store method's output.
* \param subscriptions the number of subscription extents
* \param updates the number of update extents
* \param dimensions the number of dimensions
* \return an _ERR_CODE, err_none if everything went well.
*/
_ERR_CODE generate_empty_extent_set(MatchingInstance ** out, const uint32_t subscriptions, const uint32_t updates, const uint_fast8_t dimensions);

/**
* \brief generates a predefined small MatchingInstance.
* This procedure generates a small MatchingInstance that can be used for testing or debugging purposes.
* \param out a MatchingInstance ** that will store method's output.
* \return an _ERR_CODE, err_none if everything went well.
*/
_ERR_CODE generate_sample_extent_set(MatchingInstance ** out);

/** \brief outputs a MatchingInstance to a file.
* This procedure prints a MatchingInstance and few other information on a text file
* \param out a MatchingInstance * that will be printed out on a file.
* \param file_path the path of the output file
* \param override_file if it's true the file will be overwritten if it already exists. If false the program will ask user confirmation to overwrite it.
* \return an _ERR_CODE err_none if everything went well.
*/
_ERR_CODE save_to_file (MatchingInstance * m, const char * file_path, const bool override_file);

/**
*   \brief Saves a file that contains the information about the instance
*   \param file_path the path where to save the instance info.
*   \param instance_name the name of the instance.
*   \param author_name the name of the author of the instance. DDMInstanceMaker set it to "DDMInstanceMaker" if not provided.
*   \param version_number the version number of the instance
*   \param override_file if true the file will be overwritten if already exists
*   \param random_seed the seed that generated the instance
*   \param sequence_length the number of steps of the instance
*   \param dimensions the number of dimensions of the instance
*   \param subscription the number of subscription extents of the instance
*   \param updates the number of update extents of the instance
*   \return _ERR_CODE an error code
*/
_ERR_CODE save_instance_info_on_file(const char * file_path, const char * instance_name, const char * author_name, const char *version_number, const bool override_file, const unsigned int random_seed, const uint32_t sequence_length, const uint_fast8_t dimensions, uint32_t subscriptions, uint32_t updates);
/**
 *\brief frees a MatchingInstance.
 * This procedure frees the memory occupied by a MatchingInstance.
 * \param instance, the instance whose memory must be freed.
 * \param discardResult, if false the output bitmatrix won't be freed.
 */
void free_matching_instance(MatchingInstance ** instance, bool discardResult);


/** \brief This function generates a certain number of consecutive steps using a modified random waypoint model.
 * Other functions with the same header may be created to move extents in different ways (this function is passed as a parametere in DDMInstanceMaker).
 * \remarks Be sure to check or prevent data overflow if you reimplement this function. Remember that the lower bound of an extents has to be lower or equal to the upper bound.
 *
 * \param m, the matching instance whose extents has to be moved.
 * \param iteration_number the number of steps to generate (-1, because the first has already been generated by DDMInstanceMaker)
 * \param movement_restriction it states if all extents must be moved or just a group of them.
 * \param instance_solver an optimal matching solver to solve the generated instances (standard brute force is recommended).
 * \param ouput_dir the path of the dir where the generated file has to be saved.
 * \param velocity the "speed" at which the extents must be moved.
 * \param acceleration the acceleration at which the extents must be moved.
 * \return
 *
 */
_ERR_CODE move_extents_random_waypoints(MatchingInstance * m, uint32_t iteration_number, movement_restriction restriction, void (*instance_solver)(MatchingInstance *,bitmatrix *), char * output_dir, SPACE_TYPE velocity, SPACE_TYPE acceleration);

#endif //__EXTENTMAKER_H
