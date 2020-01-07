#ifndef SDS_UTILS_RECT_H
#define SDS_UTILS_RECT_H

#include <stdint.h>
#include "utils/macros.h"

typedef struct {
    int16_t x;
    int16_t y;
} sds_pos_t;

typedef struct {
    uint16_t width;
    uint16_t height;
} sds_size_t;

typedef struct {
    int16_t x;
    int16_t y;

    uint16_t width;
    uint16_t height;
} sds_rect_t;

#endif /* SDS_UTILS_RECT_H */
