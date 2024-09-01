#include "VGA.h"

// Create a VGA object
VGA vga;

// Function to generate a simple test pattern
void generateTestPattern(uint8_t* buffer, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) / 8;
            int bitPos = 7 - (x % 8) * 3;
            uint8_t color = ((x / 8) % 2 == (y / 8) % 2) ? 0b111 : 0b000; // Checkerboard pattern
            buffer[index] &= ~(0b111 << bitPos);  // Clear the bits
            buffer[index] |= (color << bitPos);   // Set the color bits
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Initialize VGA with a resolution of 320x200
    if (!vga.init(320, 200)) {
        Serial.println("Failed to initialize VGA");
        while (1);
    }

    // Generate a test pattern in the draw buffer
    uint8_t* drawBuffer = vga.getDrawBuffer();
    generateTestPattern(drawBuffer, vga.frameWidth(), vga.frameHeight());
}

void loop() {
    // Wait for VSync
    vga.vsyncWait();

    // Swap buffers
    vga.swapBuffers();

    // Update the draw buffer with a new pattern or animation
    uint8_t* drawBuffer = vga.getDrawBuffer();
    generateTestPattern(drawBuffer, vga.frameWidth(), vga.frameHeight());
}