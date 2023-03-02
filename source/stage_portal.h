
#pragma once

#include "../engine/engine.h"
#include "portal/portal.h"

void game_start();
void game_finish();
void game_update();

static const StageRecord game_stage = { game_start, game_update, game_finish };
