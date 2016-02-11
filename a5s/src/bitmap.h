// Simple C++ wrapper for ALLEGRO_BITMAP*
#ifndef BITMAP_HPP_INCLUDED
#define BITMAP_HPP_INCLUDED

#include <allegro5/allegro.h>

class Bitmap
{
    ALLEGRO_BITMAP* bmp;

public:
    Bitmap() : bmp(0) {}
    ~Bitmap();

    Bitmap(int w, int h, ALLEGRO_COLOR col = al_map_rgb(0, 0, 0));
    Bitmap(const char* filename);

    void create(int w, int h, ALLEGRO_COLOR col = al_map_rgb(0, 0, 0));
    void load(const char* filename);
    void draw(float x, float y, int flags = 0);
};

#endif // BITMAP_HPP_INCLUDED
