#pragma once
#include <Arduino.h>

class VGA {
public:
    VGA() {}
    ~VGA() {}

    bool init(int width, int height);
    void vsyncWait();
    void swapBuffers();
    uint8_t* getDrawBuffer();

    int frameWidth() { return _frameWidth; }
    int frameHeight() { return _frameHeight; }

protected:
    uint8_t* _frameBuffer;
    int _frameWidth;
    int _frameHeight;
    int _bufferSize;
    int _hsyncPin;
    int _vsyncPin;
    int _redPin;
    int _greenPin;
    int _bluePin;
    
    void generateSyncSignals();
    void drawFrame();
};
