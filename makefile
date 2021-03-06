# makefile for building main program
# creates 'connect4' executable

# main target
all: release
	find . -name *.o -delete

# helper target for building/exporting dependencies
deps:
	cd vendor && make

# c++ flags
CXX_FLAGS = -Wshadow -Wall -std=c++17
CXX_RELEASE_FLAGS = -O2
CXX_DEBUG_FLAGS = -g -fsanitize=address -fsanitize=undefined -D_GLIBCXX_DEBUG

# collect files in lists
SRC_FILES =                      \
$(wildcard ./src/*.cpp)          \
$(wildcard ./src/graphics/*.cpp) \
$(wildcard ./src/menu/*.cpp)     \
$(wildcard ./src/game/*.cpp)     \
$(wildcard ./src/util/*.cpp)     \
$(wildcard ./src/AI/*.cpp)

HDR_FILES =                      \
$(wildcard ./src/*.h)            \
$(wildcard ./src/*.hpp)          \
$(wildcard ./src/graphics/*.h)   \
$(wildcard ./src/graphics/*.hpp) \
$(wildcard ./src/menu/*.h)       \
$(wildcard ./src/menu/*.hpp)     \
$(wildcard ./src/game/*.h)       \
$(wildcard ./src/game/*.hpp)     \
$(wildcard ./src/util/*.h)       \
$(wildcard ./src/util/*.hpp)     \
$(wildcard ./src/AI/*.h)         \
$(wildcard ./src/AI/*.hpp)

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
	g++ $(CXX_FLAGS) $< -c -o $@ $(CXX_RELEASE_FLAGS) -I ./vendor/bin/

# debug target
debug: ${DBG_OBJ_FILES}
	g++ $(CXX_FLAGS) $^ -o connect4 $(CXX_DEBUG_FLAGS) $(LINKS)

%_debug.o: %.cpp ${HDR_FILES}
	g++ $(CXX_FLAGS) $< -c -o $@ $(CXX_DEBUG_FLAGS) -I ./vendor/bin/

# clean target for command line access
.PHONY: clean
clean:
	find . -name *.o -delete
