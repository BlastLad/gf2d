#ifndef __UPGRADES_H__
#define __UPGRADES_H__

#include "Entity.h"

Entity* Health_Pot_New(Vector2D position, Vector2D gridPosition);
void SetTagsUpgrade(Entity* self);
Entity* Broom_New(Vector2D position, Vector2D gridPosition);
Entity* Dual_Cast_Upgrade_New(Vector2D position, Vector2D gridPosition);
Entity* Max_Spellbook_New(Vector2D position, Vector2D gridPosition);
Entity* Spellbook_New(Vector2D position, Vector2D gridPosition);
Entity* Mix_Upgrade_New(Vector2D position, Vector2D gridPosition);
Entity* Mix_Base_New(Vector2D position, Vector2D gridPosition);
Entity* Shadow_Clone_PickUp_New(Vector2D position, Vector2D gridPosition);

#endif
