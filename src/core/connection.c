#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "core/connection.h"
#include "core/layers.h"
#include "core/property.h"
#include "utils/vector.h"
#include "utils/macros.h"
#include "utils/rect.h"
#include "utils/log.h"

SDS_PUBLIC sds_connection_t *sds_create_connection() {
    sds_connection_t *connection = calloc(1, sizeof(sds_connection_t));
    if(!connection) {
        sds_log_fatal("[CORE] Failed to allocate SDS connectin.");
        return NULL;
    }

    connection->displays = sds_vector_init();
    connection->inputs   = sds_vector_init();

    return connection;
}

SDS_PUBLIC sds_window_t *sds_create_window(sds_size_t *size, sds_layer_t layer) {
    sds_window_t *window = calloc(1, sizeof(sds_window_t));
    if(!window) {
        sds_log_error("[CORE] Cannot allocate window memory.");
        return NULL;
    }

    window->rect = calloc(1, sizeof(sds_rect_t));
    if(!window->rect) {
        sds_log_error("[CORE] Failed to allocate window size.");
        return NULL;
    }

    window->allowed_actions = SDS_ACTION_MOVE | SDS_ACTION_RESIZE | SDS_ACTION_CLOSE;
    window->rect->width = size->width;
    window->rect->height = size->height;
    window->layer = layer;

    window->properties = sds_vector_init();
    if(!window->properties) {
        sds_log_error("[CORE] Cannot allocate window property vector.");
        return NULL;
    }

    return window;
}
