#ifndef SDS_UTILS_COLOR_H
#define SDS_UTILS_COLOR_H

#include <stdio.h>

#include "config.h"
#include "utils/colors.h"
#include "utils/macros.h"

SDS_PUBLIC void sds_set_color(char *seq) {
#if SDS_USE_COLORS
    printf("\033[%sm", seq);
#endif
}

#endif /* SDS_UTILS_COLOR_H */
