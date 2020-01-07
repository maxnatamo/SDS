#include <stdlib.h>
#include <stdint.h>

#include "utils/log.h"
#include "utils/macros.h"
#include "utils/queue.h"

SDS_PUBLIC sds_queue_t *sds_queue_init(uint16_t node_size) {
    sds_queue_t *queue = calloc(1, sizeof(sds_queue_t));

    queue->length    = 0;
    queue->node_size = node_size;
    queue->data      = (void **) calloc(queue->length, queue->node_size * queue->length);

    return queue;
}

SDS_PUBLIC void sds_queue_push_top(sds_queue_t *queue, void *value) {
    queue->length++;
    queue->data = (void **) realloc(queue->data, queue->node_size * queue->length);
    queue->data[queue->length - 1] = value;
}

SDS_PUBLIC void sds_queue_push_bottom(sds_queue_t *queue, void *value) {
    queue->length++;
    queue->data = (void **) realloc(queue->data, queue->node_size * queue->length);
    for(int i = queue->length - 1; i > 0; i--) {
        queue->data[i] = queue->data[i - 1];
    }
    queue->data[0] = value;
}

SDS_PUBLIC void sds_queue_pop_top(sds_queue_t *queue) {
    queue->length--;
    queue->data = (void **) realloc(queue->data, queue->node_size * queue->length);
}

SDS_PUBLIC void sds_queue_pop_bottom(sds_queue_t *queue) {
    for(int i = 0; i < queue->length - 1; i++) {
        queue->data[i] = queue->data[i + 1];
    }
    queue->length--;
    queue->data = (void **) realloc(queue->data, queue->node_size * queue->length);
}
