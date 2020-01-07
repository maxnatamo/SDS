#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <sys/stat.h>
#define __USE_MISC // MAP_ANONYMOUS
#include <sys/mman.h>
#undef __USE_MISC

#include "core/connection.h"
#include "drm/modesetting.h"
#include "utils/macros.h"
#include "utils/log.h"

typedef struct {
    struct drm_mode_create_dumb creq;
    struct drm_mode_map_dumb    mreq;
    struct drm_mode_fb_cmd      cmdd;
} dumb_request_result;

SDS_INTERNAL void drm_handle_connector(sds_display_device_t *, uint64_t);
SDS_INTERNAL dumb_request_result drm_handle_dumb_requests(sds_display_device_t *, struct drm_mode_modeinfo);
SDS_INTERNAL int drm_map_buffers(sds_display_device_t *, size_t, off_t);

SDS_PUBLIC sds_display_device_t **sds_drm_initialize(const char **display_devices, uint32_t count_displays) {
    sds_display_device_t **devices = (sds_display_device_t **) malloc(sizeof(sds_display_device_t *));
    if(!devices) {
        sds_log_error("[DRM] Failed to initialize display devices.");
        return NULL;
    }

    for(uint32_t i = 0; i < count_displays; i++) {
        devices[i] = malloc(sizeof(sds_display_device_t));

        if((devices[i]->fd = open(display_devices[i], O_RDWR | O_CLOEXEC)) < 0) {
            sds_log_error("[DRM] Failed to open '%s': %m", display_devices[i]);
            continue;
        }
        sds_log_debug("[DRM] Initialized display device '%s'", display_devices[i]);
    }
    return devices;
}

SDS_PUBLIC void sds_drm_modesetting_initialize(sds_display_device_t **devices, int count_displays) {
    for(int i = 0; i < count_displays; i++) {
        struct drm_mode_card_res res = {0};
        uint64_t *res_fb_buffer;
        uint64_t *res_crtc_buffer;
        uint64_t *res_conn_buffer;
        uint64_t *res_enc_buffer;

        // Set the Master of the DRI device.
        ioctl(devices[i]->fd, DRM_IOCTL_SET_MASTER, 0);

        // Get resource count.
        ioctl(devices[i]->fd, DRM_IOCTL_MODE_GETRESOURCES, &res);

        // Allocate required memory.
        res_fb_buffer   = calloc(res.count_fbs, sizeof(uint64_t));
        res_crtc_buffer = calloc(res.count_crtcs, sizeof(uint64_t));
        res_conn_buffer = calloc(res.count_connectors, sizeof(uint64_t));
        res_enc_buffer  = calloc(res.count_encoders, sizeof(uint64_t));
    
        // Set resource pointers
        res.fb_id_ptr        = (uint64_t)res_fb_buffer;
        res.crtc_id_ptr      = (uint64_t)res_crtc_buffer;
        res.connector_id_ptr = (uint64_t)res_conn_buffer;
        res.encoder_id_ptr   = (uint64_t)res_enc_buffer;
        ioctl(devices[i]->fd, DRM_IOCTL_MODE_GETRESOURCES, &res);

        for(int j = 0; j < res.count_connectors; j++) {
            drm_handle_connector(devices[i], res_conn_buffer[j]);
            break;
        }
        ioctl(devices[i]->fd, DRM_IOCTL_DROP_MASTER, 0);
    }
}

