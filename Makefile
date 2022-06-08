VPATH = src:include

DEV = D:\\_Work\\_Dev_Libs

IDIRS = include $(DEV)\\glad\\include $(DEV)\\glfw-3.3.6.bin.WIN64\\include $(DEV)\\glm $(DEV)\\stb
INCLUDES = $(patsubst %,-I%,$(IDIRS)) -g

LDIRS = $(DEV)\\glfw-3.3.6.bin.WIN64\\lib-mingw-w64 $(DEV)\\w64devkit\\x86_64-w64-mingw32\\lib
LIBRARIES = $(patsubst %,-L%,$(LDIRS)) 
LINKS = -lglfw3 -lgdi32 -luser32 -lkernel32

OBJS = $(patsubst src/%.cpp,objs/%.o,$(wildcard src/*.cpp)) objs/glad.o
DEBUG = 


program : $(OBJS)
	g++ -o program $(OBJS) $(LIBRARIES) $(LINKS) 

objs/main.o : main.cpp game.hpp
	g++ -o $@ -c $< $(INCLUDES)

objs/glad.o : $(DEV)\\glad\\src\\glad.c
	g++ -o $@ -c $(DEV)\\glad\\src\\glad.c $(INCLUDES)

objs/game.o : game.cpp game.hpp
	g++ -o $@ -c $< $(INCLUDES)

objs/world.o : world.cpp world.hpp 
	g++ -o $@ -c $< $(INCLUDES)

objs/renderer.o : renderer.cpp renderer.hpp
	g++ -o $@ -c $< $(INCLUDES)

objs/data.o : data.cpp data.hpp
	g++ -o $@ -c $< $(INCLUDES)

cleanrun :
	$(MAKE) clean 
	$(MAKE)

clean :
	rm objs/*.o