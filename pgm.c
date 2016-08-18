#include "pgm.h"

FILE *open_file(FILE *fp, char *file_name, char *mode)
{
    if((fp = fopen(file_name, mode)) == NULL)
    {
		perror(file_name);
        exit(EXIT_FAILURE);
	}
    return fp;
}

int to_i(char *arg, int start, int end)
{
    int num = 0;
    int digit = end - start - 1;
    for (; start < end; start++, digit--)
    {
        num += (arg[start] - '0') * (int)pow(10, digit);
    }
    return num;
}

unsigned char *to_c(unsigned char *string, int num, int start, int digit)
{
    for (; digit > 0; start++, digit--)
    {
        int tmp = (int)pow(10, digit - 1);
        string[start] = num / tmp + '0';
        num %= tmp;
    }
    return string;
}

int get_header_size(char *file_name)
{
    FILE *fp = open_file(fp, file_name, "r");
    int space = 0, header_size = 0;
    while (space <= 3)
    {
        if (isspace(getc(fp)))
        {
            space++;
        }
        header_size++;
    }
    fclose(fp);
    return header_size;
}

int calc_header_size(pgm_t pgm)
{
    unsigned int tmp, sum = 6;
    for (tmp = pgm.width; tmp != 0; sum++, tmp /= 10) {}
    for (tmp = pgm.height; tmp != 0; sum++, tmp /= 10) {}
    for (tmp = pgm.rgb; tmp != 0; sum++, tmp /= 10) {}
    return sum;
}

pgm_t set_pgm(pgm_t pgm, char *header)
{
    pgm.magic_number = header[1] - '0';
    int start = 3, end = 3;
    for (; !isspace(header[end]); end++) {}
    pgm.width = to_i(header, start, end);
    for (start = ++end; !isspace(header[end]); end++) {}
    pgm.height = to_i(header, start, end);
    for (start = ++end; !isspace(header[end]); end++) {}
    pgm.rgb = to_i(header, start, end);
    return pgm;
}

pgm_t set_data(pgm_t pgm, unsigned char *buffer_data, unsigned int data_size)
{
    pgm.data = (unsigned char **)malloc(data_size * sizeof(char *));
    unsigned int i, j;
    for (i = 0; i < pgm.height; i++)
    {
	    pgm.data[i] = malloc(sizeof(char) * pgm.width);
	    for (j = 0; j < pgm.width; j++)
        {
		    pgm.data[i][j] = buffer_data[i * pgm.width + j];
	    }
    }
    return pgm;
}

unsigned char *create_header(unsigned char *string, pgm_t pgm)
{
    string[0] = 'P';
    string[1] = pgm.magic_number + '0';
    string[2] = '\n';
    int start = 3, digit, tmp;
    for (digit = 0, tmp = pgm.width; tmp != 0; digit++, tmp /= 10) {}
    string = to_c(string, pgm.width, start, digit);
    start += digit;
    string[start++] = ' ';
    for (digit = 0, tmp = pgm.height; tmp != 0; digit++, tmp /= 10) {}
    string = to_c(string, pgm.height, start, digit);
    start += digit;
    string[start++] = '\n';
    for (digit = 0, tmp = pgm.rgb; tmp != 0; digit++, tmp /= 10) {}
    string = to_c(string, pgm.rgb, start, digit);
    start += digit;
    string[start] = '\n';
    return string;
}

unsigned char *create_data(unsigned char *string, pgm_t pgm)
{
    unsigned int i, j;
    int header_size = calc_header_size(pgm);
    for (i = 0; i < pgm.height; i++)
    {
        for (j = 0; j < pgm.width; j++)
        {
            string[i * pgm.width + j + header_size] = pgm.data[i][j];
        }
    }
    return string;
}

pgm_t read_pgm(char *file_name)
{
    pgm_t pgm;
    int header_size = get_header_size(file_name);
    FILE *fp = open_file(fp, file_name, "rb");
    char header[header_size];
    fread(header, sizeof(char), header_size, fp);
    pgm = set_pgm(pgm, header);
    unsigned int data_size = pgm.width * pgm.height;
    unsigned char *buffer_data = (unsigned char *)malloc(data_size * sizeof(char));
    fread(buffer_data, sizeof(char), data_size, fp);
    pgm = set_data(pgm, buffer_data, data_size);
    fclose(fp);
    return pgm;
}

void write_pgm(char *file_name, pgm_t pgm)
{
    FILE* fp = open_file(fp, file_name, "wb");
    unsigned int pgm_size = pgm.width * pgm.height + calc_header_size(pgm);
    unsigned char *buffer = (unsigned char *)malloc(pgm_size * sizeof(char));
    buffer = create_header(buffer, pgm);
    buffer = create_data(buffer, pgm);
    fwrite(buffer, sizeof(char), pgm_size, fp);
    fclose(fp);
}