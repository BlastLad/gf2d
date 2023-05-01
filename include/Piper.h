#ifndef __PIPER_H__
#define __PIPER_H__

#include "gfc_vector.h"
#include "Entity.h"


Entity *piper_entity_new(Vector2D spawnPosition);


typedef struct
{
	int sleepUpgrade;
	int mixingUpgrade;
	int currentHealth;
	int maxHealth;
	int currentHealthFragments;
	int currentSpellBooks;
	int maxSpellBooks;
	float timeSpedUp;
	int shadowClone;
	int mixAbility;
	int emeraldAbility;
	int currency;
	int oddsOnly;
	int powerUpCollected;

}PiperData;


PiperData* GetPiperData();



#endif // !__PIPER_H__

