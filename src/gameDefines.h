#ifndef _GAME_DEFINES_H
#define _GAME_DEFINES_H

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
	#define APP_SCREEN_WIDTH 800
	#define APP_SCREEN_HEIGHT 480
#endif

#define WARNING_STRING "WARNING:"
#define ERROR_STRING "ERROR:"
#define CRITICAL_STRING "CRITICAL ERROR:"
#define ERROR_CODE -67

#define GAME_FONT "fonts/lato.ttf"

#endif // _GAME_DEFINES_H

