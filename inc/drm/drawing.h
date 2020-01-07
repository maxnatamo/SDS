#ifndef SDS_DRM_DRAWING_H
#define SDS_DRM_DRAWING_H

#include <stdint.h>
#include "core/connection.h"
#include "drm/modesetting.h"
#include "drm/colors.h"
#include "utils/macros.h"
#include "utils/rect.h"

SDS_PUBLIC void sds_draw_clear(sds_display_device_t *, sds_color32_t);
SDS_PUBLIC void sds_draw_pixel(sds_display_device_t *, sds_pos_t, sds_color32_t);
SDS_PUBLIC void sds_draw_pixel_linear(sds_display_device_t *, int, sds_color32_t);
SDS_PUBLIC void sds_draw_line(sds_display_device_t *, sds_pos_t, sds_pos_t, sds_color32_t);
SDS_PUBLIC void sds_draw_rectangle(sds_display_device_t *, sds_rect_t, sds_color32_t);
SDS_PUBLIC void sds_draw_circle(sds_display_device_t *, sds_pos_t, uint16_t, sds_color32_t);

#endif /* SDS_DRM_DRAWING_H */
