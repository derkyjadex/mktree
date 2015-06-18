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

float randf()
{
    return (random() % 1000) / 1000.0;
}

float randf_uniform(float min, float max)
{
    return min + (max - min) * randf();
}

void add_children(Branch *branch, float ttl)
{
    if (ttl <= 0.0)
        return;

    int num_children = 2 + random() % (MAX_CHILDREN - 2);

    for (int i = 0; i < num_children; i++) {
        float life = ttl - randf_uniform(0.1, 0.4);

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

        Branch *child = new_branch(length, thickness);
        child->angle = branch->angle - angle;

        branch->children[i] = child;
        branch->num_children++;

        add_children(child, life);
    }
}

Branch *make_tree()
{
    Branch *root = new_branch(100, 1);
    add_children(root, 1);

    return root;
}

void render_branch(Branch *branch, float x, float y)
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

    printf(
            "<line x1='%f' y1='%f' x2='%f' y2='%f' "
            "stroke='hsl(%f, %f%%, %f%%)' "
            "stroke-width='%f'/>\n",
            x, y, x2, y2,
            h, s, l,
            branch->thickness * 10);

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
    Branch *tree = make_tree();
    render_tree(tree);
    return 0;
}
