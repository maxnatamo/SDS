#ifndef SDS_UTILS_UTILS_H
#define SDS_UTILS_UTILS_H

#include <poll.h>
#include <stdint.h>

#define MIN(a, b)        ((a < b) ? a : b)
#define MAX(a, b)        ((a > b) ? a : b)
#define CLAMP(v, i, a)   (MIN(MAX(v, i), a))
#define SGN(x)           ((x < 0) ? -1 : ((x > 0) ? 1 : 0))
#define LEN(x)           (sizeof(x)/sizeof(x[0]))

typedef struct {
    int32_t min;
    int32_t max;
} sds_interval_t;

struct pollfd *sds_utils_nonblock_fd(int);

#endif /* SDS_UTILS_UTILS_H */
