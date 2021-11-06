#include"Game/physics.h"

#include<stdbool.h>
#include<math.h>
#include<stdlib.h>

Collision detectBoxCollision(const BoxCollider2D* boxA, const BoxCollider2D* boxB)
{
  bool collisionX = boxA->position.x + boxA->size.x >= boxB->position.x &&
    boxB->position.x + boxB->size.x >= boxA->position.x;

  bool collisionY = boxA->position.y + boxA->size.y >= boxB->position.y &&
    boxB->position.y + boxB->size.y >= boxA->position.y;

  return (Collision){ collisionX && collisionY, heroFloat2zero};
}

Collision detectCircleCollision(const CircleCollider2D* circleA, const CircleCollider2D* circleB)
{
  float dx = circleA->position.x - circleB->position.x;
  float dy = circleA->position.y - circleB->position.y;
  float D2 = powf(dx, 2) + powf(dy, 2);
  float r = circleA->radius + circleB->radius;
  float R2 = powf(r, 2);

  return (Collision){ D2 > R2, heroFloat2zero};
}

Collision detectBoxCircleCollision(const BoxCollider2D* box, const CircleCollider2D* circle)
{
  HeroFloat2 center = (HeroFloat2){ circle->position.x + circle->radius, circle->position.y + circle->radius };
  HeroFloat2 aabbHalfExtents = (HeroFloat2){ box->size.x / 2.0f, box->size.y / 2.0f };
  HeroFloat2 aabbCenter = (HeroFloat2){ 
    box->position.x + aabbHalfExtents.x, 
    box->position.y + aabbHalfExtents.y };
  HeroFloat2 difference  = heroMathSubstractF2(center, aabbCenter);
  HeroFloat2 clamped = (HeroFloat2){
    heroMathClampF(difference.x, -aabbHalfExtents.x, aabbHalfExtents.x),
    heroMathClampF(difference.y, -aabbHalfExtents.y, aabbHalfExtents.y)
  };
  HeroFloat2 closest = heroMathAddF2(aabbCenter, clamped);
  difference = heroMathSubstractF2(closest, center);
  
  if(heroMathLengthF2(difference) < circle->radius)
  {
    return (Collision){true, difference };
  }
  return (Collision){ false, heroFloat2zero };
}