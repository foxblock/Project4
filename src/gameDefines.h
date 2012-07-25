#ifndef _GAME_DEFINES_H
#define _GAME_DEFINES_H

#define APP_RESIZE 0
#ifdef GP2X
	#define APP_SCREEN_WIDTH 320
	#define APP_SCREEN_HEIGHT 240
	#define APP_FULLSCREEN 1
#elif defined PANDORA
	#define APP_SCREEN_WIDTH 800
	#define APP_SCREEN_HEIGHT 480
	#define APP_FULLSCREEN 1
#else
	#define APP_SCREEN_WIDTH 800
	#define APP_SCREEN_HEIGHT 480
	#define APP_FULLSCREEN 0
#endif

#define WARNING_STRING "WARNING:"
#define ERROR_STRING "ERROR:"
#define CRITICAL_STRING "CRITICAL ERROR:"
#define ERROR_CODE -67

#define FONT_GENERAL "fonts/lato.ttf"
#define FILE_HIGHSCORE_NORMAL "data/scores_normal.txt"

#define VERSION_STRING "0.0.1.1"

#endif // _GAME_DEFINES_H

