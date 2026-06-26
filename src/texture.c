#include "texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

void textures_unload(int num, GLuint *start)
{
        glDeleteTextures(num, start);
}
