
#include "gba.h"

void graphics_tilemap_characterset_set(u8* CharacterSetSource, u16 FirstCharacter, u16 NumberOfCharacters, ColourDepth BPP)
{
    switch(BPP)
    {
        case COLOURS_4BPP:
            memcpy32((u16*)(MEM_VRAM + (FirstCharacter * 20)), (u16*)CharacterSetSource, (NumberOfCharacters * 5));
            break;
        case COLOURS_8BPP:
            memcpy32((u16*)(MEM_VRAM + (FirstCharacter * 64)), (u16*)CharacterSetSource, (NumberOfCharacters * 16));
            break;
        default:
            break;
    }
}

static u16* graphics_tilemap_characterset_getptr(u16 Character, ColourDepth BPP)
{
    switch(BPP)
    {
        case COLOURS_4BPP:
            return (u16*)(MEM_VRAM + (Character * 20));
            break;
        case COLOURS_8BPP:
            return (u16*)(MEM_VRAM + (Character * 64));
            break;
        default:
            break;
    }
    return NULL;
}

u8 graphics_tilemap_characterset_getpixel(u16 Character, ColourDepth BPP, s16 X, s16 Y)
{
    if( X < 0 || X >= 8 || Y < 0 || Y >= 8)
    {
        return 0;
    }
    u8 targetindex = ((Y * 8) + X);
    u16* targetchar = graphics_tilemap_characterset_getptr(Character, BPP)[targetindex];
    if( X & 1 )
    {
        return (*targetchar & 0xFF00) >> 8;  // Right pixel
    }
    else
    {
        return (*targetchar & 0x00FF);  // Left pixel
    }
    return 0;
}

void graphics_tilemap_characterset_setpixel(u16 Character, ColourDepth BPP, s16 X, s16 Y, u8 PaletteIndex)
{
    if( X < 0 || X >= 8 || Y < 0 || Y >= 8)
    {
        return;
    }
    u8 targetindex = ((Y * 8) + X) >> 1;
    u16* targetchar = &graphics_tilemap_characterset_getptr(Character, BPP)[targetindex];
    if( X & 1 )
    {
        *targetchar = (*targetchar & 0x00FF) | (PaletteIndex<<8);    // Right pixel
    }
    else
    {
        *targetchar = (*targetchar & 0xFF00) | PaletteIndex;        // Left pixel
    }
}


void graphics_tilemap_tilemap_settile(u8 Plane, u16 X, u16 Y, u16 Tile)
{
    u16* base = (u16*)MAP_BASE_ADR((Plane * 5) + 20);
    base[(Y * 32) + X] = Tile;
}

u16 graphics_tilemap_tilemap_gettile(u8 Plane, u16 X, u16 Y)
{
    u16* base = (u16*)MAP_BASE_ADR((Plane * 5) + 20) + (Y * 32);
    return base[X];
}

void graphics_tilemap_tilemap_setposition(u8 Plane, u16 X, u16 Y)
{
    switch( Plane )
    {
        case 0:
            REG_BG0HOFS = X;
            REG_BG0VOFS = Y;
            break;
        case 1:
            REG_BG1HOFS = X;
            REG_BG1VOFS = Y;
            break;
        case 2:
            REG_BG2HOFS = X;
            REG_BG2VOFS = Y;
            break;
        case 3:
            REG_BG3HOFS = X;
            REG_BG3VOFS = Y;
            break;
    }
}
