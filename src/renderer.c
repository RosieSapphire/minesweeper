#include <stdlib.h>

#include <glad/glad.h>

#include "error.h"
#include "file_ops.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"
#define STBI_ASSERT(_x) assertf(_x, "STB Image encountered an error.")
#pragma clang diagnostic pop
#include "stb_image.h"

#include "renderer.h"

void renderer_init(struct renderer *const restrict ren,
                   const struct window *const restrict wnd)
{
        assertf(ren, "Trying to init NULL renderer.");
        assertf(!ren->flags,
                "Renderer already has flags: 0x%.8X.",
                ren->flags);

        assertf(wnd, "Trying to init renderer with NULL window.");
        assertf(wnd->width,
                "Trying to init renderer with window of width 0.");
        assertf(wnd->height,
                "Trying to init renderer with window of height 0.");

        gladLoadGL();
        glViewport(0, 0, (int)wnd->width, (int)wnd->height);
        glDisable(GL_DEPTH);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        stbi_set_flip_vertically_on_load(0);

        ren->flags |= REND_FLAG_IS_INIT;
}

void renderer_terminate(struct renderer *const ren)
{
        assertf(ren, "Trying to terminate NULL renderer.");
        assertf(ren->flags & REND_FLAG_IS_INIT,
                "Render was never initialized.");

        ren->flags &= ~REND_FLAG_IS_INIT;
        ren->clear_col.r = 0.0f;
        ren->clear_col.g = 0.0f;
        ren->clear_col.b = 0.0f;
        ren->clear_col.a = 0.0f;
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

uint32_t shader_load(const char *const restrict vpath,
                     const char *const restrict fpath)
{
        uint32_t p, v, f;
        int      stat;

        assertf(vpath, "Vertex shader path is NULL.");
        assertf(fpath, "Fragment shader path is NULL.");

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

void shader_unload(uint32_t *const id)
{
        assertf(id, "Trying to free a NULL Shader ID pointer.");
        assertf(*id, "Shader ID was already freed OR never allocated.");
        glDeleteProgram(*id);
        *id = 0u;
}

uint32_t texture_load(const char *const path)
{
        uint8_t *buf;
        uint32_t id;
        int      w, h, c;

        assertf(path, "Texture path is NULL.");

        buf = stbi_load(path, &w, &h, &c, 3);
        assertf(buf, "Failed to load texture from \"%s\".\n", path);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        /* TODO: Add configuration for clamping and scaling mode. */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /* FIXME: This won't work for every texture. */
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     w,
                     h,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     buf);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(buf);

        return id;
}

void texture_unload(uint32_t *const id)
{
        assertf(id, "Trying to free a NULL Texture ID pointer.");
        assertf(*id, "Texture ID was already freed OR never allocated.");
        glDeleteTextures(1, id);
        *id = 0u;
}

void renderer_clear_color_set(struct renderer *const rnd,
                              const float            r,
                              const float            g,
                              const float            b,
                              const float            a)
{
        rnd->clear_col.r = r;
        rnd->clear_col.g = g;
        rnd->clear_col.b = b;
        rnd->clear_col.a = a;
}

void renderer_clear(const struct renderer *const rnd, const bool depth)
{
        glClearColor(rnd->clear_col.r,
                     rnd->clear_col.g,
                     rnd->clear_col.b,
                     rnd->clear_col.a);
        glClear(GL_COLOR_BUFFER_BIT | (depth * GL_DEPTH_BUFFER_BIT));
}

void renderer_display_to_window(const struct window *const restrict wnd,
                                const struct renderer *const restrict rnd)
{
        (void)rnd;
        glfwSwapBuffers(wnd->handle);
}
