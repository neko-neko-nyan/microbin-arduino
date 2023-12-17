#pragma once
#include "microbin.h"
#include <stdint.h>
#include <string.h>

#if !MB_EMULATED_PROGMEM
#include <pgmspace.h>
#endif

#if MB_USE_ARDUINO_STRING
#include <WString.h>
#endif

class __FlashStringHelper;

namespace MB {
class Rational;

class Encoder {
private:
    uint16_t count = 0;

    bool writeObjectHeader(Type tag, uint64_t value, bool globl);

protected:
    virtual bool write(const uint8_t *data, size_t size) = 0;

#if !MB_EMULATED_PROGMEM
    virtual bool write(const __FlashStringHelper *data, size_t size) = 0;
#endif

public:
    Encoder() {}

    bool begin(uint16_t magic = 0x0000, uint16_t count = 1, uint8_t flags = 0);

    MB_INLINE uint16_t getCount() const {
        return count;
    }

    MB_INLINE bool writeRational(const Rational &value, bool _globl = true) {
#if MB_RATIONAL_SUPPORTED
        return writeObjectHeader(Type::RATIONAL, value.toIntBits(), _globl);
#else
        return false;
#endif
    }


    // integers

    template <typename T>
    MB_INLINE bool writeUint(T value, bool _globl = true) {
        return writeObjectHeader(Type::INT, value, _globl);
    }

    template <typename T>
    MB_INLINE bool writeNint(T value, bool _globl = true) {
        return writeObjectHeader(Type::NINT, value, _globl);
    }

    template <typename T>
    MB_INLINE bool writeInt(T value, bool _globl = true) {
        if (value >= 0)
            return writeObjectHeader(Type::INT, value, _globl);
        else
            return writeObjectHeader(Type::NINT, -value, _globl);
    }


    // strings

    MB_INLINE bool writeString(const char *value, size_t size, bool _globl = true) {
        if (!writeObjectHeader(Type::STRING, size, _globl))
            return false;
        return write(reinterpret_cast<const uint8_t*>(value), size);
    }

    MB_INLINE bool writeString(const char *value, bool _globl = true) {
        return writeString(value, strlen(value), _globl);
    }

#if MB_USE_ARDUINO_STRING
    MB_INLINE bool writeString(String value, bool _globl = true) {
        return writeString(value.c_str(), value.length(), _globl);
    }
#endif

    MB_INLINE bool writeString(const __FlashStringHelper *value, size_t size, bool _globl = true) {
#if MB_EMULATED_PROGMEM
        return writeString(reinterpret_cast<const char*>(value), size, _globl);
#else
        if (!writeObjectHeader(Type::STRING, size, _globl))
            return false;
        return write(value, size);
#endif
    }

    MB_INLINE bool writeString(const __FlashStringHelper *value, bool _globl = true) {
#if MB_EMULATED_PROGMEM
        return writeString(reinterpret_cast<const char*>(value), _globl);
#else
        return writeString(value, strlen_P((PGM_P) value), _globl);
#endif
    }

    MB_INLINE bool writeEmptyString(bool _globl = true) {
        return writeObjectHeader(Type::STRING, 0, _globl);
    }


    // bytes

    MB_INLINE bool writeBytes(const uint8_t *value, size_t size, bool _globl = true) {
        if (!writeObjectHeader(Type::BYTES, size, _globl))
            return false;
        return write(value, size);
    }

    MB_INLINE bool writeBytes(const char *value, size_t size, bool _globl = true) {
        return writeBytes(reinterpret_cast<const uint8_t*>(value), size, _globl);
    }

#if MB_USE_ARDUINO_STRING
    MB_INLINE bool writeBytes(String value, bool _globl = true) {
        return writeBytes(value.c_str(), value.length(), _globl);
    }
#endif

