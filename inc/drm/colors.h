#ifndef SDS_DRM_COLORS_H
#define SDS_DRM_COLORS_H

#include <stdint.h>
#include "utils/macros.h"

typedef union {
    struct {
        uint8_t red   : 3;
        uint8_t green : 3;
        uint8_t blue  : 2;
    };
    uint8_t full;
} sds_color8_t;

typedef union {
    struct {
        uint8_t red   : 5;
        uint8_t green : 5;
        uint8_t blue  : 5;
    };
    uint16_t full;
} sds_color15_t;

typedef union {
    struct {
        uint8_t red   : 5; 
        uint8_t green : 6; 
        uint8_t blue  : 5; 
    };
    uint16_t full;
} sds_color16_t;

typedef union {
    struct {
        uint8_t red   : 8;
        uint8_t green : 8;
        uint8_t blue  : 8;
    };
    uint32_t full;
} sds_color24_t;

typedef union {
    struct {
        uint8_t red   : 8;
        uint8_t green : 8;
        uint8_t blue  : 8;
        uint8_t alpha : 8;
    };
    uint32_t full;
} sds_color32_t;

typedef union {
    sds_color8_t  color8;
    sds_color15_t color15;
    sds_color16_t color16;
    sds_color24_t color24;
    sds_color32_t color32;
} sds_color_t;

#endif /* SDS_DRM_COLORS_H */
