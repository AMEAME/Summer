#ifndef	_PGM_H_
#define	_PGM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct {
    unsigned char magic_number;
    unsigned int width;
    unsigned int height;
    unsigned int rgb;
    unsigned char **data;
} pgm_t;

FILE *open_file(FILE *fp, char *file_name, char *mode);
int to_i(char *arg, int start, int end);
unsigned char *to_c(unsigned char *string, int num, int start, int digit);
int get_header_size(char *file_name);
int calc_header_size(pgm_t pgm);
pgm_t set_pgm(pgm_t pgm, char *header);
pgm_t set_data(pgm_t pgm, unsigned char *buffer_data, int data_size);
unsigned char *create_header(unsigned char *string, pgm_t pgm);
unsigned char *create_data(unsigned char *string, pgm_t pgm);
pgm_t read_pgm(char *file_name);
void write_pgm(char *file_name, pgm_t pgm);

#endif

