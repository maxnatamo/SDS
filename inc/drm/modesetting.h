#ifndef SDS_DRM_MODESETTING_H
#define SDS_DRM_MODESETTING_H

#include <stdlib.h>
#include <stdint.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

#include "utils/macros.h"

typedef struct sds_display_device_t {
    int               fd;
    uint16_t          width;
    uint16_t          height;
    uint16_t          stride;
    uint32_t          size;
    uint8_t           bpp;
    uint32_t         *frontbuffer;
    uint32_t         *backbuffer;
} sds_display_device_t;

SDS_PUBLIC sds_display_device_t **sds_drm_initialize(const char **, uint32_t);
SDS_PUBLIC void sds_drm_modesetting_initialize(sds_display_device_t **, int);

SDS_PUBLIC sds_display_device_t *sds_drm_modesetting_open(const char *);
// SDS_PUBLIC int sds_drm_modesetting_initialize(sds_display_device_t *);
SDS_PUBLIC int sds_drm_modesetting_cleanup(sds_display_device_t *);

#endif /* SDS_DRM_MODESETTING_H */
