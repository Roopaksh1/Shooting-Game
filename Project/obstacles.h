#ifndef OBSTACLES_H_INCLUDED
#define OBSTACLES_H_INCLUDED

#include "structs.h"


void initMeteor(struct obstacle meteor[], int size);
void drawMeteor(struct obstacle meteor[], int size);
void startMeteor(struct obstacle meteor[], int size);
void updateMeteor(struct obstacle meteor[], int size, int *lives);
void collideMeteor(struct obstacle meteor[], int size, struct heli *copter, struct explode explosion[], int *lives);

void initExplosion(struct explode explosion[], int size);
void drawExplosion(struct explode explosion[], int size);
void startExplosion(struct explode explosion[], int size, int x, int y);
void updateExplosion(struct explode explosion[], int size, ALLEGRO_SAMPLE *sample);
#endif // OBSTACLES_H_INCLUDED
