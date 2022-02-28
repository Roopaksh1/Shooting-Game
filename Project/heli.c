#include <allegro5/allegro.h>
#include <stdio.h>
#include "obstacles.h"
#include "constants.h"



static char str[32];
static int count =0;
extern ALLEGRO_SAMPLE_INSTANCE *instance1, *instance3;

void initHeli(struct heli *copter){

    copter->x = 0;
    copter->y = 200;
    copter->boundx = 105;
    copter->boundy = 48;
    copter->speed = 6;

    for(int i = 1; i<= 5; i++){
        sprintf(str, "Resources\\bitmaps\\copter%d.bmp", i);
        copter->image[i-1] = al_load_bitmap(str);
    }
}

void moveHeliUp(struct heli *copter){

    copter->y -= copter->speed;
    if(copter->y < 0)
        copter->y += copter->speed;
    al_play_sample_instance(instance1);
}

void moveHeliDown(struct heli *copter){

    copter->y += copter->speed;
    if(copter->y + copter->boundy > HEIGHT)
        copter->y -= copter->speed;
    al_play_sample_instance(instance1);
}

void moveHeliRight(struct heli *copter){

    copter->x += copter->speed;
    if(copter->x + copter->boundx > WIDTH)
        copter->x -= copter->speed;
    al_play_sample_instance(instance1);
}

void moveHeliLeft(struct heli *copter){

    copter->x -= copter->speed;
    if(copter->x < 0)
        copter->x += copter->speed;
    al_play_sample_instance(instance1);
}

void drawHeli(struct heli *copter){

    if(count==4)
        count=0;

    al_draw_bitmap(copter->image[count++], copter->x, copter->y, 0);
}

void initBullet(struct obstacle bullet[], int size){

    for(int i=0; i<size; i++){

        bullet[i].speed = 6;
        bullet[i].boundx = 50;
        bullet[i].boundy = 35;
        bullet[i].live = false;
        bullet[i].image = al_load_bitmap("Resources\\bitmaps\\bullet.png");
    }
}

void startBullet(struct obstacle bullet[], int size, struct heli *copter){

    for(int i=0; i<size; i++){

        if(!bullet[i].live){

            bullet[i].live = true;
            bullet[i].x = copter->x + copter->boundx;
            bullet[i].y = copter->y + copter->boundy;
            break;
        }
    }
}

void drawBullet(struct obstacle bullet[], int size){

    for(int i=0; i<size; i++){

        if(bullet[i].live)
            al_draw_bitmap(bullet[i].image, bullet[i].x, bullet[i].y, 0);
    }
}

void updateBullet(struct obstacle bullet[], int size){

    for(int i=0; i<size; i++){

        if(bullet[i].live){

            bullet[i].x += bullet[i].speed;

            if(bullet[i].x > WIDTH)
                bullet[i].live = false;
        }
    }
}

void collideBullet(struct obstacle bullet[], int bsize, struct obstacle meteor[], int msize, struct explode explosion[], int *score){

    for(int i=0; i<bsize; i++){

        if(bullet[i].live){

            for(int j=0; j<msize; j++){

                if(meteor[j].live){

                    if(bullet[i].x < meteor[j].x + meteor[j].boundx && bullet[i].x + bullet[i].boundx > meteor[j].x
                       && bullet[i].y < meteor[j].y + meteor[j].boundy && bullet[i].y + bullet[i].boundy > meteor[j].y){

                            (*score)++;
                            meteor[j].live = false;
                            bullet[i].live = false;
                            startExplosion(explosion, NUM_EXPLOSION, bullet[i].x , bullet[i].y);
                   }
                }
            }
        }
    }
}

void reset(struct heli *copter, struct obstacle meteors[], struct obstacle bullet[], struct explode explosion[]){

    initHeli(copter);
    initMeteor(meteors, NUM_METEORS);
    initBullet(bullet, NUM_BULLETS);
    initExplosion(explosion, NUM_EXPLOSION);
}
