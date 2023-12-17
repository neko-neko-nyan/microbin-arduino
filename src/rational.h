#pragma once
#include "microbin.h"

namespace MB {
class Rational {
private:

public:
    Rational() {}

    uint64_t toIntBits() const;

    static Rational fromIntBits(uint64_t);

#error "Rational not implemented!"

};
}
