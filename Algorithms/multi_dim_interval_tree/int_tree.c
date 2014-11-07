/****************************************************************************
 *
 * int_tree.c
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

#include "int_tree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define MAX_DEPTH 32

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

/* "branchless" signum computation: SGN(a) = 1 if a>0, -1 if a<0 and 0 if a==0 */
#define SGN(a) (((a)>0) - ((a)<0))

/**
 * Update the following information stored within node |p|:
 *
 * - the height of the subtree rooted at |p|;
 *
 * - the maximum value |p->max_upper| of the upper bounds of all
 *   intervals stored in the subtree rooted at |p|;
 *
 * - the unbalance factor of the subtree rooted at |p|.
 *
 * Returns 0 if no modification has been applied to node |p|, 1
 * otherwise. Therefore, if this function returns 1, the caller MUST
 * update the information stored at the parent of |p|, if it exists.
 */
static int int_node_update( struct int_node* p, uint16_t current_dim )
{
    assert( p );
    int result = 0;
    float new_max_upper = p->in->upper[current_dim];
    float new_min_lower = p->in->lower[current_dim];
    int new_height = 0;
    int new_unbalance = 0; /* height(left subtree) - height(right subtree) */

    const struct int_node* l = p->left;
    const struct int_node* r = p->right;
    if ( l ) {
	new_max_upper = MAX(new_max_upper, l->max_upper);
	new_min_lower = MIN(new_min_lower, l->min_lower);
	new_height = MAX(new_height, 1 + l->height );
	new_unbalance += 1 + l->height;
    }
    if ( r ) {
	new_max_upper = MAX(new_max_upper, r->max_upper);
	new_min_lower = MIN(new_min_lower, r->min_lower);
	new_height = MAX(new_height, 1 + r->height );
	new_unbalance -= 1 + r->height;
    }
    assert( abs(new_unbalance) <= 2 );
    result = \
	(new_max_upper != p->max_upper) ||
	(new_min_lower != p->min_lower ) ||
	(new_height != p->height) ||
	(new_unbalance != p->unbalance);
    p->max_upper = new_max_upper;
    p->min_lower = new_min_lower;
    p->height = new_height;
    p->unbalance = new_unbalance;
    return result;
}

/**
 * Perform a right rotation of the subtree rooted at |n|, as follows:
 *
 *      n             w
 *     / \           / \
 *    w   T3  ==>  T1   n
 *   / \               / \
 * T1   T2           T2   T3
 *
 * This function also updates the local information stored at |n| and |w|.
 * Returns the new root |w| of the subtree.
 *
 */
static struct int_node* int_node_rotate_right( struct int_node* n, uint16_t current_dim )
{
    assert( n );
    struct int_node* w = n->left;
    struct int_node* T2 = w->right;
    assert( w );
    n->left = T2;
    w->right = n;
    /* update ancillary node information */
    int_node_update( n, current_dim );
    int_node_update( w, current_dim );
    return w;
}

/**
 * Perform a left rotation of the subtree rooted at |n|, as follows:
 *
 *      w             n
 *     / \           / \
 *    n   T3  <==  T1   w
 *   / \               / \
 * T1   T2           T2   T3
 *
 * This function also updates the local information stored at |n| and |w|.
 * Returns the new root |w| of the subtree.
 */
static struct int_node* int_node_rotate_left( struct int_node* n, uint16_t current_dim )
{
    assert( n );
    struct int_node* w = n->right;
    struct int_node* T2 = w->left;
    assert( w );
    n->right = T2;
    w->left = n;
    /* update ancillary node information */
    int_node_update( n, current_dim );
    int_node_update( w, current_dim );
    return w;
}

/**
 * Rebalance, if necessary, the subtree rooted at |n| by performing
 * the appropriate rotations. If |n| is already balanced, nothing is
 * done.
 *
 * Returns a pointer to the (possibly new) root |r| of the subtree
 * previously rooted at |n|.
 */
static struct int_node* int_node_balance( struct int_node* n, uint16_t current_dim )
{
    if ( abs(n->unbalance) <= 1 )
	return n; /* already balanced */

    struct int_node* result;
    assert( n->unbalance == -2 || n->unbalance == +2 );

