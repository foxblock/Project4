#ifndef _GAME_DEFINES_H
#define _GAME_DEFINES_H

#define SPARROW_PATH "../sparrow3d/sparrow3d.h"

#define APP_RESIZE 0
#define APP_FULLSCREEN 0
#ifdef GP2X
#define APP_SCREEN_WIDTH 320
#define APP_SCREEN_HEIGHT 240
#elif defined PANDORA
#define APP_SCREEN_WIDTH 800
#define APP_SCREEN_HEIGHT 480
#define APP_FULLSCREEN 1
#else
#define APP_SCREEN_WIDTH 600
#define APP_SCREEN_HEIGHT 600
#endif

#define WARNING_STRING "WARNING:"
#define ERROR_STRING "ERROR:"
#define CRITICAL_STRING "CRITICAL ERROR:"
#define ERROR_CODE -67

#define STARTING_STATE StateLevel
#define PLAYER_CLASS UnitSpike

#endif // _GAME_DEFINES_H

