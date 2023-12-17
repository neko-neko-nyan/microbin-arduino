#include <Arduino.h>
#include <microbin.h>

void setup() {
    Serial.begin(115200);
    MB::PrintEncoder e {Serial};
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
}

void loop() {
}
