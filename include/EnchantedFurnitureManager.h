#ifndef  __ENCHANTED_FURNITURE_MANAGER_H__
#define __ENCHANTED_FURNITURE_MANAGER_H__

#include "gfc_shape.h"

typedef struct {


	List* furnitureBodies;

}EnchantedFurnitureManager;


int furniture_body_clip(EnchantedFurnitureManager* efm, Shape piperStar);

#endif
