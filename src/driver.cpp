// entry point

#include "graphics/graphics.h"
#include "application_mode.h"

int main() {
	gp::init();
	
	APP_MODE = APP_MODE_MENU;
	gp::Main_loop::launch([] () -> bool { return true; });
	
	gp::terminate();
}
