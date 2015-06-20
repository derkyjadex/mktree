/*
 * Copyright 2015 Ben Mather, James Deery
 * Released under the MIT license
 * See COPYING for details.
 */

#include <stdio.h>

#include "tree.h"

int main(int argc, char *argv[])
{
    Branch *tree = make_tree();
    if (!tree) {
        fprintf(stderr, "Error making tree");
        return 1;
    }

    size_t len;
    char *result = render_tree(tree, &len);
    free_tree(tree);
    if (!result) {
        fprintf(stderr, "Error rending tree");
        return 1;
    }

    printf("%s", result);

    return 0;
}
