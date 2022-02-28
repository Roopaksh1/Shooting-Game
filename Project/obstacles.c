#include <allegro5/allegro.h>
#include "constants.h"
#include "structs.h"

extern ALLEGRO_SAMPLE_INSTANCE *instance2;

void initExplosion(struct explode explosion[], int size){

    for(int i=0; i<size; i++){

        explosion[i].boundx = 192;
        explosion[i].boundy = 192;
        explosion[i].sx = 0;
        explosion[i].sy = 0;
        explosion[i].live = false;
        explosion[i].image = al_load_bitmap("Resources\\bitmaps\\explosion.png");
    }
}

void drawExplosion(struct explode explosion[], int size){

    for(int i=0; i<size; i++){

        if(explosion[i].live){

            al_draw_bitmap_region(explosion[i].image, explosion[i].sx, explosion[i].sy, explosion[i].boundx,
            explosion[i].boundy, explosion[i].x - explosion[i].boundx/2, explosion[i].y - explosion[i].boundy/2, 0);
        }
    }
}

void startExplosion(struct explode explosion[], int size, int x, int y){

    al_play_sample_instance(instance2);

    for(int i=0; i<size; i++){

        if(!explosion[i].live){

            explosion[i].live = true;
            explosion[i].x = x;
            explosion[i].y = y;
            break;
        }
    }
}

void updateExplosion(struct explode explosion[], int size){

    for(int i=0; i<size; i++){

        if(explosion[i].live){

            explosion[i].sx += explosion[i].boundx;

            if(explosion[i].sx > 960){

                explosion[i].sx = 0;
                explosion[i].sy += explosion[i].boundy;
            }

            if(explosion[i].sy > 576){
                explosion[i].sy = 0;
                explosion[i].live = false;
            }
        }
    }
}

void initMeteor(struct obstacle meteor[], int size){

    for(int i=0; i<size; i++){

        meteor[i].boundx = 100;
        meteor[i].boundy = 52;
        meteor[i].speed = 5;
        meteor[i].live = false;
        meteor[i].image = al_load_bitmap("Resources\\bitmaps\\meteor.png");
    }
}

void drawMeteor(struct obstacle meteor[], int size){

    for(int i=0; i<size; i++){

        if(meteor[i].live){

            al_draw_bitmap(meteor[i].image, meteor[i].x, meteor[i].y, 0);
        }
    }
}

void startMeteor(struct obstacle meteor[], int size){

    for(int i=0; i<size; i++){

        if(!meteor[i].live){

            if(rand() % 500 == 0){

                meteor[i].x = WIDTH;
                meteor[i].live = true;
                meteor[i].y = rand() % (HEIGHT - 60);
                break;
            }
        }
    }
}

void updateMeteor(struct obstacle meteor[], int size, int *lives){

    for(int i=0; i<size; i++){

        if(meteor[i].live){

            meteor[i].x -= meteor[i].speed;

            if(meteor[i].x + meteor[i].boundx < 0){
                meteor[i].live = false;
                (*lives)--;
            }
        }
    }
}

void collideMeteor(struct obstacle meteor[], int size, struct heli *copter, struct explode explosion[], int *lives){

    for(int i=0; i<size; i++){

        if(meteor[i].live){

            if(meteor[i].x < copter->x + copter->boundx && meteor[i].x + meteor[i].boundx > copter->x
               && meteor[i].y < copter->y + copter->boundy && meteor[i].y + meteor[i].boundy > copter->y){

                    (*lives)--;
                    meteor[i].live = false;
                    startExplosion(explosion, NUM_EXPLOSION, copter->x + 50, copter->y);
               }
        }
    }
}
