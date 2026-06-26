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

        if (ren->shader)
                renderer_shader_unload(ren);

        ren->shader = 0u;
        ren->flags &= ~REND_FLAG_IS_INIT;
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

void renderer_shader_load(struct renderer *const restrict ren,
                          const char *const restrict vpath,
                          const char *const restrict fpath)
{
        uint32_t p, v, f;
        int      stat;

        assertf(ren, "Renderer is NULL.");
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

        ren->shader = p;
}

void renderer_shader_unload(struct renderer *const restrict ren)
{
        assertf(ren, "Renderer is NULL.");
        assertf(ren->shader, "Renderer's shader is 0.");
        glDeleteProgram(ren->shader);
}

uint32_t renderer_texture_load(struct renderer *const restrict ren,
                               const char *const restrict path)
{
        uint8_t *buf;
        uint32_t id;
        int      w, h, c;

        assertf(ren, "Renderer is NULL.");
        assertf(path, "Texture path is NULL.");

        /* Load the texture itself. */
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

        /* Now add it to the array. */
        if (!ren->tex_cnt) {
                assertf(!ren->tex_arr,
                        "Texture count is 0, but array is <%p>; non-null.",
                        ren->tex_arr);
                ren->tex_arr = (uint32_t *)malloc(sizeof(*ren->tex_arr) *
                                                  ++ren->tex_cnt);
                assertf(ren->tex_arr,
                        "Failed to allocate texture array for \"%s\".",
                        path);
        } else {
                uint32_t *arr_new;

                assertf(ren->tex_arr,
                        "Texture count is %lu, but array is NULL.",
                        ren->tex_cnt);
                arr_new = (uint32_t *)realloc(ren->tex_arr,
                                              sizeof(*ren->tex_arr) *
                                                      ++ren->tex_cnt);
                assertf(arr_new,
                        "Failed to reallocate texture array for \"%s\" for count %lu.",
                        path,
                        ren->tex_cnt);
                ren->tex_arr = arr_new;
        }

        ren->tex_arr[ren->tex_cnt - 1u] = id;

        return id;
}

static uint32_t renderer_texture_get_index(const struct renderer *const rnd,
                                           const uint32_t               id)
{
        uint32_t  cnt;
        uint32_t *arr;

        assertf(rnd, "Renderer is NULL.");
        assertf(rnd->tex_cnt && rnd->tex_arr,
                "Renderer has no textures to find [cnt=%lu arr=<%p>].\n",
                rnd->tex_cnt,
                rnd->tex_arr);
        assertf(id, "Not allowed to use Tex ID of 0; invalid ID value.");

        cnt = rnd->tex_cnt;
        arr = rnd->tex_arr;
        for (uint32_t i = 0u; i < cnt; ++i)
                if (id == arr[i])
                        return i;

        assertf(0,
                "Failed to find texture id %lu "
                "in array <%p> of %lu elements.",
                id,
                arr,
                cnt);
        return UINT32_MAX;
}

void renderer_texture_unload(struct renderer *const restrict ren,
                             const uint32_t id)
{
        const uint32_t ind = renderer_texture_get_index(ren, id);

        assertf(ind != UINT32_MAX,
                "Failed to unload texture id %lu from renderer's "
                "array of %lu; couldn't find it in list.",
                id,
                ren->tex_cnt);

        glDeleteTextures(1, &id);
}

void renderer_clear(const float r,
                    const float g,
                    const float b,
                    const float a,
                    const bool  depth)
{
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | (depth * GL_DEPTH_BUFFER_BIT));
}

void renderer_display_to_window(const struct window *const restrict wnd,
                                const struct renderer *const restrict rnd)
{
        (void)rnd;
        glfwSwapBuffers(wnd->handle);
}
