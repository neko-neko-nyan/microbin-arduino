#include "microbin.h"

bool MB::Encoder::begin(uint16_t magic, uint16_t count, uint8_t flags) {
        this->count = count;
        uint8_t header[8];
        header[0] = 'm';
        header[1] = 'b';
        header[2] = static_cast<uint8_t>(magic >> 8);
        header[3] = static_cast<uint8_t>(magic & 0xFF);
        header[4] = 0;
        header[5] = flags;
        header[6] = static_cast<uint8_t>(count >> 8);
        header[7] = static_cast<uint8_t>(count & 0xFF);
        return write(header, 8);
    }

bool MB::Encoder::writeObjectHeader(Type tag, uint64_t value, bool globl) {
    if (globl) {
        if (!count)
            return false;
        count--;
    }

    uint8_t t = static_cast<uint8_t>(tag);
    if (t > 7) {
        value = t & 0b1111;
        t = static_cast<uint8_t>(Type::SPECIAL);
    }
    t <<= 5;

    unsigned bits = 0;
    uint64_t var = value;
    for(; var; ++bits) var >>= 1;

    unsigned i = 0;
    while(bits > 4 - i && i < 5) {
        bits -= 8;
        i++;
    }

    t |= (0xFFu << (5 - i)) & 0b11111;
    if (i == 5) i = 8;
    else t |= value >> (8 * i);

    if (!write(&t, 1))
        return false;

    if (i != 8) value &= (1llu << (8 * i)) - 1llu;

    while (i) {
        i--;
        
        t = static_cast<uint8_t>((value >> (i * 8)) & 0xFF);
        if (!write(&t, 1))
            return false;
    }
    return true;
}
