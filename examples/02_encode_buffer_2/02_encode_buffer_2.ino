#include <Arduino.h>
#include <microbin.h>

void setup() {
    Serial.begin(115200);

    uint8_t *data = new uint8_t[512];

    MB::BufferEncoder e(data, 512);
    e.begin();
    e.writeListHeader(0x2);
    e.writeInt(2, false);
    e.writeInt(0xFEFFEEFEFEFFEEFEllu, false);

    size_t count = 64;
    unsigned *array = new unsigned[count];

    e.writeListHeader(count);
    for (size_t i = 0; i < count; i++) {
        e.writeInt(array[i], false);
    }
    delete[] array;

    Serial.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF", 16);
    Serial.write(data, e.getSize());
    delete[] data;
}

void loop() {
}
