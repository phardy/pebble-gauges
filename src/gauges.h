#pragma once

#include "pebble_os.h"

#define DIAL_RADIUS 70
#define HAND_LENGTH 70
#define INNER_RADIUS1 68
#define INNER_RADIUS2 63

#define NUM_HOUR_TICKS 9
#define NUM_MINUTE_TICKS 7

static const struct GPathInfo BIG_TICK = {
  4, (GPoint []) {
    {-3, -INNER_RADIUS1},
    {3, -INNER_RADIUS1},
    {3, -INNER_RADIUS2+7},
    {-3, -INNER_RADIUS2+7}
  }
};
