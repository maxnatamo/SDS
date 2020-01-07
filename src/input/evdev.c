#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <sys/signalfd.h>

#include "core/events.h"
#include "utils/macros.h"
#include "utils/log.h"
#include "utils/rect.h"
#include "utils/utils.h"
#include "input/core.h"
#include "input/evdev.h"

static sds_event_t *_event;

SDS_INTERNAL sds_input_device_t *sds_evdev_device_init(sds_input_request_t);
SDS_INTERNAL int sds_evdev_device_keyboard_events(sds_input_device_t *);
SDS_INTERNAL int sds_evdev_device_mouse_events(sds_input_device_t *);
SDS_INTERNAL void touchpad_get_size(sds_input_device_t *);
SDS_INTERNAL int sds_evdev_device_touchpad_events(sds_input_device_t *);
SDS_INTERNAL int sds_evdev_device_tablet_events(sds_input_device_t *);
SDS_INTERNAL int sds_evdev_device_joystick_events(sds_input_device_t *);
SDS_INTERNAL int sds_evdev_device_other_events(sds_input_device_t *);
SDS_INTERNAL void *sds_evdev_device_event_loop(void *);

SDS_INTERNAL sds_input_device_t *sds_evdev_device_init(sds_input_request_t request) {
    sds_input_device_t *device;
    device = malloc(sizeof(sds_input_device_t));

    if(!device) {
        sds_log_error("[EVDEV] Could not allocate input device.");
        return NULL;
    }

    device->fd = open(request.path, O_RDONLY | O_NONBLOCK);
    if(device->fd < 0) {
        sds_log_error("[EVDEV] Could not open input device '%s': %m", request.path);
        free(device);
        return NULL;
    }

    if(libevdev_new_from_fd(device->fd, &device->evdev_device) < 0) {
        sds_log_error("[EVDEV] Failed to initialize device '%s': %m", request.path);
        close(device->fd);
        free(device);

        return NULL;
    }

    _event = calloc(1, sizeof(sds_event_t));
    if(!_event) {
        sds_log_error("[EVDEV] Failed to allocate event structure: %m");
        sds_evdev_device_free(device);

        return NULL;
    }
    
    device->path        = request.path;
    device->type        = request.type;
    device->initialized = 1;

    if(device->type == SDS_TOUCHPAD) {
        touchpad_get_size(device);
    }

    sds_log_debug("[EVDEV] Initialized '%s'.", request.path);
    return device;
}

SDS_PUBLIC void sds_evdev_device_free(sds_input_device_t *device) {
    sds_log_debug("[EVDEV] Freeing device.");

    pthread_kill(device->event_thread, SIGSTOP);
    pthread_cancel(device->event_thread);

    libevdev_free(device->evdev_device);
    close(device->fd);
    free(device);
}

SDS_INTERNAL int sds_evdev_device_keyboard_events(sds_input_device_t *device) {
    switch(device->event.type) {
        case EV_KEY:
            _event->event_type = (device->event.value == 0) ? EVENT_KEY_RELEASE : EVENT_KEY_PRESS;
            _event->key_press.keycode = device->event.code;
            _event->key_release.keycode = device->event.code;
            return 1;
    
        default:
            break;
    }
    return 0;
}

SDS_INTERNAL int sds_evdev_device_mouse_events(sds_input_device_t *device) {
    if(device->event.code == BTN_LEFT     ||
       device->event.code == BTN_RIGHT    ||
       device->event.code == BTN_MIDDLE   ||
       device->event.code == BTN_SIDE     ||
       device->event.code == BTN_EXTRA    ||
       device->event.code == BTN_FORWARD  ||
       device->event.code == BTN_BACK     ||
       device->event.code == BTN_TASK) {
        
        _event->event_type = (device->event.value == 0) ? EVENT_BUTTON_RELEASE : EVENT_BUTTON_PRESS;
        _event->button_press.button   = device->event.code;
        _event->button_release.button = device->event.code;

        return 1;
    }

    switch(device->event.code) {
        default:
            return 0;
    }
}

SDS_INTERNAL void touchpad_get_size(sds_input_device_t *device) {
    unsigned int i;
    struct input_absinfo *abs;

    for(i = 0; i < ABS_MAX; i++) {
        if(!libevdev_has_event_code(device->evdev_device, EV_ABS, i)) {
            continue;
        }

        abs = (struct input_absinfo *) libevdev_get_abs_info(device->evdev_device, i);
        switch(i) {
            case ABS_MT_POSITION_X:
                device->touchpad.abs_x.min = abs->minimum;
                device->touchpad.abs_x.max = abs->maximum;
                break;

            case ABS_MT_POSITION_Y:
                device->touchpad.abs_y.min = abs->minimum;
                device->touchpad.abs_y.max = abs->maximum;
                break;

            case ABS_PRESSURE:
                device->touchpad.abs_pressure.min = abs->minimum;
                device->touchpad.abs_pressure.max = abs->maximum;
                break;

            default:
                break;
        }
    }
}

