#ifndef SHADER_H
#define SHADER_H

#define GLuint unsigned int

GLuint shader_load(const char *v_path, const char *f_path);
void shader_unload(GLuint shader);

#undef GLuint

#endif
