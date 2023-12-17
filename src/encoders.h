#pragma once
#include "microbin.h"

#if MB_USE_ARDUINO_PRINT
#include <Print.h>
#endif

#if !MB_EMULATED_PROGMEM
#include <pgmspace.h>
#endif


#if !MB_EMULATED_PROGMEM
class __FlashStringHelper;
#endif


namespace MB {
#if MB_USE_ARDUINO_PRINT

class PrintEncoder : public Encoder {
private:
    Print &print;

protected:
    virtual bool write(const uint8_t *data, size_t size) override;

#if !MB_EMULATED_PROGMEM
    virtual bool write(const __FlashStringHelper *data, size_t size) override;
#endif

public:
    PrintEncoder(Print &print) : Encoder(), print(print) {}
};

#endif

class BufferEncoder : public Encoder {
protected:
    uint8_t *buffer = nullptr;
    size_t size = 0;
    size_t pos = 0;

    virtual bool ensure(size_t count);

    virtual bool write(const uint8_t *data, size_t size) override;

#if !MB_EMULATED_PROGMEM
    virtual bool write(const __FlashStringHelper *data, size_t size) override;
#endif

public:
    BufferEncoder(uint8_t *buffer, size_t size)
        : Encoder(), buffer(buffer), size(size) {}

    MB_INLINE size_t getSize() const {
        return pos;
    }
};

class ResizeableBufferEncoder : public BufferEncoder {
protected:
    virtual bool ensure(size_t count) override;

public:
    ResizeableBufferEncoder(uint8_t *buffer, size_t size) : BufferEncoder(buffer, size) {}
    ResizeableBufferEncoder() : ResizeableBufferEncoder(nullptr, 0) {}

    ~ResizeableBufferEncoder() {
        free(buffer);
    }

    MB_INLINE uint8_t *getBuffer() const {
        return buffer;
    }
};
}
