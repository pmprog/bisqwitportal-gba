
#pragma once

#include "../../engine/engine.h"

typedef enum SectorFlags
{
    SECTOR_NONE             = 0x0000,
    SECTOR_NOCEILING        = 0x0001,   // No ceiling is drawn (skybox?)
    SECTOR_TEXTURED         = 0x0002,   // Walls are textured
    SECTOR_WARP             = 0x0004,   // Warp on entering
} SectorFlags;

typedef enum EntityFlags
{
    ENTITY_NONE             = 0x0000,
    ENTITY_REMOVE           = 0x0001,   // This entity has been destroyed, remove from list
    ENTITY_SLEEP            = 0x0002,   // Put entity to sleep, so nothing applies to it
} EntityFlags;

typedef vec2 portal_vec2;
typedef vec3 portal_vec3;

typedef struct portal_entity
{
    u32 entity_type;
    EntityFlags entity_flags;           // Flags
    void* entity_data;                  // Entity specific data
    portal_vec3 position;               // Position in X/Y/Z
    u16 position_sector;                // Sector entity is in
    portal_vec3 velocity;               // Movement velocity
    u16 rotation;                       // Direction facing
//    FIXED rotation_sin;                 // Cached rotation data, maybe scratch this as it's a LUT
//    FIXED rotation_cos;                 // Cached rotation data, maybe scratch this as it's a LUT
} portal_entity;

typedef struct portal_sector
{
    SectorFlags flags;                  // Flags for the sector
    float floor;                        // World height of floor
    float ceiling;                      // World height of ceiling (required even if SECTOR_NOCEILING set)
    u8 floor_texture_id;                // Floor texture/colour (based on SECTOR_TEXTURED)
    u8 ceiling_texture_id;              // Ceiling texture/colour (based on SECTOR_TEXTURED)
    u16 warp_target_sector;             // On entering sector, transport to different sector (SECTOR_WARP)
    u8 vertices;                        // Vertex count
    portal_vec2* vertex_list;           // Array of (vertices) portal_vec2 elements
    u16* edge_sector_list;              // Array of (vertices) elements which contain the sector id of the sector each edge (vertex n to n+1) connects to
    u8* edge_texture_list;              // Array of (vertices) elements which contain the texture id or palette index of the wall
} portal_sector;

typedef struct portal_map
{
    u16 sector_count;                   // Number of sectors in map
    portal_sector* sector_list;         // Sector array
    u8 entity_count;                    // Number of (inital) entities in the map
    portal_entity* entity_list;         // Entity array
} portal_map;

typedef struct portal_renderitem
{
    u16 sector;
    u16 sx1;
    u16 sx2;
} portal_renderitem;
