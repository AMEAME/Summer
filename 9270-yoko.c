#include "pgm.c"


int main(int argc, char **argv)
{
    if (argc != 3) return 1;
    char *file_name1 = argv[1];
    char *file_name2 = argv[2];
    pgm_t pgm1 = read_pgm(file_name1);
    pgm_t pgm2 = read_pgm(file_name2);

    unsigned char **duped = (unsigned char **)malloc(pgm1.width * pgm1.height * sizeof(char));
    unsigned int i, j;
    for (i = 0; i < pgm1.height; i++)
    {
	    duped[i] = (unsigned char *)malloc(pgm1.width * sizeof(char));
	    for (j = 0; j < pgm1.width; j++)
        {
		    duped[i][j] = pgm1.data[i][j];
	    }
        free(pgm1.data[i]);
    }
    free(pgm1.data);
    unsigned int pgm1_width = pgm1.width;
    unsigned int pgm1_height = pgm1.height;
    pgm1.width += pgm2.width;
    pgm1.height = pgm1.height >= pgm2.height ? pgm1.height : pgm2.height;

    pgm1.data = (unsigned char**)malloc(pgm1.width * pgm1.height * sizeof(char));
    for (i = 0; i < pgm1.height; i++)
    {
        pgm1.data[i] = (unsigned char*)malloc(pgm1.width * sizeof(char));
        for (j = 0; j < pgm1.width; j++)
        {
            if (j < pgm1_width)
            {
                if (i < pgm1_height)
                {
                    pgm1.data[i][j] = duped[i][j];
                }
                else
                {
                    pgm1.data[i][j] = pgm1.rgb;
                }
            }
            else 
            {
                if (i < pgm2.height)
                {
                    pgm1.data[i][j] = pgm2.data[i][j - pgm1_width];
                }
                else
                {
                    pgm1.data[i][j] = pgm1.rgb;
                }
            }
        }
    }
    write_pgm("out.pgm", pgm1);
    return 0;
}
