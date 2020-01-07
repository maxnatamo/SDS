#ifndef SDS_CORE_EVENTS_H
#define SDS_CORE_EVENTS_H

#include <stdint.h>
#include "core/connection.h"
#include "utils/macros.h"
#include "utils/rect.h"
#include "input/core.h"

/*
 *  SDS EVENTS
 *
 *  Events are used to tell the window manager,
 *  when a specific action happens.
 *
 *  The values below are used to indicate the
 *  type of event. These events are only sent to 
 *  windows and window managers, who uses the
 *  corresponding event mask (commented on the right)
 */
#define EVENT_KEY_PRESS       (uint16_t)(1 << 1)   // KEYBOARD_KEY_MASK
#define EVENT_KEY_RELEASE     (uint16_t)(1 << 2)  
#define EVENT_BUTTON_PRESS    (uint16_t)(1 << 3)   // MOUSE_BUTTON_MASK
#define EVENT_BUTTON_RELEASE  (uint16_t)(1 << 4)  
#define EVENT_POINTER_MOTION  (uint16_t)(1 << 5)   // POINTER_MOTION MASK
#define EVENT_FOCUS_IN        (uint16_t)(1 << 6)   // WINDOW_FOCUS_MASK
#define EVENT_FOCUS_OUT       (uint16_t)(1 << 7)  
#define EVENT_WINDOW_MAP      (uint16_t)(1 << 8)   // WINDOW_MAP_MASK
#define EVENT_WINDOW_UNMAP    (uint16_t)(1 << 9)  
#define EVENT_WINDOW_CREATE   (uint16_t)(1 << 10)  // WINDOW_CREATE_MASK
#define EVENT_WINDOW_DESTROY  (uint16_t)(1 << 11)  // WINDOW_DESTROY_MASK
#define EVENT_WINDOW_MOVE     (uint16_t)(1 << 12)  // WINDOW_MOVE_MASK
#define EVENT_WINDOW_RESIZE   (uint16_t)(1 << 13)  // WINDOW_RESIZE_MASK

/*
 *  SDS EVENT MASKS
 *
 *  Event masks are used to mask unwanted events from
 *  windows and window managers. If the event mask is not
 *  used, the corresponding events will not be sent
 *  to the window or window manager.
 */
#define KEYBOARD_KEY_MASK      (uint16_t)(1 << 1)
#define MOUSE_BUTTON_MASK      (uint16_t)(1 << 2)
#define WINDOW_FOCUS_MASK      (uint16_t)(1 << 3)
#define WINDOW_MAP_MASK        (uint16_t)(1 << 4)
#define WINDOW_CREATE_MASK     (uint16_t)(1 << 5)
#define WINDOW_DESTROY_MASK    (uint16_t)(1 << 6)
#define WINDOW_MOVE_MASK       (uint16_t)(1 << 7)
#define WINDOW_RESIZE_MASK     (uint16_t)(1 << 8)

/*
 *  EVENT STRUCT DEFINITIONS
 */

/*
 * Indicates that a key has been either pressed or released.
 * Whether the key has been pressed or released, is shown in
 * the event type.
 *
 * EVENT_KEY_PRESS    = key has been pressed.
 * EVENT_KEY_RELEASED = key has been released.
 */
typedef struct {
    sds_input_device_t *device;  /* Pointer to device, that triggered the event. */
    unsigned int keycode;        /* Which key triggered the event. See <linux/input-event-codes.h> */
    unsigned int mask;           /* If any, holds the keycode masks in use, when the event was fired. */
} sds_event_key_t;

/*
 * Indicates that a button has been either pressed or released.
 * Whether the button has been pressed or released, is shown in
 * the event type.
 *
 * EVENT_BUTTON_PRESS    = key has been pressed.
 * EVENT_BUTTON_RELEASED = key has been released.
 */
typedef struct {
    sds_input_device_t *device;  /* Pointer to device, that triggered the event. */
    unsigned int button;         /* Which button triggered the event. */
    unsigned int mask;           /* If any, holds the keycode masks in use, when the event was fired. */
} sds_event_button_t;

/* 
 * Indicates that the pointer/cursor has moved.
 *
 * NOTE: This event can be fired rather often, as it uses
 *       the raw input from the input device.
 */
typedef struct {
    sds_input_device_t *device;  /* Pointer to device, that triggered the event. */
    unsigned int x, y;           /* New XY-coordinates of the pointer. */
    unsigned int px, py;         /* Previous XY-coordinates of the pointer. */
    int dx, dy;
} sds_event_pointer_motion_t;

