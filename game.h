#pragma once

#include "screen.h"
#include "spatial_index.h"
#include "vararray.h"
#include "ball_object.h"
#include "paddle_object.h"

typedef struct game_data_t game_data_t;

// Setup the gamedata structure
game_data_t * game_init();

// destroy game
void game_destroy(game_data_t * game);

