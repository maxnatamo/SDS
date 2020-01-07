#ifndef SDS_INPUT_EVDEV_H
#define SDS_INPUT_EVDEV_H

#include "core/connection.h"
#include "utils/macros.h"
#include "input/core.h"
#include "input/libevdev.h"

SDS_PUBLIC sds_input_device_t **sds_evdev_init(sds_input_request_t *, int);
SDS_PUBLIC void sds_evdev_device_free(sds_input_device_t *);

#endif /* SDS_INPUT_EVDEV_H */
