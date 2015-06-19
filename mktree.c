/*
 * Copyright 2015 Ben Mather, James Deery
 * Released under the MIT license
 * See COPYING for details.
 */

#include "tree.h"

int main(int argc, char *argv[])
{
    Branch *tree = make_tree();
    render_tree(tree);
    return 0;
}
