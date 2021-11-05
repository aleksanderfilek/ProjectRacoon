#ifndef GAME_PHYSICS_H_
#define GAME_PHYSICS_H_

#include"Hero/Hero.h"

#include<stdbool.h>

typedef struct
{
  HeroFloat2 position;
  HeroFloat2 size;
} BoxCollider2D;

typedef struct
{
  HeroFloat2 position;
  float radius;
} CircleCollider2D;

typedef struct
{
  bool collided;
  HeroFloat2 direction;
} Collision;

Collision detectBoxCollision(const BoxCollider2D* boxA, const BoxCollider2D* boxB);
Collision detectCircleCollision(const CircleCollider2D* circleA, const CircleCollider2D* circleB);
Collision detectBoxCircleCollision(const BoxCollider2D* box, const CircleCollider2D* circle);

#endif