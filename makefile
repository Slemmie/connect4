# makefile for building main program
# creates 'connect4' executable

# main target
all: release
	find . -name *.o -delete

# c++ flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2
CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

# collect files in lists
SRC_FILES =             \
$(wildcard ./src/*.cpp)

HDR_FILES =           \
$(wildcard ./src/*.h)

OBJ_FILES = ${SRC_FILES:.cpp=.o}
DBG_OBJ_FILES = ${SRC_FILES:.cpp=_debug.o}

# links to libraries (glfw, glew, GL, pthread)
LINKS = \
-L ./vendor/bin/glfw/ -Wl,-rpath=./vendor/bin/glfw/ ./vendor/bin/glfw/libglfw.so.3   \
-L ./vendor/bin/glew/ -Wl,-rpath=./vendor/bin/glew/ ./vendor/bin/glew/libGLEW.so.2.2 \
-lGL -lpthread

# release target
release: ${OBJ_FILES}
	g++ $(CXX_FLAGS) $^ -o connect4 $(CXX_RELEASE_FLAGS) $(LINKS)

%.o: %.cpp ${HDR_FILES}
	g++ $(CXX_FLAGS) $< -o $@ $(CXX_RELEASE_FLAGS) -I ./vendor/bin/

# debug target
debug: ${DBG_OBJ_FILES}
	g++ $(CXX_FLAGS) $^ -o connect4 $(CXX_DEBUG_FLAGS) $(LINKS)

%_debug.o: %.cpp ${HDR_FILES}
	g++ $(CXX_FLAGS) $< -o $@ $(CXX_DEBUG_FLAGS) -I ./vendor/bin/
