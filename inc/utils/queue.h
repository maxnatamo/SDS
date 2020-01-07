#ifndef SDS_UTILS_QUEUE_H
#define SDS_UTILS_QUEUE_H

#include <stdint.h>
#include "utils/macros.h"

typedef struct {
    void **data;
    uint16_t node_size;
    uint16_t length;
} sds_queue_t;

SDS_PUBLIC sds_queue_t *sds_queue_init(uint16_t);
SDS_PUBLIC void sds_queue_push_top(sds_queue_t *, void *);
SDS_PUBLIC void sds_queue_push_bottom(sds_queue_t *, void *);
SDS_PUBLIC void sds_queue_pop_top(sds_queue_t *);
SDS_PUBLIC void sds_queue_pop_bottom(sds_queue_t *);

#endif /* SDS_UTILS_QUEUE_H */
