#ifndef SDS_UTILS_VECTOR_H
#define SDS_UTILS_VECTOR_H

#include <stdlib.h>
#include <stdint.h>

#include "utils/macros.h"

typedef struct {
    intptr_t *data;
    int length;
} sds_vector_t;

SDS_PUBLIC sds_vector_t *sds_vector_init();
SDS_PUBLIC void sds_vector_push(sds_vector_t *, intptr_t);
SDS_PUBLIC void sds_vector_pop(sds_vector_t *);
SDS_PUBLIC void sds_vector_remove(sds_vector_t *, int);
SDS_PUBLIC intptr_t sds_vector_get(sds_vector_t *, int);
SDS_PUBLIC void sds_vector_set(sds_vector_t *, int, intptr_t);
SDS_PUBLIC void sds_vector_empty(sds_vector_t *);

#endif /* SDS_UTILS_VECTOR_H */
