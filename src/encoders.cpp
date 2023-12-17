#include "encoders.h"

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#if MB_USE_ARDUINO_PRINT 

bool MB::PrintEncoder::write(const uint8_t *data, size_t size) {
    return print.write(data, size) == size;
}

#if !MB_EMULATED_PROGMEM

bool MB::PrintEncoder::write(const __FlashStringHelper *data, size_t size) {
    PGM_P d = reinterpret_cast<PGM_P>(data);
    for (size_t i = 0; i < size; i++)
        if (!print.write(pgm_read_byte(&d[i])))
            return false;
    return true;
}

#endif
#endif


bool MB::BufferEncoder::ensure(size_t count) {
    if (size - pos < count)
        return false;
    pos += size;
    return true;
}

bool MB::BufferEncoder::write(const uint8_t *data, size_t size) {
    if (!ensure(size))
        return false;
    
    memcpy(&buffer[pos - size], data, size);
    return true;
}

#if !MB_EMULATED_PROGMEM

bool MB::BufferEncoder::write(const __FlashStringHelper *data, size_t size) {
    if (!ensure(size))
        return false;
    
    memcpy_P(&buffer[pos - size], data, size);
    return true;
}

#endif


bool MB::ResizeableBufferEncoder::ensure(size_t count) {
    if (size - pos >= count)
        return true;
    
    // Resize logic from gcc (libstdc++) std::vector
    // Note that `pos` is size and `count` is capacity
    // https://github.com/gcc-mirror/gcc/blob/da70c5b17123b7c81155ef03fb4591b71a681344/libstdc%2B%2B-v3/include/bits/stl_vector.h#L1908
    size_t newSize = pos + MAX(pos, count);
    uint8_t *newBuffer = (uint8_t *) realloc(buffer, newSize);
    if (!newBuffer)
        return false;
    
    size = newSize;
    buffer = newBuffer;
    pos += size;
    return true;
}
