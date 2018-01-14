#include "../headers/textures.h"

GLuint texture_names[NUMBER_OF_TEXTURES];

static void create_texture(Image* image, GLuint* texture_names, const char* file_name, int texture_index);

void load_textures()
{
    glEnable(GL_TEXTURE_2D);

	/* enable light with textures */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	Image* image = image_init(0, 0);

	/* generate texture names */
    glGenTextures(NUMBER_OF_TEXTURES, texture_names);

	create_texture(image, texture_names, FILENAME0, 0);
	create_texture(image, texture_names, FILENAME1, 1);

	/* default texture */
    glBindTexture(GL_TEXTURE_2D, 0);

	/* destroy object for drawing textures from the file */
    image_done(image);
}

static void create_texture(Image* image, GLuint* texture_names, const char* file_name, int texture_index)
{
    image_read(image, file_name);

    glBindTexture(GL_TEXTURE_2D, texture_names[texture_index]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
}

void free_textures()
{
	glDeleteTextures(2, texture_names);
}
