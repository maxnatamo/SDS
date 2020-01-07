#ifndef SDS_UTILS_LOG_H
#define SDS_UTILS_LOG_H

#include <stdint.h>
#include "config.h"
#include "utils/macros.h"

#define SDS_LOG_LEVEL_DEBUG     (uint8_t)0x0
#define SDS_LOG_LEVEL_WARNING   (uint8_t)0x1
#define SDS_LOG_LEVEL_ERROR     (uint8_t)0x2
#define SDS_LOG_LEVEL_FATAL     (uint8_t)0x3
#define SDS_LOG_LEVEL_NONE      (uint8_t)0xf

#define LOG_LEVEL               SDS_LOG_LEVEL_DEBUG

typedef uint8_t sds_log_level;

SDS_PUBLIC int sds_log_debug(const char *, ...);
SDS_PUBLIC int sds_log_warning(const char *, ...);
SDS_PUBLIC int sds_log_error(const char *, ...);
SDS_PUBLIC int sds_log_fatal(const char *, ...);

#endif /* SDS_UTILS_LOG_H */
