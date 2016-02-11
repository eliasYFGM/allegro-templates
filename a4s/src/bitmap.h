// Simple C++ wrapper for BITMAP*

#ifndef BITMAP_HPP_INCLUDED
#define BITMAP_HPP_INCLUDED

#include <allegro.h>

typedef void (*sprite_drawer)(BITMAP*, BITMAP*, int, int);

class Bitmap
{
    BITMAP* bmp;

public:
    Bitmap() : bmp(0) {}
    ~Bitmap();

    Bitmap(int w, int h, int col = makecol(0, 0, 0));
    Bitmap(const char* filename, RGB* pal = 0);

    void create(int w, int h, int col = makecol(0, 0, 0));
    void load(const char* filename, RGB* pal = 0);
    void draw(BITMAP* dest, int x, int y, bool trans = false, sprite_drawer func = draw_sprite);
};

#endif // BITMAP_HPP_INCLUDED
