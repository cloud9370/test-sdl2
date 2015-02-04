INCLUDE=-ID:/Project/SDL2-2.0.3/include \
	-ID:/Project/SDL2_mixer-2.0.0/i686-w64-mingw32/include/SDL2 \
	-ID:/Project/SDL2_image-2.0.0/i686-w64-mingw32/include/SDL2 \
	-ID:/Project/SDL2_gfx-1.0.1 \
	-ID:/Project/SDL2_ttf-2.0.12/i686-w64-mingw32/include/SDL2 \
	-ID:/MinGW/msys/1.0/home/cloudchen/libpng-1.6.16 \
	-ID:/MinGW/msys/1.0/home/cloudchen/zlib-1.2.8 \
	-I"C:/Program Files (x86)/OpenAL 1.1 SDK/include"

LIBS=-lmingw32 -lwsock32 \
	-LD:/Project/SDL2-2.0.3/i686-w64-mingw32/lib -lSDL2main -lSDL2.dll \
	-LD:/Project/SDL2_mixer-2.0.0/i686-w64-mingw32/lib -lSDL2_mixer.dll \
	-LD:/Project/SDL2_image-2.0.0/i686-w64-mingw32/lib -lSDL2_image.dll \
	-LD:/Project/SDL2_gfx-1.0.1 -lSDL2_gfx.dll \
	-LD:/Project/SDL2_ttf-2.0.12/i686-w64-mingw32/lib -lSDL2_ttf.dll \
	-LD:/MinGW/msys/1.0/home/cloudchen/libpng-1.6.16/.libs -lpng16 \
	-LD:/MinGW/msys/1.0/home/cloudchen/zlib-1.2.8 -lz \
	"C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win32/OpenAL32.lib"

CC=gcc
CFLAGS=-O0 -g3 -D_DEBUG -DTW_ONLY -I../LzoPack $(INCLUDE)

CXX=g++
CXXFLAGS=$(CFLAGS)

LDFLAGS=
LDLIBS=$(LIBS)

OBJS=test_sdl2.o load_png.o PNGLoader.o

PROGRAM=crossgate.exe

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
