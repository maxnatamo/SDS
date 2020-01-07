#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "utils/vector.h"
#include "utils/macros.h"
#include "utils/log.h"

SDS_PUBLIC sds_vector_t *sds_vector_init() {
    sds_vector_t *vec = malloc(sizeof(sds_vector_t));
    if(!vec) {
        errno = ENOMEM;
        sds_log_error("[VEC] Couldn't allocate vector: %m");
        return NULL;
    }

    vec->length = 0;
    vec->data = calloc(vec->length, sizeof(intptr_t));
    if(!vec->data) {
        errno = ENOMEM;
        sds_log_error("[VEC] Couldn't allocate vector data: %m");
        return NULL;
    }
    return vec;
}

SDS_PUBLIC void sds_vector_push(sds_vector_t *vec, intptr_t data) {
    if(!vec) {
        sds_log_error("[VEC] Invalid vector.");
        errno = EINVAL;
        return;
    }

    vec->data = realloc(vec->data, (vec->length + 1) * sizeof(intptr_t));
    if(errno == ENOMEM) {
        sds_log_error("[VEC] Couldn't reallocate vector data: %m");
        return;
    }
    vec->length++;

    vec->data[vec->length - 1] = data;
}

SDS_PUBLIC void sds_vector_pop(sds_vector_t *vec) {
    if(!vec) {
        errno = EINVAL;
        sds_log_error("[VEC] Invalid vector.");
        return;
    }

    vec->data = realloc(vec->data, (vec->length - 1) * sizeof(intptr_t));
    if(errno == ENOMEM) {
        sds_log_error("[VEC] Couldn't reallocate vector data: %m");
        return;
    }
    vec->length--;
}

SDS_PUBLIC void sds_vector_remove(sds_vector_t *vec, int index) {
    if(!vec) {
        errno = EINVAL;
        sds_log_error("[VEC] Invalid vector.");
        return;
    }

    if(index < 0) {
        errno = EINVAL;
        sds_log_error("[VEC] Cannot remove data. Out of range. (%d > %d)", index, vec->length);
        return;
    }

    for(int i = index + 1; i < vec->length; i++) {
        if(i <= 0) {
            continue;
        }

        vec->data[i - 1] = vec->data[i];
    }

    vec->data = realloc(vec->data, (vec->length - 1) * sizeof(intptr_t));
    if(errno == ENOMEM) {
        sds_log_error("[VEC] Couldn't reallocate vector data: %m");
        return;
    }
    vec->length--;
}

SDS_PUBLIC intptr_t sds_vector_get(sds_vector_t *vec, int index) {
    if(!vec) {
        sds_log_error("[VEC] Invalid vector.");
        errno = EINVAL;
        return (intptr_t)NULL;
    }

    if(index > vec->length) {
        sds_log_error("[VEC] Cannot retrieve data. Out of range. (%d > %d)", index, vec->length);
        errno = ERANGE;
        return (intptr_t)NULL;
    }

    return vec->data[index];
}

SDS_PUBLIC void sds_vector_set(sds_vector_t *vec, int index, intptr_t value) {
    if(!vec) {
        sds_log_error("[VEC] Invalid vector.");
        errno = EINVAL;
        return;
    }

    if(index > vec->length) {
        sds_log_error("[VEC] Cannot retrieve data. Out of range. (%d > %d)", index, vec->length);
        errno = ERANGE;
        return;
    }

    vec->data[index] = value;
}

SDS_PUBLIC void sds_vector_empty(sds_vector_t *vec) {
    vec->length = 0;
    vec->data = realloc(vec->data, sizeof(intptr_t) * vec->length);
}

SDS_PUBLIC void sds_vector_free(sds_vector_t *vec) {
    sds_vector_empty(vec);
    free(vec);
}
