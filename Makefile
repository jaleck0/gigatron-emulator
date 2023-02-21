# A simple Makefile for compiling small SDL projects

# set the compiler
CC := clang

# add header files here
HDRS := $(*.h)

# add source files here
SRCS := emulator.c input.c sdldraw.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := emulate    

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lSDL2_ttf -lm

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
