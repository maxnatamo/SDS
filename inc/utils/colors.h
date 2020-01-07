#include "config.h"
#include "utils/macros.h"

#ifndef SDS_UTILS_COLOR_H
#define SDS_UTILS_COLOR_H

#define SDS_BLACK           "0;30"
#define SDS_RED             "0;31"
#define SDS_GREEN           "0;32"
#define SDS_BROWN           "0;33"
#define SDS_BLUE            "0;34"
#define SDS_PURPLE          "0;35"
#define SDS_CYAN            "0;36"
#define SDS_YELLOW          "1;33"
#define SDS_DARK_GRAY       "1;30"
#define SDS_LIGHT_GRAY      "0;37"
#define SDS_LIGHT_RED       "1;31"
#define SDS_LIGHT_GREEN     "1;32"
#define SDS_LIGHT_BLUE      "1;34"
#define SDS_LIGHT_PURPLE    "1;35"
#define SDS_LIGHT_CYAN      "1;36"
#define SDS_WHITE           "1;37"
#define SDS_RESET           "0"

SDS_PUBLIC void sds_set_color(char *);

#endif /* SDS_UTILS_COLOR_H */
