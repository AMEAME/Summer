#include "pgm.c"


int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    char *file_name = argv[1];
    pgm_t pgm = read_pgm(file_name);
    int i, j;
    for (i = 0; i < pgm.height; i++)
    {
        for (j = 0; j < pgm.width; j++)
        {
            if (pgm.data[i][j] == 0)
            {
                pgm.data[i][j] = pgm.rgb;
            }
            else
            {
                pgm.data[i][j] = pgm.data[i][j] * -1;
            }
        }
    }
    write_pgm("out.pgm", pgm);
    return 0;
}
