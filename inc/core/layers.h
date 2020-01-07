#ifndef SDS_CORE_LAYERS_H
#define SDS_CORE_LAYERS_H

#include <stdint.h>

#define SDS_LAYER_WALLPAPER   (uint8_t)0x00000000
#define SDS_LAYER_DESKTOP     (uint8_t)0x00000001
#define SDS_LAYER_WINDOW      (uint8_t)0x00000002
#define SDS_LAYER_TOOLBAR     (uint8_t)0x00000003
#define SDS_LAYER_CURSOR      (uint8_t)0x00000004

typedef uint8_t sds_layer_t;

#endif /* SDS_CORE_LAYERS_H */
