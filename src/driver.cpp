// entry point

#include "graphics/graphics.h"
#include "application_mode.h"
#include "menu/menu.h"
#include "AI/tt.h"

#include <functional>

Application_mode APP_MODE = APP_MODE_NONE;

int main(int argc, char** argv) {
	gp::init();
	
	// resize transposition table
	// default is 1 GB
	// can be resized by executing -> $ ./connect4 tt=<size in megabytes>
	size_t megabytes_for_TT = 1024;
	for (int i = 1; i < argc; i++) {
		std::string s = argv[i];
		if (s.size() > (size_t) 3 && s.substr(0, 3) == "tt=") {
			megabytes_for_TT = std::stoull(s.substr(3));
		}
	}
	TT.resize(megabytes_for_TT);
	
	// launch menu
	APP_MODE = APP_MODE_MENU;
	Main_menu main_menu;
	gp::Main_loop::launch([&main_menu] () -> bool { return main_menu.frame(); });
	
	gp::terminate();
}