SDS_INTERNAL void drm_handle_connector(sds_display_device_t *device, uint64_t connector_buffer) {
    struct drm_mode_get_connector conn = {0};
    struct drm_mode_modeinfo *conn_mode_buffer;
    uint64_t *conn_prop_buffer;
    uint64_t *conn_values_buffer;
    uint64_t *conn_encoder_buffer;
    
    conn.connector_id = connector_buffer;
    
    // Get resource count.
    ioctl(device->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    // Allocate required memory.
    conn_mode_buffer    = calloc(conn.count_modes, sizeof(struct drm_mode_modeinfo));
    conn_prop_buffer    = calloc(conn.count_props, sizeof(uint64_t));
    conn_values_buffer  = calloc(conn.count_props, sizeof(uint64_t));
    conn_encoder_buffer = calloc(conn.count_encoders, sizeof(uint64_t));
    
    // Set resource pointers
    conn.modes_ptr       = (uint64_t) conn_mode_buffer;
    conn.props_ptr       = (uint64_t) conn_prop_buffer;
    conn.prop_values_ptr = (uint64_t) conn_values_buffer;
    conn.encoders_ptr    = (uint64_t) conn_encoder_buffer;
    ioctl(device->fd, DRM_IOCTL_MODE_GETCONNECTOR, &conn);
    
    if(conn.count_encoders <= 0 || conn.count_modes <= 0 || !conn.encoder_id || !conn.connection) {
        sds_log_debug("[DRM] Output not connected.");
        return;
    }

    // Handle dumb requests.
    dumb_request_result request = drm_handle_dumb_requests(device, conn_mode_buffer[0]);
    
    // Map framebuffers
    if(drm_map_buffers(device, request.creq.size, request.mreq.offset) != EXIT_SUCCESS) {
        return;
    }

    // Get/set CRTC.
    struct drm_mode_get_encoder enc  = {0};
    struct drm_mode_crtc        crtc = {0};
    
    enc.encoder_id = conn.encoder_id;
    ioctl(device->fd, DRM_IOCTL_MODE_GETENCODER, &enc);
    
    crtc.crtc_id = enc.crtc_id;
    ioctl(device->fd, DRM_IOCTL_MODE_GETCRTC, &crtc);
    
    crtc.fb_id              = request.cmdd.fb_id;
    crtc.set_connectors_ptr = (uint64_t)&connector_buffer;
    crtc.mode               = conn_mode_buffer[0];
    crtc.count_connectors   = 1;
    crtc.mode_valid         = 1;
    ioctl(device->fd, DRM_IOCTL_MODE_SETCRTC, &crtc);
    
    sds_log_debug("[DRM] Created DRI device: %d", device->fd);
}

SDS_INTERNAL dumb_request_result drm_handle_dumb_requests(sds_display_device_t *device, struct drm_mode_modeinfo mode) {
    dumb_request_result result = {0};

    result.creq.width  = mode.hdisplay;
    result.creq.height = mode.vdisplay;
    result.creq.bpp    = 32;
    result.creq.flags  = 0;
    result.creq.pitch  = 0;
    result.creq.size   = 0;
    result.creq.handle = 0;
    ioctl(device->fd, DRM_IOCTL_MODE_CREATE_DUMB, &result.creq);
    
    // Framebuffer request.
    result.cmdd.width   = result.creq.width;
    result.cmdd.height  = result.creq.height;
    result.cmdd.pitch   = result.creq.pitch;
    result.cmdd.bpp     = result.creq.bpp;
    result.cmdd.handle  = result.creq.handle;
    result.cmdd.depth   = 24;
    ioctl(device->fd, DRM_IOCTL_MODE_ADDFB, &result.cmdd);
    
    device->width  = result.creq.width;
    device->height = result.creq.height;
    device->stride = result.creq.pitch;
    device->bpp    = result.creq.bpp;
    device->size   = result.creq.size;
    
    // Map buffer request.
    result.mreq.handle    = result.creq.handle;
    ioctl(device->fd, DRM_IOCTL_MODE_MAP_DUMB, &result.mreq);

    return result;
}

SDS_INTERNAL int drm_map_buffers(sds_display_device_t *device, size_t size, off_t offset) {
    device->frontbuffer = (uint32_t *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, device->fd, offset);
    if(device->frontbuffer == MAP_FAILED) {
        sds_log_fatal("[DRM] Could not map framebuffer: %m");
        return -errno;
    }

#if SDS_DOUBLE_BUFFERING
    device->backbuffer  = (uint32_t *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, device->fd, 0);
    if(device->backbuffer == MAP_FAILED) {
        sds_log_fatal("[DRM] Could not map backbuffer: %m");
        return -errno;
    }
#endif
    return EXIT_SUCCESS;
}

SDS_PUBLIC int sds_drm_modesetting_cleanup(sds_display_device_t *device) {
    if(munmap((void *)device->frontbuffer, device->size) == -1) {
        sds_log_error("[DRM] Failed to unmap frontbuffer: %m");
        return -errno;
    }

#if SDS_DOUBLE_BUFFERING
    if(munmap((void *)device->backbuffer, device->size) == -1) {
        sds_log_error("[DRM] Failed to unmap backbuffer: %m");
        return -errno;
    }
#endif
    
    sds_log_debug("[CORE] Finished cleaning up");
    return EXIT_SUCCESS;
}
