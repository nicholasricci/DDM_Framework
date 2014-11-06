/****************************************************************************
 *
 * int_tree.h
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

/*
 * This header file contains definitions for an "interval tree" data
 * structure (see Cormen, Leiserson and Rivest, "Introduction to
 * Algorithms", MIT Press, 1996, section 15.3). The implementation
 * described in the book is based on Red-Black trees; this
 * implementation, on the other hand, is based on AVL trees.
 *
 * An interval tree can store a dynamic set S of n semiopen intervals
 * [a, b) and supports insertion and removal of an interval in time
 * O(log n). It is possible to enumerate all K intervals in S that
 * intersect a given query interval x in time O(min{n, K log n}).
 * Couting the intersections without enumerating them could be done in
 * time O(log n), using binary search on two interval trees sorted
 * according to the upper and lower bounds, respectively.
 */
#ifndef INT_TREE_H
#define INT_TREE_H

#include <stdlib.h>
#include "interval.h"
#include "DDM_input_output.h"

/**
 * A node of the interval tree.
 */
struct int_node {
    const struct interval* in; /* The interval stored in this node */
    float max_upper; /* The maximum upper bound among all intervals
			stored in the subtree rooted at this node */
    float min_lower; /* the minimum lower bound among all intervals
			stored in the subtree rooted at this node */
    struct int_node *left, *right;
    int height; /* the maximum height of the subtree rooted at this
		   node. If this node has no childrens, its height is
		   zero. */
    int unbalance; /* height(left) - height(right) */
};

/**
 * The interval tree data structure.
 */
struct int_tree {
    struct int_node* root; /* pointer to the root of this tree; NULL
			      if the tree is empty */
    size_t size; /* number of nodes in this tree */
};

/**
 * Initializes an empty tree object.
 */
void int_tree_init( struct int_tree* tree );

/**
 * Removes all nodes from |tree|, freeing the corresponding allocated
 * memory.  Interval objects are NOT deallocated, only tree nodes are.
 */
void int_tree_clear( struct int_tree* tree );

/**
 * Returns the size (number of nodes) of tree |tree|.
 */
size_t int_tree_size( const struct int_tree* tree );

/**
 * Inserts the interval |q| into tree |tree|.
 */
void int_tree_insert( struct int_tree* tree, const struct interval* q );

typedef int(*int_callback)(const struct interval*, const struct interval*, void*);

/**
 * Enumerate all intervals in |tree| that intersect |q|. When a new
 * interval |x| intersecting |q| is found, this function calls the
 * user-defined callback |f(x,q,param)|. If |f| returns a nonzero
 * value, the search procedure terminates.
 */
size_t int_tree_find_intersect( uint_fast8_t **result, const struct int_tree* tree, const struct interval* q );

/**
 * For debug only
 */
void int_tree_check( const struct int_tree* tree );

/**
 * For debug only
 */
void int_tree_dump( const struct int_tree* tree );

/**
 * Removes the node containing interval |q| from the tree. If |q| is
 * not contained in the tree, does nothing. Note that only the tree
 * node containing |q| is deallocated; |q| itself is not altered in
 * any way.
 */
void int_tree_delete( struct int_tree* tree, const struct interval* q );

#endif