/*
 * Indicates that the window focus has been changed.
 * Whether the window has been focused or unfocused,
 * is shown in the event type.
 *
 * EVENT_FOCUS_IN  = window has been focused.
 * EVENT_FOCUS_OUT = window has been un-focused.
 */
typedef struct {
    sds_window_t *focus;         /* Pointer to new window focus. */
    sds_window_t *prevous_focus; /* Pointer to previus window focus. */
} sds_event_focus_t;

/*
 * Indicates that a window has been either mapped,
 * or unmapped to the screen.
 * Whether the window has been mapped or unmapped, is
 * shown in the event type.
 *
 * EVENT_WINDOW_MAP   = window has been mapped.
 * EVENT_WINDOW_UNMAP = window has been un-mapped.
 *
 * NOTE: This event is fired AFTER the action has taken place.
 */
typedef struct {
    sds_window_t *window;        /* Pointer to the newly-mapped window. */
    sds_rect_t rect;             /* The size and position of the window. */
} sds_event_window_mapping_t;

/*
 * Indicates that a window has been created.
 *
 * NOTE: This event is fired AFTER the window
 *       has been created.
 */
typedef struct {
    sds_window_t *window;        /* Pointer to the newly-created window. */
} sds_event_window_create_t;

/*
 * Indicates that a window has been destroyed.
 *
 * NOTE: This event is fired AFTER the window
 *       has been destroyed.
 */
typedef struct {
    sds_window_t *window;        /* Pointer to the just-destroyed window. */
} sds_event_window_destroy_t;

/*
 * Indicates that a window has been moved.
 *
 * NOTE: This event can be fired rather often,
 *       as usually relies on the pointer
 *       motion event.
 */
typedef struct {
    sds_window_t *window;        /* Pointer to the moved window. */
    sds_pos_t new_pos;           /* New position of the window */
    sds_pos_t old_pos;           /* Old position of the window */
} sds_event_window_move_t;

/*
 * Indicates that a window has been moved.
 *
 * NOTE: This event is sometimes fired rather
 *       often, as it might rely on the pointer
 *       motion event.
 */
typedef struct {
    sds_window_t *window;        /* Pointer to the resized window. */
    sds_size_t new_size;         /* New size of the window. */
    sds_size_t old_size;         /* Old size of the window. */
} sds_event_window_resize_t;

typedef sds_event_key_t             sds_event_key_press_t;
typedef sds_event_key_t             sds_event_key_release_t;
typedef sds_event_button_t          sds_event_button_press_t;
typedef sds_event_button_t          sds_event_button_release_t;
typedef sds_event_focus_t           sds_event_focus_in_t;
typedef sds_event_focus_t           sds_event_focus_out_t;
typedef sds_event_window_mapping_t  sds_event_window_map_t;
typedef sds_event_window_mapping_t  sds_event_window_unmap_t;

typedef struct {
    uint16_t event_type;
    uint16_t mask;
    union {
        sds_event_key_press_t       key_press;
        sds_event_key_release_t     key_release;
        sds_event_button_press_t    button_press;
        sds_event_button_release_t  button_release;
        sds_event_pointer_motion_t  pointer_motion;
        sds_event_focus_in_t        focus_in;
        sds_event_focus_out_t       focus_out;
        sds_event_window_map_t      window_map;
        sds_event_window_unmap_t    window_unmap;
        sds_event_window_create_t   window_create;
        sds_event_window_destroy_t  window_destroy;
        sds_event_window_move_t     window_move;
        sds_event_window_resize_t   window_resize;
    };
} sds_event_t;

/*
 * Initializes the event pipe and structure.
 *
 * RETURN VALUE
 * 0 (EXIT_SUCCESS) on success, negative
 * value otherwise, with errno indicating
 * the error.
 */
SDS_PUBLIC int sds_events_init();

/*
 * Pushes the specified event to the event pipe.
 *
 * Event type and event mask MUST be set,
 * otherwise the call will fail.
 *
 * RETURN VALUE
 * 0 (EXIT_SUCCESS) on success, negative
 * value otherwise, with errno indicating
 * the error.
 */
SDS_PUBLIC int sds_push_event(sds_event_t *event);

/*
 * Polls the event pipe for incoming events,
 * conforming to the event mask. Note, that
 * this is blocking, if the event pipe
 * is empty.
 *
 * This function copies the next event in the 
 * event pipe, to the passed event structure,
 * and returns.
 *
 * RETURN VALUE
 * 0 (EXIT_SUCCESS) on success, negative
 * value otherwise, with errno indicating
 * the error.
 */
SDS_PUBLIC int sds_next_event(sds_event_t *event);

#endif /* SDS_CORE_EVENTS_H */
