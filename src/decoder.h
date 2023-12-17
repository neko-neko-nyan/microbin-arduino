#pragma once

#include "microbin.h"

namespace MB {
class Decoder {
private:
    const uint8_t *data;
    size_t maxsize;
    size_t pos = 0;
    uint16_t count = 0;
    uint8_t flags = 0;

    MB_INLINE bool ensure(size_t count) {
        pos += count;
        return pos < maxsize;
    }

    MB_INLINE bool decodeSpecificObject(Type type, uint64_t &value) {
        Type t;
        if (!decodeObjectHeader(t, value))
            return false;
        if (t != type)
            return false;
        return true;
    }

public:
    Decoder(const uint8_t *data, size_t maxsize) : data(data), maxsize(maxsize) {}

    bool begin(uint16_t magic = 0x0000);

    bool decodeObjectHeader(Type &type, uint64_t &value);

    MB_INLINE uint8_t getFlags() const {
        return flags;
    }

    MB_INLINE uint16_t getCount() const {
        return count;
    }

    bool skipValue(Type type, uint64_t value);

    bool skip(size_t count = 1);

    MB_INLINE bool decodeRational(Rational &value) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::RATIONAL, ivalue))
            return false;

#if MB_RATIONAL_SUPPORTED
        value = Rational::fromIntBits(ivalue);
        return true;
#else
        return false;
#endif
    }


    // integers

    template <typename T>
    MB_INLINE bool decodeUint(T &value) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::INT, ivalue))
            return false;
        
        value = ivalue;
        return true;
    }

    template <typename T>
    MB_INLINE bool decodeNint(T &value) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::NINT, ivalue))
            return false;
        
        value = ivalue;
        return true;
    }

    template <typename T>
    MB_INLINE bool decodeInt(T &value) {
        Type type;
        uint64_t ivalue;
        if (!decodeObjectHeader(type, ivalue))
            return false;

        if (type == Type::INT)
            value = ivalue;

        else if (type == Type::NINT)
            value = -ivalue;
        
        else return false;
        
        return true;
    }


    // strings

    MB_INLINE bool decodeString(const char *&value, size_t &ssize) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::STRING, ivalue))
            return false;
        
        value = reinterpret_cast<const char *>(&data[pos]);
        ssize = ivalue;
        return ensure(ivalue);
    }


    // bytes

    MB_INLINE bool decodeBytes(const char *&value, size_t &ssize) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::BYTES, ivalue))
            return false;
        
        value = reinterpret_cast<const char *>(&data[pos]);
        ssize = ivalue;
        return ensure(ivalue);
    }


    // singletons

    MB_INLINE bool decodeNone(const char *value, size_t &ssize) {
        uint64_t ivalue;
        return decodeSpecificObject(Type::NONE, ivalue);
    }

    MB_INLINE bool decodeBool(bool &value) {
        Type type;
        uint64_t ivalue;
        if (!decodeObjectHeader(type, ivalue))
            return false;

        if (type == Type::TRUE)
            value = true;

        else if (type == Type::FALSE)
            value = false;
        
        else return false;
        
        return true;
    }

    
    // floats

    bool decodeFloat(float &value) {
        Type type;
        uint64_t ivalue;
        if (!decodeObjectHeader(type, ivalue))
            return false;

        if (type == Type::FLOAT64) {
            if (!ensure(8))
                return false;
            
            value = *reinterpret_cast<const double*>(&data[pos - 8]);
        
        } else if (type == Type::FLOAT32) {
            if (!ensure(4))
                return false;
            
            value = *reinterpret_cast<const float*>(&data[pos - 4]);
        
        } else if (type == Type::FLOAT16) {
            if (!ensure(2))
                return false;
            
#if MB_FLOAT16_SUPPORTED
            value = *reinterpret_cast<const float16_t*>(&data[pos - 2]);
else
            return false;
#endif
        
        } else return false;
        return true;
    }

    bool decodeFloat(double &value) {
        Type type;
        uint64_t ivalue;
        if (!decodeObjectHeader(type, ivalue))
            return false;

        if (type == Type::FLOAT64) {
            if (!ensure(8))
                return false;
            
            value = *reinterpret_cast<const double*>(&data[pos - 8]);
        
        } else if (type == Type::FLOAT32) {
            if (!ensure(4))
                return false;
            
            value = *reinterpret_cast<const float*>(&data[pos - 4]);
        
        } else if (type == Type::FLOAT16) {
            if (!ensure(2))
                return false;
            
#if MB_FLOAT16_SUPPORTED
            value = *reinterpret_cast<const float16_t*>(&data[pos - 2]);
else
            return false;
#endif
        
        } else return false;
        return true;
    }


    // compounds

    MB_INLINE bool decodeListHeader(size_t &count) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::LIST, ivalue))
            return false;
        
        count = ivalue;
        return true;
    }

    MB_INLINE bool decodeMapHeader(size_t &count) {
        uint64_t ivalue;
        if (!decodeSpecificObject(Type::MAP, ivalue))
            return false;
        
        count = ivalue;
        return true;
    }
};
}
