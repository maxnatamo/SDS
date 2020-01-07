#ifndef SDS_CORE_CONNECTION_H
#define SDS_CORE_CONNECTION_H

#include "core/layers.h"
#include "drm/modesetting.h"
#include "input/core.h"
#include "utils/vector.h"
#include "utils/macros.h"
#include "utils/rect.h"

typedef struct {
    sds_rect_t *rect;
    sds_layer_t layer;
    sds_vector_t *properties;
    uint32_t allowed_actions;
} sds_window_t;

typedef struct {
    sds_display_device_t **display_devices;
    sds_input_device_t **input_devices;

    sds_vector_t *displays;
    sds_vector_t *inputs;
    sds_window_t *windows;

    uint32_t count_displays;
    uint32_t count_inputs;
    uint32_t count_windows;
} sds_connection_t;

SDS_PUBLIC sds_connection_t *sds_create_connection();
SDS_PUBLIC sds_window_t *sds_create_window(sds_size_t *, sds_layer_t);

#endif /* SDS_CORE_CONNECTION_H */
