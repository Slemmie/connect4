// application mode

#pragma once

enum Application_mode {
	
	APP_MODE_NONE = 0, // null
	APP_MODE_MENU = 1, // menu screen, select gamemode, etc.
	APP_MODE_GAME = 2, // actual game
	APP_MODE_OVER = 3  // game is won/lost/drawn
	
} APP_MODE = APP_MODE_NONE;
