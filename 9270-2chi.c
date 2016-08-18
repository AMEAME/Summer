#include "pgm.h"

int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    char *file_name = argv[1];
    pgm_t pgm = read_pgm(file_name);
    int i, j;
    int median = pgm.rgb / 2;
    for (i = 0; i < pgm.height; i++)
    {
        for (j = 0; j < pgm.width; j++)
        {
            pgm.data[i][j] = pgm.data[i][j] >= median ? pgm.rgb : 0;
        }
    }
    write_pgm("out.pgm", pgm);
    return 0;
}
