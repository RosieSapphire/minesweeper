#include <stdio.h>
#include <malloc.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

static GLuint shader_compile_individual(const char *path, int type)
{
	FILE *file = fopen(path, "r");
	long size = 0;
	char *buffer = NULL;
	GLuint shader;

	if(!file) {
		printf("Failed to load file from '%s'\n", path);

		return 0;
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	buffer = calloc(size, sizeof(char));
	fread(buffer, sizeof(char), size, file);
	buffer[size - 1] = 0;
	fclose(file);

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char **)&buffer, NULL);
	glCompileShader(shader);

	int shader_status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_status);

	if(!shader_status) {
		char log[512];

		glGetShaderInfoLog(shader, 512, NULL, log);
		printf("Failed to create %s Shader: '%s'\n",
				(type == GL_VERTEX_SHADER) ?
				"Vertex" : "Fragment", log);

		return 0;
	}

	free(buffer);

	return shader;
}

GLuint shader_load(const char *v_path, const char *f_path)
{
	GLuint program = glCreateProgram();
	GLuint vert = shader_compile_individual(v_path, GL_VERTEX_SHADER);
	GLuint frag = shader_compile_individual(f_path, GL_FRAGMENT_SHADER);

	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	int program_status;

	glGetProgramiv(program, GL_LINK_STATUS, &program_status);

	if(!program_status) {
		char log[512];

		glGetProgramInfoLog(program, 512, NULL, log);
		printf("Failed to link shader program: '%s'\n", log);

		return 0;
	}

	glDeleteShader(frag);
	glDeleteShader(vert);

	return program;
}

void shader_unload(GLuint shader)
{
	glDeleteProgram(shader);
}
