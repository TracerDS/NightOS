#pragma once

#include <cstdint>

inline void draw_pixel(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, uint32_t color) {
    if (x >= width || y >= height) return;
    
    // Calculate pixel position
    // Note: pitch is in bytes, we need to convert to pixels (divide by 4 for 32bpp)
    uint32_t *pixel = (uint32_t*)((uint8_t*)framebuffer + y * pitch + x * 4);
    *pixel = color;
}

inline void draw_hline(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, int len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        draw_pixel(framebuffer, width, height, pitch, x + i, y, color);
    }
}

// Draw a vertical line
inline void draw_vline(uint32_t *framebuffer, uint32_t width, uint32_t height, 
                uint32_t pitch, int x, int y, int len, uint32_t color) {
    for (int i = 0; i < len; i++) {
        draw_pixel(framebuffer, width, height, pitch, x, y + i, color);
    }
}

// Draw a rectangle
inline void draw_rect(uint32_t *framebuffer, uint32_t width, uint32_t height, 
               uint32_t pitch, int x, int y, int w, int h, uint32_t color) {
    draw_hline(framebuffer, width, height, pitch, x, y, w, color);
    draw_hline(framebuffer, width, height, pitch, x, y + h - 1, w, color);
    draw_vline(framebuffer, width, height, pitch, x, y, h, color);
    draw_vline(framebuffer, width, height, pitch, x + w - 1, y, h, color);
}

// Fill a rectangle
inline void fill_rect(uint32_t *framebuffer, uint32_t width, uint32_t height, 
               uint32_t pitch, int x, int y, int w, int h, uint32_t color) {
    for (int dy = 0; dy < h; dy++) {
        draw_hline(framebuffer, width, height, pitch, x, y + dy, w, color);
    }
}