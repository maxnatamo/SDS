#include <math.h>

#include "core/connection.h"
#include "drm/modesetting.h"
#include "drm/drawing.h"
#include "drm/colors.h"
#include "utils/utils.h"
#include "utils/vector.h"
#include "utils/macros.h"
#include "utils/log.h"

SDS_PUBLIC void sds_draw_clear(sds_display_device_t *device, sds_color32_t color) {
    for(unsigned int i = 0; i < device->size / 4; i++) {
        sds_draw_pixel_linear(device, i, color);
    }
}

SDS_PUBLIC void sds_draw_pixel(sds_display_device_t *device, sds_pos_t pos, sds_color32_t color) {
    if(pos.x > device->width || pos.y > device->height) {
        return;
    }

    int location = CLAMP(pos.y, 0, device->height) * device->width + CLAMP(pos.x, 0, device->width);
    sds_draw_pixel_linear(device, location, color);
}

SDS_PUBLIC void sds_draw_pixel_linear(struct sds_display_device_t *device, int pos, sds_color32_t color) {
    if(pos > device->size / 4) {
        return;
    }
    
#if SDS_DOUBLE_BUFFERING
    device->backbuffer[pos] = color.full;
#else
    device->frontbuffer[pos] = color.full;
#endif
}

SDS_PUBLIC void sds_draw_line(sds_display_device_t *device, sds_pos_t p1, sds_pos_t p2, sds_color32_t color) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int dxabs = abs(dx);
    int dyabs = abs(dy);
    int sdx = SGN(dx);
    int sdy = SGN(dy);
    int x = dyabs >> 1;
    int y = dxabs >> 1;
    int px = p1.x;
    int py = p1.y;
    int i;
    sds_pos_t rect;

    if(dxabs >= dyabs) {
        for(i = 0; i < dxabs; i++) {
            y += dyabs;
            if(y >= dxabs) {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            rect.x = px;
            rect.y = py;
            sds_draw_pixel(device, rect, color);
        }
    } else {
        for(i = 0; i < dyabs; i++) {
            x += dxabs;
            if(x >= dyabs) {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            rect.x = px;
            rect.y = py;
            sds_draw_pixel(device, rect, color);
        }
    }
}

SDS_PUBLIC void sds_draw_rectangle(sds_display_device_t *device, sds_rect_t rect, sds_color32_t color) {
    // Top border
    sds_pos_t line1[2] = {
        {rect.x,              rect.y},
        {rect.x + rect.width, rect.y}
    };

    // Right border
    sds_pos_t line2[2] = {
        {rect.x + rect.width, rect.y},
        {rect.x + rect.width, rect.y + rect.height}
    };

    // Bottom border
    sds_pos_t line3[2] = {
        {rect.x + rect.width, rect.y + rect.height},
        {rect.x,              rect.y + rect.height}
    };

    // Left border
    sds_pos_t line4[2] = {
        {rect.x,              rect.y + rect.height},
        {rect.x,              rect.y}
    };

    sds_draw_line(device, line1[0], line1[1], color);
    sds_draw_line(device, line2[0], line2[1], color);
    sds_draw_line(device, line3[0], line3[1], color);
    sds_draw_line(device, line4[0], line4[1], color);
}

SDS_PUBLIC void sds_draw_circle(sds_display_device_t *device, sds_pos_t center, uint16_t radius, sds_color32_t color) {

}
