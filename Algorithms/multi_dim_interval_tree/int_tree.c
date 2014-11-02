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

void MAXA( float *max_a1, float *max_a2, size_t dimensions){
  int i;
  
  for (i = 0; i < dimensions; ++i){
    max_a1[i] = (max_a1[i] > max_a2[i] ? max_a1[i] : max_a2[i]);
  }
}

void MINA( float *min_a1,  float *min_a2, size_t dimensions){
  int i;
  
  for (i = 0; i < dimensions; ++i){
    min_a1[i] = (min_a1[i] < min_a2[i] ? min_a1[i] : min_a2[i]);
  }
}

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
static int int_node_update( struct int_node* p, size_t dimensions )
{
    assert( p );
    int result = 0;
    float new_max_upper[MAX_DIMENSION];
    float new_min_lower[MAX_DIMENSION];
    int new_height = 0;
    int new_unbalance = 0; /* height(left subtree) - height(right subtree) */

     struct int_node* l = p->left;
     struct int_node* r = p->right;
    
    size_t i;
    
    //init the new max upper and new min lower
    for (i = 0; i < dimensions; ++i){
	new_max_upper[i] = p->in->upper[i];
	new_min_lower[i] = p->in->lower[i];
    }
    
    if ( l ) {
	//max between two array but the result is inside new_max_upper
	MAXA(new_max_upper, l->max_upper, dimensions);
	MINA(new_min_lower, l->min_lower, dimensions);
	new_height = MAX(new_height, 1 + l->height );
	new_unbalance += 1 + l->height;
    }
    if ( r ) {
	MAXA(new_max_upper, r->max_upper, dimensions);
	MINA(new_min_lower, r->min_lower, dimensions);
	new_height = MAX(new_height, 1 + r->height );
	new_unbalance -= 1 + r->height;
    }
    assert( abs(new_unbalance) <= 2 );
    
    result = \
	(new_height != p->height) ||
	(new_unbalance != p->unbalance);
    if (result != 0)
      for (i = 0; i < dimensions; ++i){
	if (new_max_upper[i] != p->max_upper[i] || new_min_lower[i] != p->min_lower[i]){
	  result = 0;
	  break;
	}
      }
    
    
    for (i = 0; i < dimensions; ++i){
      p->max_upper[i] = new_max_upper[i];
      p->min_lower[i] = new_min_lower[i];
    }
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
static struct int_node* int_node_rotate_right( struct int_node* n, size_t dimensions )
{
    assert( n );
    struct int_node* w = n->left;
    struct int_node* T2 = w->right;
    assert( w );
    n->left = T2;
    w->right = n;
    /* update ancillary node information */
    int_node_update( n, dimensions );
    int_node_update( w, dimensions );
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
static struct int_node* int_node_rotate_left( struct int_node* n, size_t dimensions )
{
    assert( n );
    struct int_node* w = n->right;
    struct int_node* T2 = w->left;
    assert( w );
    n->right = T2;
    w->left = n;
    /* update ancillary node information */
    int_node_update( n, dimensions );
    int_node_update( w, dimensions );
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
static struct int_node* int_node_balance( struct int_node* n, size_t dimensions )
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
	    result = int_node_rotate_right(n, dimensions);
	    assert( result == l );
	} else {
	    /* double rotation */
	    n->left = int_node_rotate_left(l, dimensions);
	    result = int_node_rotate_right(n, dimensions);
	}
    } else { /* right leaning case */
	struct int_node* r = n->right; /* right child */	
	assert(r);
	assert( r->unbalance == -1 || r->unbalance == +1 );
	if ( r->unbalance == -1 ) {
	    /* single rotation */
	    result = int_node_rotate_left(n, dimensions);
	    assert( result == r );
	} else {
	    /* double rotation */
	    n->right = int_node_rotate_right(r, dimensions);
	    result = int_node_rotate_left(n, dimensions);
	}
    }
    return result;
}

/**
 * Compare intervals x and y. Intervals are sorted according to their
 * lower bound; ties are broken by considering also the upper
 * bound.
 */
/*static int int_compare( const struct interval* x, const struct interval* y )
{
    if ( x == y ) return 0;

    if ( x->lower != y->lower) 
	return SGN(x->lower - y->lower);
    else
	return SGN(x->upper - y->upper);
}*/

