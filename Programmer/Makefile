ODIR=obj
SDIR=src
BDIR=bin

CC=g++
FLAGS=-std=c++11 -I imgui -I imgui/examples -I Serial-Interface/src -I imgui_club/imgui_memory_editor
LIB=-L Serial-Interface -l serialcomm -lm -l glfw -l GLEW

ifdef MAC
LIB+=-framework OpenGL
else
LIB+=-l GL
endif

EXECOUT=$(BDIR)/eeprom

ifdef DEBUG
FLAGS+=-g -O0
endif

ifdef RELEASE
FLAGS+=-O2
endif

OBJS=programmer.cpp
_OBJS=$(patsubst %.cpp, $(ODIR)/%.o, $(OBJS))

HEADERS=$(SDIR)/programmer.h $(SDIR)/interface.h

IMGUI_SRC=imgui.cpp imgui_draw.cpp imgui_widgets.cpp examples/imgui_impl_glfw.cpp examples/imgui_impl_opengl3.cpp
IMGUI_OBJ=$(patsubst %.cpp, $(ODIR)/%.o, $(patsubst examples/%, %, $(IMGUI_SRC)))

IMGUI_FLAGS=-I imgui -I imgui/examples -D IMGUI_IMPL_OPENGL_LOADER_GLEW

.PHONY: imgui clean

programmer: makedir $(_OBJS)
	test -s $(ODIR)/imgui.o || make imgui
	$(CC) $(IMGUI_OBJ) $(_OBJS) $(LIB) -o $(EXECOUT)

imgui:
	$(CC) -c $(IMGUI_FLAGS) $(patsubst %.cpp, imgui/%.cpp, $(IMGUI_SRC))
	mv *.o $(ODIR)

makedir:
	mkdir -p $(ODIR)
	mkdir -p $(BDIR)

$(ODIR)/%.o: $(SDIR)/%.cpp $(HEADERS)
	$(CC) -c $(FLAGS) -o $@ $<

clean:
	rm -f $(EXECOUT) $(ODIR)/*.o
