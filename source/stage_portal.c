
#include "stage_portal.h"

portal_vec2 game_testmap_sector1_vertices[] = {
      { ( 10), (-10) },
      { ( 20), (-10) },
      { ( 20), ( 10) },
      { ( 10), ( 10) },
};
u16 game_testmap_sector1_edgesectors[] = {
      0xFFFF,
      0xFFFF,
      0xFFFF,
      0,
};
u8 game_testmap_sector1_edgepalette[] = {
      6,
      7,
      8,
      9,
};


portal_vec2 game_testmap_sector0_vertices[] = {
      { (-10), (-10) },
      { ( 10), (-10) },
      { ( 10), ( 10) },
      { (-10), ( 10) },
};
u16 game_testmap_sector0_edgesectors[] = {
      0xFFFF,
      1,
      0xFFFF,
      0xFFFF,
};
u8 game_testmap_sector0_edgepalette[] = {
      6,
      7,
      8,
      9,
};
portal_sector game_testmap_sectors[] = {
      { SECTOR_NONE, (0), (10), 2, 3, 0, 4, game_testmap_sector0_vertices, game_testmap_sector0_edgesectors, game_testmap_sector0_edgepalette },
      { SECTOR_NONE, (1), (8), 2, 3, 0, 4, game_testmap_sector1_vertices, game_testmap_sector1_edgesectors, game_testmap_sector1_edgepalette },
};

portal_entity game_testmap_entities[] = {
      // Player
      { 0, ENTITY_NONE, NULL, { (0), (0), (3) }, 0, { (0), (0), (0) }, 0 },
};

// Map
portal_map game_testmap = { 2, &game_testmap_sectors, 1, game_testmap_entities };


void game_start()
{
    graphics_mode_bitmap( 0 );
#ifdef LIBGBA
    // Half the horizontal resolution to make faster
    REG_BG2PA = 0x80;
    portal_initialise( &game_testmap, ( 0.38f * 120.0f ), ( 80 ) );
#else
    portal_initialise( &game_testmap, ( 0.73f * 240.0f ), ( 80 ) );
#endif

}

void game_finish()
{

}

void game_update()
{

    portal_update();

#ifdef LIBGBA
    portal_render(&active_entities[0], 120);
#else
    portal_render(&active_entities[0], 240);
#endif

    if (input_button_held(DPAD_Left))
    {
        active_entities[0].rotation = (active_entities[0].rotation + 355) % 360;
    }
    if (input_button_held(DPAD_Right))
    {
        active_entities[0].rotation = (active_entities[0].rotation + 5) % 360;
    }
    if (input_button_held(DPAD_Up))
    {
        active_entities[0].position.x += fixed_to_float(cosi(active_entities[0].rotation));
        active_entities[0].position.y += fixed_to_float(sini(active_entities[0].rotation));
    }
    if (input_button_held(DPAD_Down))
    {
        active_entities[0].position.x -= fixed_to_float(cosi(active_entities[0].rotation));
        active_entities[0].position.y -= fixed_to_float(sini(active_entities[0].rotation));
    }

}

