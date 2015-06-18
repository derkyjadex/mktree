#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


const int MAX_CHILDREN = 5;

typedef struct Branch Branch;
struct Branch {
    float length;
    float thickness;
    float angle;

    struct Branch *children[MAX_CHILDREN];
    int num_children;
};

Branch *new_branch(float length, float thickness)
{
    Branch *branch = malloc(sizeof(Branch));
    if (!branch) {
        fprintf(stderr, "Bad malloc!");
        abort();
    }

    branch->length = length;
    branch->thickness = thickness;
    branch->angle = 0;
    branch->num_children = 0;

    return branch;
}
void add_fake_children(Branch *branch, int ttl)
{
    if (ttl == 0)
        return;

    Branch *a = new_branch(branch->length * 0.7, branch->thickness * 0.7);
    a->angle = branch->angle - M_PI / 4.0;

    Branch *b = new_branch(branch->length * 0.6, branch->thickness * 0.9);
    b->angle = branch->angle + M_PI / 4.0;

    branch->children[0] = a;
    branch->children[1] = b;
    branch->num_children = 2;

    add_fake_children(a, ttl - 1);
    add_fake_children(b, ttl - 1);
}

Branch *fake_tree()
{
    Branch *root = new_branch(100, 10);
    add_fake_children(root, 6);

    return root;
}

void render_branch(Branch *branch, float x, float y)
{
    float x2 = x + branch->length * sinf(branch->angle);
    float y2 = y - branch->length * cosf(branch->angle);

    float h, s, l;
    if (branch->num_children == 0) {
        h = 120;
        s = 100;
        l = 50;
    } else {
        h = 30;
        s = 75;
        l = 40;
    }
    
    printf(
            "<line x1='%f' y1='%f' x2='%f' y2='%f'"
            "stroke='hsl(%f, %f%%, %f%%)'"
            "stroke-width='%f'/>\n",
            x, y, x2, y2,
            h, s, l,
            branch->thickness);

    for (int i = 0; i < branch->num_children; i++) {
        render_branch(branch->children[i], x2, y2);
    }
}

void render_tree(Branch *tree)
{
    printf(
        "<?xml version='1.0'?>\n"
        "<svg version='1.1' width='1000' height='550' xmlns='http://www.w3.org/2000/svg'>\n"
    );
    printf("<text x='500' y='520'>Tree!</text>\n");

    render_branch(tree, 500, 500);
    printf(
        "</svg>"
    );

}

int main(int argc, char *argv[])
{
    Branch *tree = fake_tree();
    render_tree(tree);
    return 0;
}
