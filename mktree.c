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
    size_t len;
    char *result = render_tree(tree, &len);
    printf("%s", result);

    return 0;
}
