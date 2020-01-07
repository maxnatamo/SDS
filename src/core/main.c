#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include "core/events.h"
#include "drm/modesetting.h"
#include "drm/colors.h"
#include "core/connection.h"
#include "drm/drawing.h"
#include "utils/queue.h"
#include "utils/vector.h"
#include "utils/rect.h"
#include "utils/log.h"
#include "utils/utils.h"
#include "input/core.h"
#include "input/evdev.h"

void swapbuffers(sds_display_device_t *device) {
    for(int i = 0; i < device->size / 4; i++) {
        device->frontbuffer[i] = device->backbuffer[i];
    }
}

int main(int argc, char *argv[]) {
    const char *displays[] = {
        "/dev/dri/card0"
    };
    sds_input_request_t inputs[] = {
        { .path = "/dev/input/event12", .type = SDS_TOUCHPAD }
    };

    sds_connection_t *connection = sds_create_connection();
    connection->input_devices    = sds_input_init(inputs, LEN(inputs), SDS_INPUT_DRIVER_EVDEV);
    connection->display_devices  = sds_drm_initialize(displays, LEN(displays));
    connection->count_inputs     = LEN(inputs);
    connection->count_displays   = LEN(displays);
    
    sds_events_init();

    sds_drm_modesetting_initialize(connection->display_devices, connection->count_displays);

    for(int i = 0; i < connection->count_displays; i++) {
        sds_display_device_t *display = connection->display_devices[i];
        sds_log_debug("[DRM] Resolution: %dx%d, %d-bit (%d bytes per color)", display->width, display->height, display->bpp, display->bpp / sizeof(void *));
        sds_log_debug("[DRM] Screen size: %u bytes", display->size);
    }
    sds_log_debug("[CORE] Finished initializing displays.");

    sds_display_device_t *device = connection->display_devices[0];

    sds_color32_t bg = { .full = 0xffffffff };
    sds_color32_t fg = { .full = 0x00000000 };

    sds_draw_clear(device, bg);
    swapbuffers(device);

    int maxX, maxY, x, y;
    sds_event_t *event = calloc(1, sizeof(sds_event_t));

    while(sds_next_event(event) == EXIT_SUCCESS) {
        if(event->event_type == EVENT_POINTER_MOTION) {
            maxX = event->pointer_motion.device->touchpad.abs_x.max;
            maxY = event->pointer_motion.device->touchpad.abs_y.max;
            x = event->pointer_motion.x * 100.0f / maxX * device->width / 100;
            y = event->pointer_motion.y * 100.0f / maxY * device->height / 100;
            
            sds_draw_clear(device, bg);
            sds_draw_line(device, (sds_pos_t){.x = 0, .y = 0}, (sds_pos_t){.x = x, .y = y}, fg);
            sds_draw_line(device, (sds_pos_t){.x = device->width, .y = 0}, (sds_pos_t){.x = x, .y = y}, fg);
            sds_draw_line(device, (sds_pos_t){.x = device->width, .y = device->height}, (sds_pos_t){.x = x, .y = y}, fg);
            sds_draw_line(device, (sds_pos_t){.x = 0, .y = device->height}, (sds_pos_t){.x = x, .y = y}, fg);

#if SDS_DOUBLE_BUFFERING
            swapbuffers(device);
#endif
        }
    }

    sds_drm_modesetting_cleanup(device);
    return 0;
}
