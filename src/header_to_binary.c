/*
 *  NES ROM - Header to Binary Test
 *
 *  Copyright (c) 2016 Alex Dale
 *
 *  This program is licensed under the MIT License (MIT). Please review the
 *  accompanying LICENSE file for details.
 *
 *  This program is intended to test the binary to header command in nes-rom.py
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include "sampleHeader.h"
#include <stdio.h>      /* fprintf(), stderr */
#include <unistd.h>     /* close(), write() */
#include <fcntl.h>      /* open(), O_CREAT, O_TRUNC, O_WRONLY */
#include <sys/stat.h>   /* S_IRUSR, S_IWUSR, S_IRGRP */

int main(int argc, char ** argv)
{
    int         fd;     /* File descriptor of output file */
    unsigned     i;

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

    for (i = 0; i < sizeof(sample_rom) - 8; i += 8)
    {
        write(fd, &sample_rom[i], 8);
    }

    if (i < sizeof(sample_rom))
    {
        write(fd, &sample_rom[i], sizeof(sample_rom) - i);
    }

    close(fd);

    return 0;
}
