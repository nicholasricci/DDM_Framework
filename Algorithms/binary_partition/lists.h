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


#ifndef __LISTS_H
#define __LISTS_H

/**
*   \brief This files contains functions to handle dinamically allocated lists.
*   \author Francesco Bedini
*   \file lists.h
*/

#include <stdlib.h>
#include <stdio.h>


/** \brief An element of the list
 */
typedef struct list_element{
    size_t id;                  /**< The id of the element */
    struct list_element * next;     /**< A pointer to the next element of the list */
}list_element;

typedef list_element * list;        /**< A typedef for a list, it is a pointer to a list element */

/** \brief Initialize a list
 *  \remarks The list must be already allocated. The pointer can't be NULL.
 *
 * \param l list*, A pointer to the list that must be initialized.
 * \return void
 *
 */
void list_init(list * l);

/** \brief This function add an element to a list in O(1) operations (appending it at the beginning of the list).
 *
 * \param l list*, the list where the element must be added.
 * \param element_id const uint64_t, the ID of the element that will be created and appended.
 * \return void
 *
 */
void list_add_by_id(list * l, const size_t element_id);


/** \brief This functions add an element to an ordered list.
 * \remarks this method assumes that the list is already ordered (i.e. all its elements have been added calling this method).
 *
 * \param l const list, the list where the element must be added.
 * \param element_id const uint64_t, the id of the element to add.
 * \return void
 *
 */
void list_add_by_id_ordered(const list l, const size_t element_id);


/** \brief Add an existing element passing a reference to it.
 *
 * \param l list*, the list where the element will be added
 * \param element const list_element*, a pointer to the element to be added
 * \return void
 *
 */
void list_add_element(list * l, const list_element * element);

/** \brief Removes an element from a list providing its ID.
 *
 * \param l list*, the list from where the element will be removed
 * \param element_id const uint64_t, the ID of the element to be removed
 * \return int, the number of elements removed (1 or 0)
 *
 */
int list_remove_by_id(list * l, const size_t element_id);

/** \brief Looks for an element in a list
 *
 * \param l const list, the list to check
 * \param element_id const uint64_t, the id of the element to look for.
 * \return list_element*, a pointer to the element if found.
 *
 */
list_element * is_in_list(const list l, const size_t element_id);

/** \brief Looks for an element in an ordered list.
As the list is ordered, this method is on average more efficient.
 *
 * \param l const list, the list to check
 * \param element_id const uint64_t, the id of the element to look for.
 * \return list_element*, a pointer to the element if found.
 *
 */
list_element * is_in_ordered_list(const list l, const size_t element_id);


/** \brief Removes all the element of a list
 *
 * \param l list*, the list whose elements must be removed
 * \return void
 *
 */
void list_remove_all(list * l);

/** \brief Prints a human readable list
 *
 * \param l list, the list that must be printed
 * \param title char*, a string that can be used to distinguish different prints.
 * \return void
 *
 */
void list_print(list l, char * title);

#endif // __LISTS_H
