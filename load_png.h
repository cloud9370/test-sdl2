/*
 * load_png.h
 *
 *  Created on: 2015¦~1¤ë28¤é
 *      Author: cloudchen
 */

#ifndef LOAD_PNG_H_
#define LOAD_PNG_H_

#include <SDL.h>

void testLoadPNG(const char *pngFile);
SDL_Surface *SDL_CreateSurfaceFromPNG(const char *filename);


#endif /* LOAD_PNG_H_ */