static int int_compare( const struct interval* x, const struct interval* y, size_t dimensions )
{
    int result = 0;
    int i;
  
    if ( x == y ) return 0;
    
    for (i = 0; i < dimensions; ++i){
      result += (x->lower[i] - y->lower[i]);
    }
    if (result != 0.0)
      for (i = 0; i < dimensions; ++i){
	result += (x->upper[i] - y->upper[i]);
      }
    
    if (result < 0.0)
      result = -1;
    else if (result > 0.0)
      result = 1;
    else
      result = 0;
    
    return result;
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

static struct int_node* int_tree_insert_rec( struct int_node* n, const struct interval* interv, size_t dimensions )
{
    int i;
    struct int_node* result = n;
    if ( n == NULL ) {
	/* Create and initialize new node */
	result = (struct int_node*)malloc( sizeof *result );
	assert( result );
	result->in = interv;
	result->left = result->right = NULL;
	for (i = 0; i < dimensions; ++i){
	  result->max_upper[i] = interv->upper[i];
	  result->min_lower[i] = interv->lower[i];
	}
	result->height = result->unbalance = 0;
    } else {
	if ( int_compare( interv, n->in, dimensions ) < 0 )
	    result->left = int_tree_insert_rec( result->left, interv, dimensions );
	else
	    result->right = int_tree_insert_rec( result->right, interv, dimensions );

	/* rebalance if necessary */
	if ( int_node_update( result, dimensions ) )
	    result = int_node_balance( result, dimensions );
    }
    return result;
}

/* inserts the interval |interv| into tree |tree| */
void int_tree_insert( struct int_tree* tree, const struct interval* q, size_t dimensions )
{
    assert( tree );
    assert( q );

    tree->root = int_tree_insert_rec( tree->root, q, dimensions );
    tree->size++;
}

/* returns a pointer to the node containing interval |q|; if not
   present, return NULL */
struct int_node* int_tree_find( struct int_tree* tree, const struct interval* q, size_t dimensions )
{
    struct int_node* result = tree->root;
    while ( result && int_compare(result->in, q, dimensions ) ) {
	if ( int_compare(q, result->in, dimensions ) > 0 )
	    result = result->right;
	else
	    result = result->left;
    }
    return result;
}

/* recursively delete the node containing interval |q| from the
   subtree rooted at |n|; returns the (possibly new) root of the tree
   previously rooted at |n|. If interval |q| is found and deleted, set
   |*found| to 1; otherwise |*found| is left unchanged. */
static struct int_node* int_tree_delete_rec( struct int_node* n, const struct interval* q, int *found, size_t dimensions )
{
    if ( n == NULL )
	return n;

    struct int_node* result = n;
    int c = int_compare( q, n->in, dimensions );
    switch(c) {
    case 0:
	*found = 1;
	if ( n->left == NULL ) {
	    /* Case 1: empty left subtree */
	    result = n->right;
	    free( n );
	} else {
	    if ( n->right == NULL ) {
		/* Case 2: empty right subtree */
		result = n->left;
		free( n );
	    } else {
		/* Case 3: both the right and left subtrees are not
		   empty. We found and delete the rightmost node in
		   the left subtree. */
		struct int_node* max = n->left;
		while ( max->right )
		    max = max->right;

		result->in = max->in;
		result->left = int_tree_delete_rec( n->left, max->in, found, dimensions );
	    }
	}
	break;
    case 1:
	n->right = int_tree_delete_rec( n->right, q, found, dimensions );
	break;
    case -1:
	n->left = int_tree_delete_rec( n->left, q, found, dimensions );
	break;
    }

    /* rebalance if necessary */
    if ( int_node_update( result, dimensions ) )
	result = int_node_balance( result, dimensions );

    return result;    
}

/* removes the interval |interv| from the tree |tree|. Note that
   |interv| is not deallocated. If |interv| is not present, nothing is
   done. */
void int_tree_delete( struct int_tree* tree, const struct interval* interv, size_t dimensions )
{
    int found = 0;
    tree->root = int_tree_delete_rec( tree->root, interv, &found, dimensions );
    if ( found ) tree->size--;
}

static size_t find_intersect_rec( const struct int_node* n, const struct interval* q, int_callback f, void* param, size_t dimensions )
{
    float test = 0.0;
    int i;
    
    if ( ! n )
	return 0;

    /* If the lower bound of |q| is larger than the upper bound of any
       node in the subtree rooted at |n|, then there can't be any match */
    for (i = 0; i < dimensions; ++i)
      test += (q->lower[i] - n->max_upper[i]);
    if (test >= 0.0)
      return 0;
    test = 0.0;
    for (i = 0; i < dimensions; ++i)
      test += (q->upper[i] - n->min_lower[i]);
    if (test < 0.0)
      return 0;
    /*if ( q->lower >= n->max_upper || q->upper < n->min_lower )
	return 0;*/

    /* Check left children */
    size_t result = find_intersect_rec( n->left, q, f, param, dimensions );

    if ( intersect( n->in, q, dimensions ) ) {
	result++;
	if ( f( n->in, q, param) ) return result;
    }
    
    test = 0.0;
    for (i = 0; i < dimensions; ++i)
      test += (q->upper[i] - n->in->lower[i]);
    if ( test > 0.0 )
	result += find_intersect_rec( n->right, q, f, param, dimensions );    

    return result;
}

size_t int_tree_find_intersect( const struct int_tree* tree, const struct interval* interv, int_callback f, void* param, size_t dimensions )
{
    return find_intersect_rec( tree->root, interv, f, param, dimensions );
}

static void int_tree_check_rec( struct int_node* n, float *max_upper, size_t dimensions )
{
    int result = 0;
    int i;
    if ( ! n )
	return;

    for (i = 0; i < dimensions; ++i){
      result += (n->in->upper[i] - max_upper[i]);
    }
    
    if (result <= 0.0)
      result = 1;
    else
      result = 0;
    
    assert( result );

    if ( n->left ) {
	int_tree_check_rec( n->left, n->max_upper, dimensions );
    }
    if ( n->right ) {
	int_tree_check_rec( n->right, n->max_upper, dimensions );
    }
}

void int_tree_check( const struct int_tree* tree, size_t dimensions )
{
    if ( tree->root )
	int_tree_check_rec( tree->root, tree->root->max_upper, dimensions );
}

static int int_tree_dump_rec( const struct int_node* n, int l )
{
    int i;
    int c = 0;
    for (i=0; i<l; ++i)
	printf(" ");

    if ( !n ) {
	printf("()\n");
    } else {
	//printf("%d:(%f, %f) max_upper=%f h=%d unb=%d\n",n->in->id, n->in->lower, n->in->upper, n->max_upper, n->height, n->unbalance);
	c = 1;
	c += int_tree_dump_rec(n->left,l+1);
	c += int_tree_dump_rec(n->right,l+1);
    }

    return c;
}

#pragma GCC diagnostic ignored "-Wunused-variable"
void int_tree_dump( const struct int_tree* tree )
{
    int c = int_tree_dump_rec( tree->root, 0 );
    assert( c == tree->size );
}
