INCLUDE=

LIBS=

CC=emcc
CFLAGS=-O0 -g3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s LEGACY_GL_EMULATION=1 $(INCLUDE)

CXX=em++
CXXFLAGS=$(CFLAGS)

LDFLAGS=
LDLIBS=$(LIBS)

OBJS=test_sdl2.o

PROGRAM=crossgate.html

.PHONY: all clean html

all: $(PROGRAM)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@



$(PROGRAM): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)


clean:
	rm -f $(PROGRAM)
	rm -f $(OBJS)
