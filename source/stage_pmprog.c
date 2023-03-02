
#include "stage_pmprog.h"
#include "stage_bitmaptest.h"
#include "stage_portal.h"
#include "../resources/sprites_pmprog.h"

/*
float easeoutback(float Time, float Begin, float Change, float Duration, float Swing);

float easeoutback(float Time, float Begin, float Change, float Duration, float Swing)
{
	float time = Time;
	if (Swing == 0)
	{
		Swing = 1.70158f;
	}
	return Change * ((time = time / Duration - 1) * time * ((Swing + 1) * time + Swing) + 1) + Begin;
}
*/

void pmprog_start()
{
    audio_music_play(0);

	graphics_sprites_characterset_set((u8*)sprites_pmprog, 0, sprites_pmprog_tile_count, COLOURS_8BPP);

	graphics_sprites_configure(0, 0, COLOURS_8BPP, 0, SIZE_64x64);
	graphics_sprites_position(0, 1, 1, 88, 160, MIRROR_NONE);

}

void pmprog_finish()
{
    audio_music_stop();
	graphics_sprites_clear();
}

void pmprog_update()
{
	if(engine_stage_time >= 160)
	{
		if( input_button_pressed( BUTTON_A ) )
		{
			engine_changestage((StageRecord*)&bitmaptest_stage);
		}
		if( input_button_pressed( BUTTON_B ) )
		{
			engine_changestage((StageRecord*)&game_stage);
		}
		//engine_changestage((StageRecord*)&mainmenu_stage);
		return;
	}

	s32 starty = 160;
	s32 endy = 48;
	if(engine_stage_time < 60)
	{
		u16 cury = (u16)easeoutback((float)engine_stage_time, (float)starty, (float)(endy - starty), 60.0f, 2.0f);
		graphics_sprites_position(0, 1, 1, 88, cury, MIRROR_NONE);
	}
	else if (engine_stage_time >= 100)
	{
		u16 cury = (u16)easeoutback((float)engine_stage_time - 100, (float)endy, (float)(starty - endy + 20), 70.0f, 1.0f);
		graphics_sprites_position(0, 1, 1, 88, cury, MIRROR_NONE);
	}
}
