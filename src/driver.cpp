// entry point

#include "graphics/graphics.h"
#include "application_mode.h"
#include "menu/menu.h"

#include <functional>

Application_mode APP_MODE = APP_MODE_NONE;

int main() {
	gp::init();
	
	APP_MODE = APP_MODE_MENU;
	Main_menu main_menu;
	gp::Main_loop::launch([&main_menu] () -> bool { return main_menu.frame(); });
	
	gp::terminate();
}
