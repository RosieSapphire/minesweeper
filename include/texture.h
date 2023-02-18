#ifndef TEXTURE_H
#define TEXTURE_H

#define GLuint unsigned int

GLuint texture_load(const char *path);
void textures_unload(int num, GLuint *start);

#undef GLuint

#endif
