#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <GL/glut.h>
#include "image.h"

#define NUMBER_OF_TEXTURES 2
#define FILENAME0 "textures/wall.bmp"
#define FILENAME1 "textures/floor.bmp"

/* texture id */
extern GLuint texture_names[NUMBER_OF_TEXTURES];

void load_textures();

#endif
