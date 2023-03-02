
#pragma once

#include "platform.h"
#include "maths.h"

typedef struct SpriteConfiguration
{
    u8          Plane;          // Display on which plane
    vec2        Position;       // Position relative to the MapEx plane, not the display
    u8          Visible;        // Keeps the slot allocated, but don't display
    ColourDepth BPP;            // Colour depth
    u8          PaletteIndex;   // Palette for 4BPP images
    SpriteSize  Size;           // Sprite Dimensions
    u16         GraphicTile;    // Index of graphics tile
} SpriteConfiguration;

// Removes all sprites from list
void mapex_sprites_clear();

// Adds a new sprite, returns an Index for
u16 mapex_sprites_add(u16 Tile, ColourDepth BPP, u8 PaletteIndex, SpriteSize Size);

// Removes a sprite
void mapex_sprites_remove(u16 Index);

// Reposition a sprite within the world
void mapex_sprites_position(u16 Index, u8 Plane, u8 Visible, s16 X, s16 Y);

// Animate a sprite
void mapex_sprites_animate(u16 Index, u16 GraphicTile, u8 PaletteIndex, Mirroring Flip);

// Get all the information about a sprite
SpriteConfiguration* mapex_sprites_get(u16 Index);
