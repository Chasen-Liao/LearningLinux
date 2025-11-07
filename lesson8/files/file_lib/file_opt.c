// file_opt.c

#include <stdio.h>

int copy_file(const char *src, const char *dest)
{
    FILE *in = fopen(src, "r");
    if (in == NULL)
    {
        perror("Failed to open source file");
        return 1;
    }

    FILE *out = fopen(dest, "w");
    if (!out)
    {
        perror("Failed to open destination file");
        fclose(in);
        return 1;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0)
    {
        fwrite(buffer, 1, bytes, out);
    }
    fclose(in);
    fclose(out);
    return 0;
}