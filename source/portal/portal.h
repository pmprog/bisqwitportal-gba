
#pragma once

#include "typedefs.h"

extern portal_entity active_entities[255];
extern u8 active_entity_count;

// Public functions
void portal_initialise(portal_map* Map, float HorzFoV, float VertFoV);
void portal_entity_add(portal_entity* NewEntity);
void portal_update();
void portal_render(portal_entity* Origin, u8 RenderWidth);

