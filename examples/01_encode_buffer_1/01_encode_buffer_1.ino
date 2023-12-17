#include <microbin.h>
#include <Arduino.h>


void setup() {
    MB::ResizeableBufferEncoder e {};
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


    Serial.begin(115200);
    Serial.write(e.getBuffer(), e.getSize());
}

void loop() {
}
