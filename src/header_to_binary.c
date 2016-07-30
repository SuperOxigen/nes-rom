
#include "sampleHeader.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, char ** argv)
{
    int         fd,     /* File Descriptor */
                i;
    const int   size = sizeof(sample_rom);

    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s FILENAME\n", argv[0]);
        return 0;
    }

    /* open: create, erase, writable. Mode: -rw-r----- */
    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP );

    if (fd == -1)
    {
        fprintf(stderr, "Failed to open file %s\n", argv[1]);
        return 1;
    }

    for (i = 0; i < size; i += 8)
    {
        write(fd, &sample_rom[i], 8);
    }

    close(fd);

    return 0;
}
