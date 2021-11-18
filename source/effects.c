#include"Game/effects.h"

#include<stdlib.h>
#include<string.h>

EffectsManager* effectsManagerCreate()
{
  EffectsManager* manager = (EffectsManager*)malloc(sizeof(EffectsManager));
  memset(manager, 0, sizeof(EffectsManager));

  manager->spriteSheet = gameSpriteSheetLoad("assets/sprites/powerups.he");

  // default collider size
  manager->collider.size = (HeroFloat2){ 16.0f, 16.0f };

  return manager;
}
#include<stdio.h>
void effectsManagerUpdate(EffectsManager* manager, double deltaTime, GameRacket* racket)
{
  for(int i = 0; i < EFFECTS_CAPACITY; i++)
  {
    
    if(manager->types[i] == EFFECT_NONE)
    {
      continue;
    }

    HeroFloat2* position = &manager->positions[i];
    // position update
    position->y += EFFECTS_SPEED * deltaTime;

    // removing effect, effect outside bound
    if(position->y > 720)
    {
      manager->types[i] = EFFECT_NONE;
      manager->length--;
    }

    // checking collisions
    manager->collider.position = *position;
    Collision collision = detectBoxCollision(&racket->collider, &manager->collider);
    if(collision.collided == true)
    {
      effectsManagerCollisionResolve(manager, manager->types[i]);
    }
  }
}

void effectsManagerDraw(EffectsManager* manager, HeroSpriteBatch* spriteBatch)
{
  // drawing sprites
  for(int i = 0; i < EFFECTS_CAPACITY; i++)
  {
    if(manager->types[i] == EFFECT_NONE)
    {
      continue;
    }
    
    HeroInt2 position = { (int)manager->positions[i].x, manager->positions[i].y };
    HeroInt2 size = { (int)manager->collider.size.x, manager->collider.size.y };
    heroSpriteBatchDrawTextureEx(spriteBatch, manager->spriteSheet->texture,
      position, size, manager->spriteSheet->rects[(int)manager->types[i]-1], 0.0f, 
      (HeroColor){0xFF, 0xFF, 0xFF, 0xFF});
  }
}

void effectsManagerDestroy(EffectsManager* manager)
{
  gameSpriteSheetUnload(manager->spriteSheet);
  free(manager);
}

void effectsManagerAdd(EffectsManager* manager, EffectEnum type, HeroInt2 position)
{
  // check if buffer is full
  if(manager->length >= EFFECTS_CAPACITY)
  {
    return;
  }

  // find slot
  int i;
  for(i = 0; i < EFFECTS_CAPACITY; i++)
  {
    if(manager->types[i] == EFFECT_NONE)
    {
      break;
    }
  }

  // set data
  manager->types[i] = type;
  manager->positions[i] = (HeroFloat2){ (float)position.x, (float)position.y };
  manager->length++;
}

void effectsManagerCollisionResolve(EffectsManager* manager, EffectEnum type)
{
  printf("Effect Collsiion\n");
}