// entry point

#include "graphics/graphics.h"
#include "application_mode.h"
#include "menu/menu.h"

#include <functional>

Application_mode APP_MODE = APP_MODE_NONE;

int main() {
	gp::init();
	
	APP_MODE = APP_MODE_MENU;
	gp::Main_loop::launch(std::bind(&Main_menu::frame, Main_menu()));
	
	gp::terminate();
}
