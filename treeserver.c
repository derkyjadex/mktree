/*
 * Copyright 2015 James Deery
 * Released under the MIT license
 * See COPYING for details.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "tree.h"
#include "civetweb.h"

static int return_error(struct mg_connection *conn)
{
    fprintf(stderr, "Error");
    mg_printf(
            conn,
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 10\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Oh dear :/");

    return 1;
}

static int handle_root(struct mg_connection *conn, void *cbdata)
{
    Branch *tree = make_tree();
    if (!tree)
        return return_error(conn);

    size_t svg_len;
    char *svg = render_tree(tree, &svg_len);
    free_tree(tree);
    if (!svg)
        return return_error(conn);

    mg_printf(
            conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: image/svg+xml\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            svg_len, svg);
    free(svg);

    return 1;
}

int main(int argc, char *argv[])
{
    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    char *port;
    if (argc >= 2) {
        port = argv[1];
    } else {
        port = "20185";
    }

    const char *options[] = {
        "listening_ports", port,
        NULL
    };

    struct mg_context *ctx = mg_start(&callbacks, NULL, options);
    if (!ctx) {
        fprintf(stderr, "Error starting Civetweb\n");
        return 1;
    }

    mg_set_request_handler(ctx, "/$", handle_root, NULL);

    printf("Listening on port %s\n", port);

    while (true) {
        sleep(1000);
    }

    return 0;
}
