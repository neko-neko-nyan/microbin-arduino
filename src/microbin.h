#pragma once
#include <stdint.h>

// Enable Float16 type support
#ifndef MB_FLOAT16_SUPPORTED
#define MB_FLOAT16_SUPPORTED 0
#pragma GCC info "Float16 support not enabled so writeFloat16 will not work!"
#endif

// Enable Rational type support
#ifndef MB_RATIONAL_SUPPORTED
#define MB_RATIONAL_SUPPORTED 0
#pragma GCC info "Rational support not enabled so writeRational will not work!"
#endif

// Use String from arduino
#ifndef MB_USE_ARDUINO_STRING
#ifdef ARDUINO
#define MB_USE_ARDUINO_STRING 1
#else
#define MB_USE_ARDUINO_STRING 0
#endif 
#endif

// Use Print from arduino
#ifndef MB_USE_ARDUINO_PRINT
#ifdef ARDUINO
#define MB_USE_ARDUINO_PRINT 1
#else
#define MB_USE_ARDUINO_PRINT 0
#endif 
#endif

// MCU's PROGMEM is emulated, we can read it without special functions
#ifndef MB_EMULATED_PROGMEM
#ifdef ESP32
#define MB_EMULATED_PROGMEM 1
#else
#define MB_EMULATED_PROGMEM 0
#endif 
#endif

#define MB_INLINE inline __attribute__((always_inline))

namespace MB {
    enum class Type : uint8_t {
        SPECIAL  = 0b000,  //
        RATIONAL = 0b001,  // 
        INT      = 0b100,  // inline, max 64
        NINT     = 0b010,  // inline, max 64
        STRING   = 0b011,  // sized, inline
        BYTES    = 0b101,  // sized, inline
        LIST     = 0b110,  // sized
        MAP      = 0b111,  // sized

        NONE     = 0b10000,  // singleton
        FALSE    = 0b10001,  // singleton
        TRUE     = 0b10010,  // singleton
        FLOAT16  = 0b10011,  // 2 bytes
        FLOAT32  = 0b10100,  // 4 bytes
        FLOAT64  = 0b10101,  // 8 bytes
    };
}

#if MB_RATIONAL_SUPPORTED
#include "rational.h"
#endif
#include "encoder.h"
#include "encoders.h"
#include "decoder.h"
