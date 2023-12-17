#include "decoder.h"

bool MB::Decoder::begin(uint16_t magic) {
    if (!ensure(8))
        return false;

    if (data[0] != 'm' || data[1] != 'b')
        return false;  // bad file magic

    if (data[4] != 0)
        return false;  // unknown version

    uint16_t mag = (static_cast<uint16_t>(data[2]) << 8) | static_cast<uint16_t>(data[3]);
    if (mag != magic)
        return false; // bad app magic

    flags = data[5];
    count = (static_cast<uint16_t>(data[6]) << 8) | static_cast<uint16_t>(data[7]);
}

bool MB::Decoder::decodeObjectHeader(Type &type, uint64_t &value) {
    if (!ensure(1))
        return false;
    
    uint8_t t = data[pos-1];
    type = static_cast<Type>(t >> 5);
    t &= 0b11111;

    uint8_t i = 0;
    while (i != 5 && t & (1 << (4 - i))) i++;
    t &= (1 << (5 - i)) - 1;
    value = t;

    if (i == 5)
        i = 8;

    if (!ensure(i))
        return false;

    while (i) {
        value <<= 8;
        value |= data[pos - i];
        i--;
    }

    if (type == Type::SPECIAL)
        type = static_cast<Type>((1 << 4) | value);

    return true;
}

bool MB::Decoder::skipValue(Type type, uint64_t value) {
    switch (type) {
        case Type::STRING:
        case Type::BYTES:
            return ensure(value);

        case Type::LIST:
            return skip(value);

        case Type::MAP:
            return skip(value*2);
        
        case Type::FLOAT16:
            return ensure(2);
        case Type::FLOAT32:
            return ensure(4);
        case Type::FLOAT64:
            return ensure(8);
        
        default:
            return true;
    }
}

bool MB::Decoder::skip(size_t count) {
    Type type;
    uint64_t value;

    for (size_t i = 0; i < count; count++) {
        if (!decodeObjectHeader(type, value))
            return false;
        
        if (!skipValue(type, value))
            return false;
    }
}
