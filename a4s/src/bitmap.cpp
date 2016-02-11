// Simple C++ wrapper for BITMAP*

#include <iostream>
#include "bitmap.h"

Bitmap::Bitmap(int w, int h, int col)
{
    create(w, h, col);
}

Bitmap::Bitmap(const char* filename, RGB* pal)
{
    load(filename, pal);
}

Bitmap::~Bitmap()
{
    if (bmp)
    {
        destroy_bitmap(bmp);
    }
}

void Bitmap::create(int w, int h, int col)
{
    if (bmp)
    {
        destroy_bitmap(bmp);
    }

    bmp = create_bitmap(w, h);
    clear_to_color(bmp, col);
}

void Bitmap::load(const char* filename, RGB* pal)
{
    if (bmp)
    {
        destroy_bitmap(bmp);
    }

    bmp = load_bitmap(filename, pal);

    if (!bmp)
    {
        std::cout << "Failed to load " << filename << std::endl;
        create(32, 32);
    }
}

void Bitmap::draw(BITMAP* dest, int x, int y, bool trans, sprite_drawer func)
{
    if (trans)
    {
        (*func)(dest, bmp, x, y);
    }
    else
    {
        blit(bmp, dest, 0, 0, x, y, bmp->w, bmp->h);
    }
}
