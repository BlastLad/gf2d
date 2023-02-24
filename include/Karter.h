#ifndef __KARTER_H__
#define __KARTER_H__

#include "Entity.h"

/**
 * @brief spawn a new normal student at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* karter_new(Vector2D position, int gridPositionX, int gridPositionY, int targetGridX, int targetGridY);

#endif
