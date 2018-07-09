#pragma once

#define SHOULD_DISPLAY_TEXT 0
#define SHOULD_DISPLAY_COSTS 0

#define RANDOM_MAZE_SIZE 30

/**
 Must be int
 > 1 will cause only the Nth simulation step to be drawn    => faster
 < 0 will pause for -N milliseconds                        => slower
 */
#define GRAPHIC_ACCELERATION 3

/**
1 - Manhatten Distance
2 - Euclidean Distance
3 - Scaled Euclidean Distance (2*Euclidean)
default - 0 Distance
 */
#define MAZE_HEURISTIC 1
