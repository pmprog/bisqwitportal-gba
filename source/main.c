
#include "../engine/engine.h"
#include "../resources/jamfox_gba.h"
#include "stage_pmprog.h"
#include "stage_portal.h"

int main(void)
{
	platform_init();

	graphics_palette_set( (Colour*)jamfox_gba, 0, jamfox_gba_len);
	graphics_palette_set( (Colour*)&jamfox_gba[1], 0, 1);	// Force black override
	graphics_mode_tilemap( 3, COLOURS_8BPP );	// Single plane
	graphics_sprites_clear();

	// Use code as a test character set
	//graphics_tilemap_characterset_set((u8*)luma_font, 0, 493, COLOURS_8BPP);
	//graphics_sprites_characterset_set((u8*)luma_font, 0, 493, COLOURS_8BPP);

    random_init( 44 );

	// Execute
	engine_execute((StageRecord *)&pmprog_stage);
    //engine_execute((StageRecord *)&game_stage);

	audio_music_stop();
	platform_shutdown();
}
