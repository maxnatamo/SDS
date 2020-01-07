#include "core/connection.h"
#include "input/core.h"
#include "input/evdev.h"
#include "utils/macros.h"
#include "utils/log.h"

SDS_PUBLIC sds_input_device_t **sds_input_init(sds_input_request_t *devices, int count_devices, int driver) {
    sds_log_debug("Init devices.");
    switch(driver) {
        case SDS_INPUT_DRIVER_EVDEV:
            return sds_evdev_init(devices, count_devices);

        default:
            break;
    }
    return NULL;
}