    if ( n->unbalance == +2 ) { /* left leaning case */
	struct int_node* l = n->left; /* left child */
	assert(l);
	assert( l->unbalance == -1 || l->unbalance == +1 );
	if ( l->unbalance == +1 ) {
	    /* single rotation */
	    result = int_node_rotate_right(n, current_dim);
	    assert( result == l );
	} else {
	    /* double rotation */
	    n->left = int_node_rotate_left(l, current_dim);
	    result = int_node_rotate_right(n, current_dim);
	}
    } else { /* right leaning case */
	struct int_node* r = n->right; /* right child */
	assert(r);
	assert( r->unbalance == -1 || r->unbalance == +1 );
	if ( r->unbalance == -1 ) {
	    /* single rotation */
	    result = int_node_rotate_left(n, current_dim);
	    assert( result == r );
	} else {
	    /* double rotation */
	    n->right = int_node_rotate_right(r, current_dim);
	    result = int_node_rotate_left(n, current_dim);
	}
    }
    return result;
}

/**
 * Compare intervals x and y. Intervals are sorted according to their
 * lower bound; ties are broken by considering also the upper
 * bound.
 */
static int int_compare( const struct interval* x, const struct interval* y )
{
    if ( x == y ) return 0;

    if ( x->lower != y->lower)
	return SGN(x->lower - y->lower);
    else
	return SGN(x->upper - y->upper);
}

void int_tree_init( struct int_tree* tree )
{
    tree->root = NULL;
    tree->size = 0;
}

static void int_tree_clear_rec( struct int_node* n )
{
    if ( n == NULL )
	return;

    int_tree_clear_rec( n->left );
    int_tree_clear_rec( n->right );
    free( n );
}

void int_tree_clear( struct int_tree* tree )
{
    if ( tree == NULL )
	return;

    int_tree_clear_rec( tree->root );
    tree->root = NULL;
    tree->size = 0;
}

size_t int_tree_size( const struct int_tree* tree )
{
    assert( tree );
    return tree->size;
}

static struct int_node* int_tree_insert_rec( struct int_node* n, const struct interval* interv, size_t current_dim )
{
    struct int_node* new_node = n;
    if ( n == NULL ) {
	/* Create and initialize new node */
        new_node = (struct int_node*)malloc( sizeof( struct int_node) );
        assert( new_node );
        new_node->in = interv;
        new_node->left = new_node->right = NULL;
        new_node->max_upper = interv->upper[current_dim];
        new_node->min_lower = interv->lower[current_dim];
        new_node->height = new_node->unbalance = 0;
    } else {
	if ( int_compare( interv, n->in ) < 0 )
	    new_node->left = int_tree_insert_rec( new_node->left, interv, current_dim );
	else
	    new_node->right = int_tree_insert_rec( new_node->right, interv, current_dim );

	/* rebalance if necessary */
	if ( int_node_update( new_node, current_dim ) )
	    new_node = int_node_balance( new_node, current_dim );
    }
    return new_node;
}

/* inserts the interval |interv| into tree |tree| */
void int_tree_insert( struct int_tree* tree, const struct interval* q, size_t current_dim )
{
    assert( tree );
    assert( q );

    tree->root = int_tree_insert_rec( tree->root, q, current_dim );
    tree->size++;
}

/* returns a pointer to the node containing interval |q|; if not
   present, return NULL */
struct int_node* int_tree_find( struct int_tree* tree, const struct interval* q )
{
    struct int_node* new_node = tree->root;
    while ( new_node && int_compare(new_node->in, q ) ) {
	if ( int_compare(q, new_node->in ) > 0 )
	    new_node = new_node->right;
	else
	    new_node = new_node->left;
    }
    return new_node;
}

/* recursively delete the node containing interval |q| from the
   subtree rooted at |n|; returns the (possibly new) root of the tree
   previously rooted at |n|. If interval |q| is found and deleted, set
   |*found| to 1; otherwise |*found| is left unchanged. */
static struct int_node* int_tree_delete_rec( struct int_node* n, const struct interval* q, int *found, uint16_t current_dim )
{
    if ( n == NULL )
	return n;

