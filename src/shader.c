#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "error.h"

#include "shader.h"

static FILE *fopen_check(const char *const __restrict path,
                         const char *const __restrict mode)
{
        FILE *fp;

        assertf(path, "Tried opening file with no path.");
        assertf(mode, "Trying to open file \"%s\" with no mode.", path);

        fp = fopen(path, mode);
        assertf(fp, "Failed to load file from \"%s\" with mode \"%s\".");

        return fp;
}

static void fseek_check(FILE *const fp, const int off, const int whence)
{
        int r;

        assertf(fp, "fseek(): No file pointer.");
        r = fseek(fp, off, whence);
        assertf(r == 0, "fseek(): Returned %d.", r);
}

static size_t ftell_check(FILE *const fp)
{
        const long t = ftell(fp);

        assertf(t > 0l, "File <%p> has size of 0.\n", fp);
        return (size_t)t;
}

static void fclose_check(FILE *const fp)
{
        int r;

        r = fclose(fp);
        assertf(r >= 0, "fclose(): Returned %d.", r);
}

static uint32_t shader_part_compile(const char *const path,
                                    const uint32_t    type)
{
        FILE    *fp;
        char    *buf;
        size_t   sz;
        uint32_t s;
        int      stat;

        fp = fopen_check(path, "rb");
        fseek_check(fp, 0, SEEK_END);
        sz = ftell_check(fp);
        rewind(fp);

        buf = (char *)malloc(sz);
        fread(buf, sizeof(char), sz, fp);
        buf[sz - 1ul] = '\0';
        fclose_check(fp);

        s = glCreateShader(type);
        glShaderSource(s, 1, (const char *const *const)&buf, NULL);
        glCompileShader(s);

        glGetShaderiv(s, GL_COMPILE_STATUS, &stat);
        if (!stat) {
                char log[512];

                glGetShaderInfoLog(s, 512, NULL, log);
                printf("Failed to create %s Shader: '%s'\n",
                       (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment",
                       log);

                return 0;
        }

        free(buf);

        return s;
}

uint32_t shader_load(const char *const __restrict vpath,
                     const char *const __restrict fpath)
{
        uint32_t p, v, f;
        int      stat;

        p = glCreateProgram();
        v = shader_part_compile(vpath, GL_VERTEX_SHADER);
        f = shader_part_compile(fpath, GL_FRAGMENT_SHADER);

        glAttachShader(p, v);
        glAttachShader(p, f);
        glLinkProgram(p);

        glGetProgramiv(p, GL_LINK_STATUS, &stat);

        if (!stat) {
                char log[512];

                glGetProgramInfoLog(p, 512, NULL, log);
                fprintf(stderr, "ERROR::SHADER::PROGRAM: %s\n", log);
                exit(EXIT_FAILURE);
        }

        glDeleteShader(f);
        glDeleteShader(v);

        return p;
}

void shader_unload(GLuint shader)
{
        glDeleteProgram(shader);
}
