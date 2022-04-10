// entry point

#include "graphics/graphics.h"

int main() {
	gp::init();
	
	gp::Main_loop::launch([] () -> bool { return true; });
	
	gp::terminate();
}
