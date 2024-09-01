#include "VGA.h"

bool VGA::init(int width, int height) {
    _frameWidth = width;
    _frameHeight = height;
    _bufferSize = (_frameWidth * _frameHeight) / 8;
    _frameBuffer = (uint8_t*)malloc(_bufferSize);

    if (!_frameBuffer) {
        Serial.println("Failed to allocate frame buffer");
        return false;
    }

    // Assign GPIO pins
    _hsyncPin = 12;  // Horizontal Sync
    _vsyncPin = 13;  // Vertical Sync
    _redPin = 1;     // Red
    _greenPin = 2;   // Green
    _bluePin = 3;    // Blue

    // Initialize pins
    pinMode(_hsyncPin, OUTPUT);
    pinMode(_vsyncPin, OUTPUT);
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);

    return true;
}

void VGA::vsyncWait() {
    // Placeholder: Insert vertical sync waiting logic
    delay(16); // Simple delay for a 60Hz refresh rate
}

void VGA::swapBuffers() {
    drawFrame();
}

uint8_t* VGA::getDrawBuffer() {
    return _frameBuffer;
}

void VGA::generateSyncSignals() {
    // Horizontal Sync Pulse
    digitalWrite(_hsyncPin, LOW);
    delayMicroseconds(4);  // 4us H-sync pulse width
    digitalWrite(_hsyncPin, HIGH);

    // Vertical Sync Pulse
    static int lineCount = 0;
    lineCount++;
    if (lineCount >= _frameHeight) {
        lineCount = 0;
        digitalWrite(_vsyncPin, LOW);
        delayMicroseconds(64);  // 64us V-sync pulse width
        digitalWrite(_vsyncPin, HIGH);
    }
}

void VGA::drawFrame() {
    for (int y = 0; y < _frameHeight; y++) {
        generateSyncSignals();
        for (int x = 0; x < _frameWidth; x++) {
            int index = (y * _frameWidth + x) / 8;
            int bitPos = (x % 8) * 3;
            uint8_t pixelBits = (_frameBuffer[index] >> (7 - bitPos)) & 0x07;

            digitalWrite(_redPin, pixelBits & 0x01);
            digitalWrite(_greenPin, (pixelBits >> 1) & 0x01);
            digitalWrite(_bluePin, (pixelBits >> 2) & 0x01);
        }
    }
}