    MB_INLINE bool writeBytes(const __FlashStringHelper *value, size_t size, bool _globl = true) {
#if MB_EMULATED_PROGMEM
        return writeBytes(reinterpret_cast<const char*>(value), size, _globl);
#else
        if (!writeObjectHeader(Type::BYTES, size, _globl))
            return false;
        return write(value, size);
#endif
    }

    MB_INLINE bool writeBytes(const __FlashStringHelper *value, bool _globl = true) {
#if MB_EMULATED_PROGMEM
        return writeBytes(reinterpret_cast<const uint8_t*>(value), _globl);
#else
        return writeBytes(value, strlen_P((PGM_P) value), _globl);
#endif
    }

    MB_INLINE bool writeEmptyBytes(bool _globl = true) {
        return writeObjectHeader(Type::BYTES, 0, _globl);
    }

    // singletons

    MB_INLINE bool writeNone(bool _globl = true) {
        return writeObjectHeader(Type::NONE, 0, _globl);
    }

    MB_INLINE bool writeBool(bool value, bool _globl = true) {
        return writeObjectHeader(value ? Type::TRUE : Type::FALSE, 0, _globl);
    }


    // floats

    MB_INLINE bool writeFloat16(float value, int size = 32, bool _globl = true) {
#if MB_FLOAT16_SUPPORTED
        if (!writeObjectHeader(Type::FLOAT16, size, _globl))
            return false;
        float16_t hvalue = value;
        return write(reinterpret_cast<uint8_t*>(&hvalue), 2);
#else
        return false;
#endif
    }

    MB_INLINE bool writeFloat16(double value, int size = 32, bool _globl = true) {
#if MB_FLOAT16_SUPPORTED
        if (!writeObjectHeader(Type::FLOAT16, size, _globl))
            return false;
        float16_t hvalue = value;
        return write(reinterpret_cast<uint8_t*>(&hvalue), 2);
#else
        return false;
#endif
    }


    MB_INLINE bool writeFloat32(float value, int size = 32, bool _globl = true) {
        if (!writeObjectHeader(Type::FLOAT32, size, _globl))
            return false;
        return write(reinterpret_cast<uint8_t*>(&value), 4);
    }

    MB_INLINE bool writeFloat32(double value, int size = 32, bool _globl = true) {
        if (!writeObjectHeader(Type::FLOAT16, size, _globl))
            return false;
        float fvalue = value;
        return write(reinterpret_cast<uint8_t*>(&fvalue), 4);
    }


    MB_INLINE bool writeFloat64(float value, int size = 32, bool _globl = true) {
        if (!writeObjectHeader(Type::FLOAT64, size, _globl))
            return false;
        double dvalue = value;
        return write(reinterpret_cast<uint8_t*>(&dvalue), 8);
    }

    MB_INLINE bool writeFloat64(double value, int size = 32, bool _globl = true) {
        if (!writeObjectHeader(Type::FLOAT64, size, _globl))
            return false;
        return write(reinterpret_cast<uint8_t*>(&value), 8);
    }

    MB_INLINE bool writeFloat(float value, int size = 32, bool _globl = true) {
        switch (size) {
            case 16:
                return writeFloat16(value, _globl);
            case 32:
                return writeFloat32(value, _globl);
            case 64:
                return writeFloat64(value, _globl);
            default:
                return false;
        }
    }

    MB_INLINE bool writeFloat(double value, int size = 32, bool _globl = true) {
        switch (size) {
            case 16:
                return writeFloat16(value, _globl);
            case 32:
                return writeFloat32(value, _globl);
            case 64:
                return writeFloat64(value, _globl);
            default:
                return false;
        }
    }


    // compounds

    MB_INLINE bool writeListHeader(size_t size, bool _globl = true) {
        return writeObjectHeader(Type::LIST, size, _globl);
    }

    MB_INLINE bool writeMapHeader(size_t size, bool _globl = true) {
        return writeObjectHeader(Type::MAP, size, _globl);
    }
};
}