SDS_INTERNAL int sds_evdev_device_touchpad_events(sds_input_device_t *device) {
    static int x = -1, y = -1;
    static int px, py;

    if(device->event.code == BTN_LEFT || device->event.code == BTN_RIGHT) {
        _event->event_type = (device->event.value == 0) ? EVENT_BUTTON_RELEASE : EVENT_BUTTON_PRESS;
        _event->button_press.device = device;
        _event->button_press.button = device->event.code;

        return 1;
    }

    if(device->event.code == ABS_MT_POSITION_X || device->event.code == ABS_MT_POSITION_Y) {
        if(device->event.code == ABS_MT_POSITION_X) {
            px = x;
            x = device->event.value;
        } else {
            px = x;
            y = device->event.value;
        }

        // Prevents pushing event, when
        // only one coordinate is updated.
        if(x != -1 && y != -1) {
            _event->event_type = EVENT_POINTER_MOTION;
            _event->pointer_motion.device = device;
            _event->pointer_motion.x = x;
            _event->pointer_motion.y = y;
            _event->pointer_motion.px = px;
            _event->pointer_motion.py = py;

            x = -1;
            y = -1;

            return 1;
        }
        return 0;
    }

    switch(device->event.code) {
        case BTN_TOUCH:
            return 0;
        default:
            return 0;
    }

    return 0;
}

SDS_INTERNAL int sds_evdev_device_tablet_events(sds_input_device_t *device) {
    switch(device->event.code) {
        default:
            return 0;
    }
}

SDS_INTERNAL int sds_evdev_device_joystick_events(sds_input_device_t *device) {
    switch(device->event.code) {
        default:
            return 0;
    }
}

SDS_INTERNAL int sds_evdev_device_other_events(sds_input_device_t *device) {
    switch(device->event.code) {
        default:
            return 0;
    }
}

SDS_INTERNAL void *sds_evdev_device_event_loop(void *_device) {
    sds_input_device_t *device = (sds_input_device_t *)_device;
    struct pollfd *fds = sds_utils_nonblock_fd(device->fd);
    
    int i;
    int event_waiting = 0;

    while(poll(fds, 2, -1)) {
        if(fds[1].revents) {
            break;
        }

        do {
            i = libevdev_next_event(device->evdev_device, LIBEVDEV_READ_FLAG_NORMAL, &device->event);
            if(i != LIBEVDEV_READ_STATUS_SUCCESS) {
                continue;
            }

            // Syncronization events are somewhat
            // irrelevant to us, currently.
            if(device->event.type == EV_SYN) {
                continue;
            }

            switch(device->type) {
                case SDS_KEYBOARD:   event_waiting = sds_evdev_device_keyboard_events  (device); break;
                case SDS_MOUSE:      event_waiting = sds_evdev_device_mouse_events     (device); break;
                case SDS_TOUCHPAD:   event_waiting = sds_evdev_device_touchpad_events  (device); break;
                case SDS_TABLET:     event_waiting = sds_evdev_device_tablet_events    (device); break;
                case SDS_JOYSTICK:   event_waiting = sds_evdev_device_joystick_events  (device); break;
                case SDS_OTHER:      event_waiting = sds_evdev_device_other_events     (device); break;
                default: break;
            }

            if(event_waiting) {
                // Event is waiting to be pushed to queue.
                sds_push_event(_event);
            }
        } while(i == -EAGAIN);
    }
    return NULL;
}

SDS_PUBLIC sds_input_device_t **sds_evdev_init(sds_input_request_t *devices, int count_devices) {
    sds_input_device_t **input_devices = calloc(count_devices, sizeof(sds_input_device_t));

    for(int i = 0; i < count_devices; i++) {
        sds_input_device_t *device = sds_evdev_device_init(devices[i]);
        if(!device) {
            return NULL;
        }
        input_devices[i] = device;

        if(pthread_create(&device->event_thread, NULL, sds_evdev_device_event_loop, (void *)device) != 0) {
            sds_log_error("[EVDEV] Failed to create event thread: %m");
        }
    }
    return input_devices;
}
