#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>

#include <pthread.h>

#include "core/events.h"
#include "utils/utils.h"
#include "utils/macros.h"
#include "utils/vector.h"
#include "utils/log.h"

static int *fds;
static struct pollfd *event_fds;

SDS_PUBLIC int sds_events_init() {
    fds = calloc(2, sizeof(int));
    if(pipe(fds) != 0) {
        sds_log_error("[EVENT] Failed to open event pipe: %m");
        return -errno;
    }
    event_fds = sds_utils_nonblock_fd(fds[0]);
    return EXIT_SUCCESS;
}

SDS_PUBLIC int sds_push_event(sds_event_t *event) {
    if(event->event_type == 0) {
        sds_log_error("[EVENT] Event type is invalid.");
        errno = EINVAL;
        return -errno;
    }

    if(write(fds[1], event, sizeof(sds_event_t)) == -1) {
        sds_log_error("[EVENT] Failed to write to event file descriptors: %m");
        return -errno;
    }
    return EXIT_SUCCESS;
}

SDS_PUBLIC int sds_next_event(sds_event_t *event) {
    if(event_fds == NULL) {
        sds_log_error("[EVENT] Event file descriptors not initialized.");

        errno = ENOENT;
        return -errno;
    }

    while(poll(event_fds, 2, -1) != -1) {
        if(event_fds[1].revents) {
            break;
        }

        if(read(fds[0], event, sizeof(sds_event_t)) == -1) {
            sds_log_error("[EVENT] Couldn't read event pipe: %m");
            return -errno;
        }
        return EXIT_SUCCESS;
    }
    sds_log_warning("[EVENT] Error occured while polling events: %m");
    return -errno;
}
