VPATH = src:include
PLATFORM = 'desktop'
BUILD = 'debug'

DEV = D:\\_Work\\_Dev_Libs

IDIRS = include $(DEV)\\glad-es\\include  $(DEV)\\glm $(DEV)\\stb

ifeq ($(PLATFORM), 'desktop')
	IDIRS += $(DEV)\\glfw-3.2.bin.WIN64\\include
else
	IDIRS += $(DEV)\\emsdk\\upstream\\emscripten\\cache\\sysroot\\include
endif

INCLUDES = $(patsubst %,-I%,$(IDIRS))

LDIRS = $(DEV)\\glfw-3.2.bin.WIN64\\lib-mingw-w64 $(DEV)\\w64devkit\\x86_64-w64-mingw32\\lib
LIBRARIES = $(patsubst %,-L%,$(LDIRS))
LINKS = -lglfw3 -lgdi32 -luser32 -lkernel32 

ifeq ($(PLATFORM), 'desktop')
	LINKS += -o program.exe
else
	LINKS += -s USE_GLFW=3 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 --preload-file Assets/ \
			-o index.html --shell-file shell.html -sEXPORTED_FUNCTIONS=_main,_AddLevel
endif

ifneq ($(PLATFORM), 'desktop')
	ifneq ($(BUILD), 'debug')
		LINKS += -03 --profiling-funcs
	endif
endif

ifeq ($(PLATFORM), 'desktop')
	OBJDIR = objs
else
	OBJDIR = objs_em
endif
OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(wildcard src/*.cpp)) $(OBJDIR)/glad.o

DEBUG = -g

ifeq ($(BUILD), 'debug')
	CXXFLAGS = $(INCLUDES) $(DEBUG)
else
	ifeq ($(PLATFORM), 'desktop')
		CXXFLAGS = $(INCLUDES)
	else
		CXXFLAGS = $(INCLUDES) --profiling
	endif
endif

ifeq ($(PLATFORM), 'desktop')
	CC = g++
else
	CC = em++
endif

program : $(OBJS)
	$(CC) $(OBJS) $(LIBRARIES) $(LINKS)

$(OBJDIR)/glad.o : $(DEV)\\glad-es\\src\\glad.c
	$(CC) -o $@ -c $(DEV)\\glad-es\\src\\glad.c $(CXXFLAGS)

$(OBJDIR)/main.o : main.cpp game.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)

$(OBJDIR)/%.o : %.cpp %.hpp
	$(CC) -o $@ -c $< $(CXXFLAGS)
		
cleanrun :
	$(MAKE) clean 
	$(MAKE)

clean :
	rm $(OBJDIR)/*.o