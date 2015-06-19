/*
 * Copyright 2015 Ben Mather, James Deery
 * Released under the MIT license
 * See COPYING for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "tree.h"

static Branch *new_branch(float length, float thickness, float angle)
{
    Branch *branch = malloc(sizeof(Branch));
    if (!branch) {
        fprintf(stderr, "Bad malloc!");
        abort();
    }

    branch->length = length;
    branch->thickness = thickness;
    branch->angle = angle;
    branch->num_children = 0;

    return branch;
}

static float randf()
{
    return (random() % 1000) / 1000.0;
}

static float randf_uniform(float min, float max)
{
    return min + (max - min) * randf();
}

static void add_children(Branch *branch, float ttl)
{
    if (ttl <= 0.0)
        return;

    int num_children = 2 + random() % (MAX_CHILDREN - 2);

    for (int i = 0; i < num_children; i++) {
        float life = ttl - (0.08 + 0.05 / randf());

        float length;
        float thickness;

        if (life < 0) {
            length = 10;
            thickness = 1;
        } else {
            length = branch->length * 0.7;
            thickness = branch->thickness * randf_uniform(0.6, 0.7);
        }

        float angle = (1.0 - 0.7 * life) * M_PI * randf_uniform(-0.5, 0.5);

        Branch *child = new_branch(length, thickness, branch->angle - angle);

        branch->children[i] = child;
        branch->num_children++;

        add_children(child, life);
    }
}

Branch *make_tree()
{
    Branch *root = new_branch(100, 1, 0);
    add_children(root, 1);

    return root;
}

void free_tree(Branch *tree)
{
    for (int i = 0; i < tree->num_children; i++) {
        free_tree(tree->children[i]);
    }
    free(tree);
}

static void concatf(char **a, size_t *a_len, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    char *b;
    int b_len = vasprintf(&b, format, ap);
    if (b_len < 0)
        abort();

    size_t new_a_len = *a_len + b_len;
    char *new_a = realloc(*a, new_a_len + 1);
    if (!new_a)
        abort();

    memcpy(new_a + *a_len, b, b_len);
    new_a[new_a_len] = '\0';
    free(b);

    *a = new_a;
    *a_len = new_a_len;

    va_end(ap);
}

static void render_branch(char **result, size_t *len, Branch *branch, float x, float y)
{
    float x2 = x + branch->length * sinf(branch->angle);
    float y2 = y - branch->length * cosf(branch->angle);

    float h, s, l;
    if (branch->num_children == 0) {
        h = randf_uniform(115, 125);
        s = 80;
        l = randf_uniform(25, 60);
    } else {
        h = 30;
        s = 75;
        l = 40;
    }

    concatf(
        result, len,
        "<line x1='%f' y1='%f' x2='%f' y2='%f' "
        "stroke='hsl(%f, %f%%, %f%%)' "
        "stroke-width='%f'/>\n",
        x, y, x2, y2,
        h, s, l,
        branch->thickness * 10);

    for (int i = 0; i < branch->num_children; i++) {
        render_branch(result, len, branch->children[i], x2, y2);
    }
}

char *render_tree(Branch *tree, size_t *len)
{
    char *result = NULL;
    *len = 0;

    concatf(
        &result, len,
        "<?xml version='1.0'?>\n"
        "<svg version='1.1' width='1000' height='550' xmlns='http://www.w3.org/2000/svg'>\n");
    concatf(
        &result, len,
        "<text x='500' y='520'>Tree!</text>\n");

    render_branch(&result, len, tree, 500, 500);
    concatf(
        &result, len,
        "</svg>");

    return result;
}

