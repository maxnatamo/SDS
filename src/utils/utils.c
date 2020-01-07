#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/signalfd.h>

#include "utils/macros.h"
#include "utils/log.h"

SDS_PUBLIC struct pollfd *sds_utils_nonblock_fd(int in_fd) {
    struct pollfd *fds = calloc(2, sizeof(struct pollfd));
    sigset_t mask;

    fds[0].fd = in_fd;
    fds[0].events = POLLIN;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    fds[1].fd = signalfd(-1, &mask, SFD_NONBLOCK);
    fds[1].events = POLLIN;

    sigprocmask(SIG_BLOCK, &mask, NULL);

    return fds;
}
