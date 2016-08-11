#include "pgm.c"


int main(int argc, char **argv)
{
    if (argc != 3) return 1;
    char *file_name1 = argv[1];
    char *file_name2 = argv[2];
    pgm_t pgm1 = read_pgm(file_name1);
    pgm_t pgm2 = read_pgm(file_name2);
    int pgm1_width = pgm1.width;
    int pgm1_height = pgm1.height;
    unsigned char **duped = (unsigned char **)malloc(pgm1.width * pgm1.height * sizeof(char));
    unsigned int i, j;
    for (i = 0; i < pgm1.height; i++)
    {
	    duped[i] = (unsigned char *)malloc(pgm1.width * sizeof(char));
	    for (j = 0; j < pgm1.width; j++)
        {
		    duped[i][j] = pgm1.data[i][j];
	    }
    }
    
    if (pgm1.width < pgm2.width && pgm1.height < pgm2.height)
    {
        pgm1.width = pgm2.width;
        pgm1.height = pgm2.height;
        pgm1.data = (unsigned char **)realloc(pgm1.data, pgm1.width * pgm1.height * sizeof(char));
        for (i = 0; i < pgm1.height; i++)
        {
            pgm1.data[i] = (unsigned char *)realloc(pgm1.data[i], pgm1.width * sizeof(char));
            for (j = 0; j < pgm1.width; j++)
            {
                pgm1.data[i][j] = pgm2.data[i][j];
                if (i < pgm1_height && j < pgm1_width)
                {
                    pgm1.data[i][j] = duped[i][j];
                }
            }
        }
    }
    else if (pgm1.width >= pgm2.width && pgm1.height >= pgm2.height)
    {
        for (i = 0; i < pgm1.height; i++)
        {
            for (j = 0; j < pgm1.width; j++)
            {
                if (i < pgm2.height && j < pgm2.width)
                {
                    pgm1.data[i][j] = pgm2.data[i][j];
                }
            }
        }
    }
    else
    {
        puts("error");
    }
    write_pgm("out.pgm", pgm1);
    return 0;
}
