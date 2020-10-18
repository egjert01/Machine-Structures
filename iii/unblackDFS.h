//unblackDFS.h

#ifndef UNBLACKDFS_H
#define UNBLACKDFS_H

#include <stdio.h>
#include <stdlib.h>

#include "unblackedges.h"
#include "bit2.h"
#include "seq.h"

typedef struct bitNode bitNode;

/*
* Name: depthFirstSearch
* Arguments: (Bit2_T *bitmap) pointer to a Bit2_T containing the read-in
             pbm file
             (int x) the starting horizontal index for a black edge pixel
             found on the edge
             (int y) the starting vertical index for a black edge pixel found
             on the edge
* Returns: none
* Purpose: starting at given coordinate checks neighbors if is black bit.
            Continues ot check of neighbors of neighbors are black bits and
            so on. Alters given bitmap to have black edges removed.
            Makes a Bit2_T to track visited pixels while traversing, and
            a stack to keep track of neightbors. Frees stack and visited
            Bit2_T.
            Raises a CRE if the given bitmap is NULL, or any other error from
            this .c
*/
void depthFirstSearch(Bit2_T *bitmap, int x, int y);

/*
* Name: traverseGraph
* Arguments: (Seq_T *stack) pointer to a stack representing neightbors
             (int stack_len) the beginning length of the stack
             (Bit2_T *bitmap) pointer to bitmap being altered
             (Bit2_T *visited) pointer to represented visited bits
             (int *x) pointer to starting horizontal coordinate
             (int *y) pointer to starting vertical coordinate
* Returns: none
* Purpose: Traverses the bitmap to find all blackedge pixels and replace
            them with white pixels. Updates internal stack and values as
            needed.
            Raises a CRE if any of the input pointers to values passed
            by reference are NULL
*/
void traverseGraph(Seq_T *stack, int stack_len, Bit2_T *bitmap,
                   Bit2_T *visited, int *x, int *y);

/*
* Name: pushOnStack
* Arguments: (int x) horizontal coordinate of bit
            (int y) vertical coordinate of bit
            (Seq_T *stack) pointer to stack representation
            (int  *stack_len) length of stack passed by reference
* Returns: none
* Purpose: makes a new bitNode instance and pushes it on the stack,
          updating the length as necessary.
          Raises a CRE if the pointers to the values passed by reference
          are NULL
*/
void pushOnStack(int x, int y, Seq_T *stack, int *stack_len);

/*
* Name: makeStruct
* Arguments: (int x) horizontal coordinate of bit
             (int y) vertical coordinate of bit
* Returns: a pointer to a struct bitNode which represents a bit. Members
           include the coordinates of the bit
* Purpose: initializes a new instance of a bitNode pointer, assigns
            values to its members, and returns the new bitNode *
*/
bitNode *makeStruct(int x, int y);

/*
* Name: shouldPush
* Arguments: bitmap, a Bit2_T, an x and a y value representing coordinates on
*            the bitmap, and a Bit2_T that has marked on the grid which
*            coordinates have already been visited
* Returns: true if neighboring pixel is a blackedge, false otherwise true if the given x and y coordinates are in-bounds, if the value
*          at the x and y coordinates is 1, and if it hasn't been visited yet,
*          false if not
* Purpose: check to see if a struct with the given x and y values should be
*          pushed on the stack (requirements mentioned above)
*/
bool shouldPush(int x, int y, Bit2_T bitmap, Bit2_T visited);

/*
* Name: outOfBounds outOfBounds
* Arguments: bitmap, the Bit2_T, and two integers representing coordinates on
*            on the grid
* Returns: false if the given x and y exist on the bitmap, true if not
* Purpose: check if the given coordinates are out of bounds
*/
bool outOfBounds(Bit2_T bitmap, int x, int y);


#endif
