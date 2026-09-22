#include "error.h"

#include "file_ops.h"

FILE *fopen_check(const char *const __restrict path,
                  const char *const __restrict mode)
{
#ifdef DEBUG
        FILE *fp;

        assertf(path, "Tried opening file with no path.");
        assertf(mode, "Trying to open file \"%s\" with no mode.", path);

        fp = fopen(path, mode);
        assertf(fp, "Failed to load file from \"%s\" with mode \"%s\".");

        return fp;
#else  /* #ifdef DEBUG */
        return fopen(path, mode);
#endif /* #ifdef DEBUG #else */
}

void fseek_check(FILE *const fp, const int off, const int whence)
{
#ifdef DEBUG
        int r;

        assertf(fp, "fseek(): No file pointer.");
        r = fseek(fp, off, whence);
        assertf(r == 0, "fseek(): Returned %d.", r);
#else  /* #ifdef DEBUG */
        (void)fseek(fp, off, whence);
#endif /* #ifdef DEBUG #else */
}

size_t ftell_check(FILE *const fp)
{
#ifdef DEBUG
        const long t = ftell(fp);

        assertf(t > 0l, "File <%p> has size of 0.\n", fp);
        return (size_t)t;
#else  /* #ifdef DEBUG */
        return (size_t)ftell(fp);
#endif /* #ifdef DEBUG #else */
}

void fclose_check(FILE *const fp)
{
#ifdef DEBUG
        int r;

        r = fclose(fp);
        assertf(r >= 0, "fclose(): Returned %d.", r);
#else  /* #ifdef DEBUG */
        (void)fclose(fp);
#endif /* #ifdef DEBUG #else */
}
