#pragma once

/* Generated with cbindgen:0.27.0 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define FRAME_TICKS 50

#define SIM_X_SIZE 512

#define SIM_Y_SIZE 256

void sim_init(void);

void sim_exit(void);

void sim_flush(void);

void sim_put_pixel(int32_t x, int32_t y, uint32_t argb);

int32_t sim_rand(void);
