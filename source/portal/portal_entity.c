
#include "portal_entity.h"

void portal_processentities()
{
    for(u8 e = 0; e < active_entity_count; ++e)
    {
        // Run Updates/Movement

        // Flagged for removal, remove
        if( (active_entities[e].entity_flags & ENTITY_REMOVE) == ENTITY_REMOVE )
        {
            if(e == active_entity_count - 1)
            {
                active_entity_count--;
                break;
            }
            else
            {
                active_entities[e] = active_entities[active_entity_count - 1];
                e--;
            }
        }
    }
}
