/*
 * Copyright 2015 Ben Mather, James Deery
 * Released under the MIT license
 * See COPYING for details.
 */

#include <stdlib.h>

#define MAX_CHILDREN 5

typedef struct Branch Branch;
struct Branch {
    float length;
    float thickness;
    float angle;

    struct Branch *children[MAX_CHILDREN];
    int num_children;
};

Branch *make_tree();
void free_tree(Branch *tree);
char* render_tree(Branch *tree, size_t *len);

