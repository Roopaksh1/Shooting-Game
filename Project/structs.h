#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <allegro5/allegro_audio.h>

struct heli{

    int x;
    int y;
    int boundx;
    int boundy;
    int speed;
    ALLEGRO_BITMAP *image[5];
};

struct obstacle{

    int x;
    int y;
    int speed;
    bool live;
    int boundx;
    int boundy;
    ALLEGRO_BITMAP *image;

};

struct explode{

    int x;
    int y;
    int sx;
    int sy;
    bool live;
    int boundx;
    int boundy;
    ALLEGRO_BITMAP *image;

};

#endif // STRUCTS_H_INCLUDED
