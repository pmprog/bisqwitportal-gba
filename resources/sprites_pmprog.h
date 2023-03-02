
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define sprites_pmprog_len 4096
#define sprites_pmprog_width 64
#define sprites_pmprog_height 64
#define sprites_pmprog_tile_width 8
#define sprites_pmprog_tile_height 8
#define sprites_pmprog_tile_count 64
extern const uint8_t sprites_pmprog[];

#ifdef __cplusplus
}
#endif