    struct int_node* new_node = n;
    int c = int_compare( q, n->in );
    switch(c) {
    case 0:
	*found = 1;
	if ( n->left == NULL ) {
	    /* Case 1: empty left subtree */
	    new_node = n->right;
	    free( n );
	} else {
	    if ( n->right == NULL ) {
		/* Case 2: empty right subtree */
		new_node = n->left;
		free( n );
	    } else {
		/* Case 3: both the right and left subtrees are not
		   empty. We found and delete the rightmost node in
		   the left subtree. */
		struct int_node* max = n->left;
		while ( max->right )
		    max = max->right;

		new_node->in = max->in;
		new_node->left = int_tree_delete_rec( n->left, max->in, found, current_dim );
	    }
	}
	break;
    case 1:
	n->right = int_tree_delete_rec( n->right, q, found, current_dim );
	break;
    case -1:
	n->left = int_tree_delete_rec( n->left, q, found, current_dim );
	break;
    }

    /* rebalance if necessary */
    if ( int_node_update( new_node, current_dim ) )
	new_node = int_node_balance( new_node, current_dim );

    return new_node;
}

/* removes the interval |interv| from the tree |tree|. Note that
   |interv| is not deallocated. If |interv| is not present, nothing is
   done. */
void int_tree_delete( struct int_tree* tree, const struct interval* interv, uint16_t current_dim )
{
    int found = 0;
    tree->root = int_tree_delete_rec( tree->root, interv, &found, current_dim );
    if ( found ) tree->size--;
}

int set_result(uint_fast8_t **result, const struct interval* x, const struct interval* q){

    result[x->id][q->id] = 1;
    return 0;
}

static size_t find_intersect_rec( uint_fast8_t **result_matrix, const struct int_node* n, const struct interval* q, uint16_t current_dim )
{
    if ( ! n )
	return 0;

    /* If the lower bound of |q| is larger than the upper bound of any
       node in the subtree rooted at |n|, then there can't be any match */
    if ( q->lower[current_dim] >= n->max_upper || q->upper[current_dim] < n->min_lower )
	return 0;

    /* Check left children */
    size_t result = find_intersect_rec( result_matrix, n->left, q, current_dim );

    if ( intersect( n->in, q, current_dim ) ) {
	result++;
	if ( set_result(result_matrix, q, n->in) ) return result;
    }

    if ( q->upper > n->in->lower )
	result += find_intersect_rec( result_matrix, n->right, q, current_dim );

    return result;
}

size_t int_tree_find_intersect( uint_fast8_t **result, const struct int_tree* tree, const struct interval* interv, uint16_t current_dim )
{
    return find_intersect_rec( result, tree->root, interv, current_dim );
}

static void int_tree_check_rec( const struct int_node* n, float max_upper, uint16_t current_dim )
{
    if ( ! n )
	return;

    assert( n->in->upper[current_dim] <= max_upper );

    if ( n->left ) {
	int_tree_check_rec( n->left, n->max_upper, current_dim );
    }
    if ( n->right ) {
	int_tree_check_rec( n->right, n->max_upper, current_dim );
    }
}

void int_tree_check( const struct int_tree* tree, uint16_t current_dim )
{
    if ( tree->root )
	int_tree_check_rec( tree->root, tree->root->max_upper, current_dim );
}

static int int_tree_dump_rec( const struct int_node* n, int l, uint16_t current_dim )
{
    int i;
    int c = 0;
    for (i=0; i<l; ++i)
	printf(" ");

    if ( !n ) {
	printf("()\n");
    } else {
	printf("%d:(%f, %f) max_upper=%f h=%d unb=%d\n",n->in->id, n->in->lower[current_dim], n->in->upper[current_dim], n->max_upper, n->height, n->unbalance);
	c = 1;
	c += int_tree_dump_rec(n->left,l+1, current_dim);
	c += int_tree_dump_rec(n->right,l+1, current_dim);
    }

    return c;
}

#pragma GCC diagnostic ignored "-Wunused-variable"
void int_tree_dump( const struct int_tree* tree, uint16_t current_dim )
{
    int c = int_tree_dump_rec( tree->root, 0, current_dim );
    assert( c == tree->size );
}
