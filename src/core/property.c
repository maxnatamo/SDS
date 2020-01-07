#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "core/connection.h"
#include "core/property.h"
#include "utils/macros.h"
#include "utils/vector.h"
#include "utils/log.h"

SDS_PUBLIC sds_property_t *sds_create_prop(uint32_t key, intptr_t value) {
    sds_property_t *prop = malloc(sizeof(sds_property_t));
    if(!prop) {
        sds_log_error("[CORE] Window property could not be allocated.");
        return NULL;
    }

    prop->key   = key;
    prop->value = value;

    return prop;
}

SDS_PUBLIC int sds_window_add_prop(sds_window_t *window, sds_property_t *prop) {
    if(!window || !window->properties) {
        sds_log_error("[CORE] Invalid window object.");
        return -EINVAL;
    }

    sds_vector_push(window->properties, (intptr_t) &prop);
    
    return EXIT_SUCCESS;
}
