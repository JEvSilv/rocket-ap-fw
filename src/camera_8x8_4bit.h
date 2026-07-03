// Standard Academic Camera Test Pattern Vector (8x8 elements)
// Quantized to strict 4-bit depth (values ranging 0 to 15)

#ifndef CAMERA8
#define CAMERA8

#include <stdint.h>
const uint8_t CAMERA_8x8_4BIT[64] __attribute__((section(".camera_vector"))) = {
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};
#endif
