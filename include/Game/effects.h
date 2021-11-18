#ifndef EFFECTS_H_
#define EFFECTS_H_

#include"Hero/Hero.h"

#include"Game/spritesheet.h"
#include"Game/physics.h"
#include"Game/racket.h"

#include<stdint.h>

#define EFFECTS_CAPACITY 5
#define EFFECTS_SPEED 100.0f

typedef enum
{
  EFFECT_NONE = 0,
  EFFECT_RACKED_SPEEDUP,
  EFFECT_RACKET_SPEEDDOWN,
  EFFECT_BALL_SPEEDUP,
  EFFECT_BALL_SPEEDDOWN
} EffectEnum;

typedef struct
{
  GameSpriteSheet* spriteSheet;

  uint32_t length;
  EffectEnum types[EFFECTS_CAPACITY];
  HeroFloat2 positions[EFFECTS_CAPACITY];
  BoxCollider2D collider;
} EffectsManager;

EffectsManager* effectsManagerCreate();
void effectsManagerUpdate(EffectsManager* manager, double deltaTime, GameRacket* racket);
void effectsManagerDraw(EffectsManager* manager, HeroSpriteBatch* spriteBatch);
void effectsManagerDestroy(EffectsManager* manager);
void effectsManagerAdd(EffectsManager* manager, EffectEnum type, HeroInt2 position);
void effectsManagerCollisionResolve(EffectsManager* manager, EffectEnum type);

#endif