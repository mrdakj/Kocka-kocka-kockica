/* copied from _________________ */

#include "headers/textures.h"

GLuint names[NUMBER_OF_TEXTURES];

void load_textures() {

    glEnable(GL_TEXTURE_2D);

	/* light and texture */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    Image* image = image_init(0, 0);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(NUMBER_OF_TEXTURES, names);

	create_texture(image, names, FILENAME0, 0);
	create_texture(image, names, FILENAME1, 1);


    /* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

static void create_texture(Image* image, GLuint* names, const char* file_name, int texture_index) {

    image_read(image, file_name);


    glBindTexture(GL_TEXTURE_2D, names[texture_index]);
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

