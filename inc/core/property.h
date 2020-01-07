#ifndef SDS_CORE_PROPERTY_H
#define SDS_CORE_PROPERTY_H

#include <stdint.h>
#include "core/connection.h"
#include "utils/macros.h"

/*
 * SDS_PROP - WINDOW PROPERTIES
 *
 * All of the following properties are
 * added on window creation (sds_create_window).
 *
 * If any NOT NULLABLE properties are found,
 * the window will not be able to be mapped.
 */
#define SDS_PROP_WINDOW_NAME       (1 << 0) /* Window title [STRING, NOT NULLABLE] */
#define SDS_PROP_WINDOW_CLASS      (1 << 1) /* Window class - this is usually the same as SDS_PROP_WINDOW_NAME [STRING, NOT NULLABLE] */
#define SDS_PROP_WINDOW_PID        (1 << 2) /* PID of the window [INT, NOT NULLABLE] */
#define SDS_PROP_WINDOW_EXEC       (1 << 3) /* Program executable and potential arguments [STRING, NOT NULLABLE] */
#define SDS_PROP_WINDOW_DESKTOP    (1 << 4) /* Which desktop the is window residing in [INT, NULLABLE] */
#define SDS_PROP_WINDOW_ICON       (1 << 5) /* Path to program icon [STRING, NULLABLE] */

/*
 * SDS_ACTION - WINDOW ALLOWED ACTIONS
 *
 * The window manager does not need to honor these, but
 * they are used to indicate best window options.
 */ 
#define SDS_ACTION_MOVE            (1 << 0) /* The window is allowed to be moved */
#define SDS_ACTION_RESIZE          (1 << 1) /* The window is allowed to be reszized */
#define SDS_ACTION_CLOSE           (1 << 2) /* The window is allowed to be closed */
#define SDS_ACTION_MAXIMIZE        (1 << 3) /* The window is allowed to be maximized. Implies SDS_ACTION_MAXIMIZE_VERT and SDS_ACTION_MAXIMIZE_HORI */
#define SDS_ACTION_MINIMIZE        (1 << 4) /* The window is allowed to be minimized */
#define SDS_ACTION_FULLSCREEN      (1 << 5) /* The window is allowed to be fullscreen */
#define SDS_ACTION_MAXIMIZE_VERT   (1 << 6) /* The window is allowed to be maximized vertically. SDS_ACTION_MAXIMIZE implies this */
#define SDS_ACTION_MAXIMIZE_HORI   (1 << 7) /* The window is allowed to be maximized horizontally. SDS_ACTION_MAXIMIZE implies this */

/* Typedefs / structs */
typedef  struct {
    uint32_t key;
    intptr_t value;
} sds_property_t;

SDS_PUBLIC sds_property_t *sds_create_prop(uint32_t, intptr_t);
SDS_PUBLIC int sds_window_add_prop(sds_window_t *, sds_property_t *);

#endif /* SDS_CORE_PROPERTY_H */
