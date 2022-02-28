#ifndef HELI_H_INCLUDED
#define HELI_H_INCLUDED

#include "structs.h"

void initHeli(struct heli *copter);
void drawHeli(struct heli *copter);
void moveHeliUp(struct heli *copter);
void moveHeliDown(struct heli *copter);
void moveHeliRight(struct heli *copter);
void moveHeliLeft(struct heli *copter);


void initBullet(struct obstacle bullet[], int size);
void drawBullet(struct obstacle bullet[], int size);
void startBullet(struct obstacle bullet[], int size, struct heli *copter);
void updateBullet(struct obstacle bullet[], int size);
void collideBullet(struct obstacle bullet[], int bsize, struct obstacle meteor[], int msize, struct explode explosion[], int *score);
void reset(struct heli *copter, struct obstacle meteor[], struct obstacle bullet[], struct explode explosion[]);

#endif // HELI_H_INCLUDED
