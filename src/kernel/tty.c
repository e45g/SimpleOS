#include "tty.h"
#include "vga.h"

static size_t VGA_WIDTH = 80;
static size_t VGA_HEIGHT= 25;

static uint16_t *terminal_buffer;
static uint8_t terminal_color;
static uint8_t terminal_y;
static uint8_t terminal_x;

void terminal_initialization() {
    terminal_buffer = (uint16_t *)0xB8000;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_y = 0;
    terminal_x = 0;
    for(size_t y = 0; y < VGA_HEIGHT; y++) {
        for(size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_set_color(uint8_t fg, uint8_t bg) {
    terminal_color = vga_entry_color(fg, bg);
}

void terminal_scroll(int direction) {
    direction++; // no unused var
}

void terminal_putcharat(char c, uint8_t color, size_t x, size_t y) {
    terminal_buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if(c == '\n') {
        terminal_x = 0;
        terminal_y++;
        return;
    }
    terminal_putcharat(c, terminal_color, terminal_x, terminal_y);
    if(++terminal_x >= VGA_WIDTH) {
        terminal_x = 0;
        if(++terminal_y >= VGA_HEIGHT) {
            terminal_scroll(1);
        }

    }
}

void terminal_write(const char *data, size_t size) {
    for(size_t i = 0; i < size; i++) {
        terminal_putchar(*(data+i));
    }
}

void terminal_writestring(const char *str) {
    size_t len = strlen(str);
    terminal_write(str, len);
}


void welcome_msg() {
    terminal_set_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("     _____       \n");
    terminal_writestring("    /     \\      \n");
    terminal_writestring("   |  O O  |     \n");
    terminal_writestring("   |   ^   |     ");

    terminal_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_writestring("Kernel OK\n");

    terminal_set_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK);
    terminal_writestring("   |  \\_/  |     \n");
    terminal_writestring("    \\_____/      \n");

    terminal_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
