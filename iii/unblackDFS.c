/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           unblackDFS.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           10/02/2020 (1 late token)
 *
 *                               iii
 *
 *     Use depthFirstSearch to replace all black edge pixles to white
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "unblackDFS.h"
#include "unblackedges.h"
#include "bit2.h"
#include "seq.h"

struct bitNode {
    int x;
    int y;
};


void depthFirstSearch (Bit2_T *bitmap, int x, int y)
{
    assert(bitmap != NULL);
    int width = Bit2_width(*bitmap);
    int height = Bit2_height(*bitmap);

    Bit2_T visited = Bit2_new(width, height);

    Seq_T stack = Seq_new(0);
    bitNode *node = makeStruct(x, y);
    Seq_addlo(stack, node);

    int stack_len = Seq_length(stack);

    traverseGraph(&stack, stack_len, bitmap, &visited, &x, &y);

    Seq_free(&stack);
    Bit2_free(&visited);
}

void traverseGraph(Seq_T *stack, int stack_len, Bit2_T *bitmap,
                   Bit2_T *visited, int *x, int *y)
{
    assert(stack != NULL && bitmap != NULL && visited != NULL
        && visited != NULL && x != NULL && y != NULL);

    while (stack_len != 0) {
        bitNode *curr_node = (bitNode *)Seq_remlo(*stack);

        stack_len = Seq_length(*stack);
        *x = curr_node->x;
        *y = curr_node->y;

        Bit2_put(*bitmap, *x, *y, 0);
        Bit2_put(*visited, *x, *y, 1);

        if (shouldPush(*x, *y - 1, *bitmap, *visited)) {
            pushOnStack(*x, *y - 1, stack, &stack_len);
        }
        if (shouldPush(*x + 1, *y, *bitmap, *visited)) {
            pushOnStack(*x + 1, *y, stack, &stack_len);
        }
        if (shouldPush(*x, *y + 1, *bitmap, *visited)) {
            pushOnStack(*x, *y + 1, stack, &stack_len);
        }
        if (shouldPush(*x - 1, *y, *bitmap, *visited)) {
            pushOnStack(*x - 1, *y, stack, &stack_len);
        }
        free(curr_node);
    }
}

void pushOnStack(int x, int y, Seq_T *stack, int *stack_len)
{
    assert(stack != NULL && stack_len != NULL);

    bitNode *node = makeStruct(x, y);
    Seq_addlo(*stack, node);
    *stack_len =+ 1;
}

bitNode *makeStruct(int x, int y)
{
    bitNode *node = malloc(sizeof(struct bitNode));
    node->x = x;
    node->y = y;
    return node;
}

bool shouldPush(int x, int y, Bit2_T bitmap, Bit2_T visited)
{
    if (outOfBounds(bitmap, x, y)
        || Bit2_get(bitmap, x, y) == 0
        || Bit2_get(visited, x, y) == 1) {

        return false;
    }
    return true;
}

bool outOfBounds(Bit2_T bitmap, int x, int y)
{
    if ( x < 0 || x >= Bit2_width(bitmap)
        || y < 0 || y >= Bit2_height(bitmap) ) {

        return true;
    }
    return false;
}
