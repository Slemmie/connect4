# makefile for building and/or exporting vendor modules
# exports to ./bin/...

## main target
all: glfw glew glm
	git submodule deinit -f .
	git submodule update --init .

# creates glfw shared library and exports glfw header
.PHONY: glfw
glfw:
	git submodule deinit -f ./glfw/
	git submodule update --init ./glfw/
	@if [ ! -d "./glfw/" ]; then         \
		echo "! glfw submodule missing"; \
		false;                           \
	fi;
	if [ ! -d "./bin/glfw/" ]; then mkdir -p ./bin/glfw/; fi;
	mkdir ./glfw/build/ && cmake -S ./glfw/ -B ./glfw/build/ -DBUILD_SHARED_LIBS=ON
	cd ./glfw/build/ && make glfw
	cp ./glfw/build/src/libglfw.so.3 ./bin/glfw/libglfw.so.3
	cp ./glfw/include/GLFW/glfw3.h ./bin/glfw/glfw3.h

# creates glew shared library and exports header
.PHONY: glew
glew:
	git submodule deinit -f ./glew/
	git submodule update --init ./glew/
	@if [ ! -d "./glew/" ]; then         \
		echo "! glew submodule missing"; \
		false;                           \
	fi;
	if [ ! -d "./bin/glew/" ]; then mkdir -p ./bin/glew/; fi;
	cd ./glew/auto/ && make
	cd ./glew/ && make
	cp ./glew/lib/libGLEW.so.2.2 ./bin/glew/libGLEW.so.2.2
	cp ./glew/include/GL/glew.h ./bin/glew/glew.h

# exports glm headers
.PHONY: glm
glm:
	git submodule deinit -f ./glm/
	git submodule update --init ./glm/
	@if [ ! -d "./glm/" ]; then         \
		echo "! glm submodule missing"; \
		false;                          \
	fi;
	if [ ! -d "./bin/glm/include/" ]; then mkdir -p ./bin/glm/; fi;
	cp -r ./glm/glm/* ./bin/glm/
