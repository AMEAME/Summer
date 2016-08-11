#include "pgm.c"


int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    char *file_name = argv[1];
    pgm_t pgm = read_pgm(file_name);
    unsigned int i, j;
    unsigned char **duped = (unsigned char **)malloc(pgm.width * pgm.height * sizeof(char));
    for (i = 0; i < pgm.height; i++)
    {
	    duped[i] = (unsigned char *)malloc(pgm.width * sizeof(char));
	    for (j = 0; j < pgm.width; j++)
        {
		    duped[i][j] = pgm.data[i][j];
	    }
    }
    for (i = 0; i < pgm.height; i++)
    {
        for (j = 0; j < pgm.width; j++)
        {
            int j_v = pgm.width - j - 1;
            pgm.data[i][j] = duped[i][j_v];
        }
    }
    write_pgm("out.pgm", pgm);
    return 0;
}
