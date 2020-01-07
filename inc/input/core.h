#ifndef SDS_INPUT_CORE_H
#define SDS_INPUT_CORE_H

#include <stdint.h>
#include <pthread.h>

#include "core/connection.h"
#include "utils/rect.h"
#include "utils/utils.h"
#include "input/libevdev.h"

/*
 *  INPUT DRIVERS
 *
 *  List of currently supposed input drivers.
 */
#define SDS_INPUT_DRIVER_EVDEV    (uint16_t)(1 << 1)
#define SDS_INPUT_DRIVER_LIBINPUT (uint16_t)(1 << 2)

/*
 *  INPUT DEVICE TYPES
 *
 *  List of input device types.
 *  This is used to determine the
 *  event handler, in the initialization.
 */
#define SDS_KEYBOARD              (uint16_t)(1 << 1)
#define SDS_MOUSE                 (uint16_t)(1 << 2)
#define SDS_TOUCHPAD              (uint16_t)(1 << 3)
#define SDS_TABLET                (uint16_t)(1 << 4)
#define SDS_JOYSTICK              (uint16_t)(1 << 5)
#define SDS_OTHER                 (uint16_t)(1 << 6)
#define SDS_UNDEFINED             (uint16_t)(1 << 7)

/*
 *  KEYCODE MASKS
 *
 *  List of possible keycode masks.
 *  These are used to determine whether
 *  any special keys was pressed, during
 *  an event.
 *
 *  EXAMPLE
 *  KEY_PRESS_EVENT with keycode
 *  KEY_C and mask SDS_CONTROL_MASK
 *  indicates Control+C with pressed.
 */
#define SDS_NONE_MASK             (uint16_t)(1 << 0) /* No special keys was pressed */
#define SDS_CONTROL_MASK          (uint16_t)(1 << 1) /* A Control key was pressed (either left- or right- Control) */
#define SDS_SHIFT_MASK            (uint16_t)(1 << 2) /* A Shift key was pressed (either left- or right- Shift) */
#define SDS_LOCK_MASK             (uint16_t)(1 << 3) /* Scroll lock was activated. */
#define SDS_MOD_1_MASK            (uint16_t)(1 << 4) /* Mod 1 was pressed. */
#define SDS_MOD_2_MASK            (uint16_t)(1 << 5) /* Mod 2 was pressed. */
#define SDS_MOD_3_MASK            (uint16_t)(1 << 6) /* Mod 3 was pressed. */
#define SDS_MOD_4_MASK            (uint16_t)(1 << 7) /* Mod 4 was pressed. */
#define SDS_MOD_5_MASK            (uint16_t)(1 << 8) /* Mod 5 was pressed. */

typedef struct libevdev_device libevdev_device;
typedef struct libevdev_event libevdev_event;

/*
 *  INPUT DEVICE STRUCTURES
 *
 *  These structures hold type-dependant
 *  information about the device.
 *  See the 'type' variable of
 *  the sds_input_device_t struct.
 */

/* KEYBOARD SPECIFIC STRUCT */
typedef struct {
} sds_input_keyboard_t;

/* MOUSE SPECIFIC STRUCT */
typedef struct {
} sds_input_mouse_t;

/* TOUCHPAD SPECIFIC STRUCT */
typedef struct {
    sds_interval_t abs_x;           /* Possible interval for X-coordinates. */
    sds_interval_t abs_y;           /* Possible interval for Y-coordinates. */
    sds_interval_t abs_pressure;    /* Possible interval for pressure. */
} sds_input_touchpad_t;

/* TABLET SPECIFIC STRUCT */
typedef struct {
} sds_input_tablet_t;

/* JOYSTICK SPECIFIC STRUCT */
typedef struct {
} sds_input_joystick_t;

typedef struct {
    char *path;                     /* The supplied path to the device block. */
    int fd;                         /* The file descriptor of the device. */
    char initialized;               /* 1 if the device is initialized, 0 otherwise. */
    uint16_t type;                  /* The input device type. See INPUT DEVICE TYPES. */

    struct libevdev *evdev_device;  /* EVDEV struct for handling events. */
    struct input_event event;       /* EVDEV event. */
    pthread_t event_thread;         /* Input device pthread. */

    union {
        sds_input_keyboard_t  keyboard;
        sds_input_mouse_t     mouse;
        sds_input_touchpad_t  touchpad;
        sds_input_tablet_t    tablet;
        sds_input_joystick_t  joystick;
    };
} sds_input_device_t;

/* TODO: Add short comment. */
typedef struct {
    char *path;
    uint16_t type;
} sds_input_request_t;

/*
 * Initializes the input devices and structures.
 * Takes the current SDS connection, connection, as parameter.
 */
SDS_PUBLIC sds_input_device_t **sds_input_init(sds_input_request_t *devices, int count_devices, int driver);

#endif /* SDS_INPUT_CORE_H */
