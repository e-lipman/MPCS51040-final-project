#pragma once

#include<math.h>
#include"game_object.h"
#include"ball_object.h"

typedef struct brick_data_t brick_data_t;

game_object_t * brick_create(game_data_t *  data, rectangle_t * r);
