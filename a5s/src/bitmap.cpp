// Simple C++ wrapper for ALLEGRO_BITMAP*
#include <iostream>
#include "bitmap.h"
#include "gameengine.h"

Bitmap::Bitmap(int w, int h, ALLEGRO_COLOR col)
{
    create(w, h, col);
}

Bitmap::Bitmap(const char* filename)
{
    load(filename);
}

Bitmap::~Bitmap()
{
    if (bmp)
    {
        al_destroy_bitmap(bmp);
    }
}

void Bitmap::create(int w, int h, ALLEGRO_COLOR col)
{
    if (bmp)
    {
        al_destroy_bitmap(bmp);
    }

    bmp = al_create_bitmap(w, h);

    al_set_target_bitmap(bmp);
    al_clear_to_color(col);
}

void Bitmap::load(const char* filename)
{
    if (bmp)
    {
        al_destroy_bitmap(bmp);
    }

    bmp = al_load_bitmap(filename);

    if (!bmp)
    {
        std::cout << "Error loding " << filename << std::endl;
        create(32, 32);
    }
}

void Bitmap::draw(float x, float y, int flags)
{
    al_draw_bitmap(bmp, x, y, flags);
}
