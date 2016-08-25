/* Copyright 2016 Ryuichirouh Ikeuchi. All rights reserved. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


typedef struct {
    unsigned char magic_number;
    int width;
    int height;
    int rgb;
    unsigned char **data;
} pgm_t;

FILE *open_file(FILE *fp, char *file_name, char *mode) {
    if((fp = fopen(file_name, mode)) == NULL) {
		perror(file_name);
        exit(EXIT_FAILURE);
	}
    return fp;
}

int to_i(char *arg, int start, int end) {
    int num = 0;
    int digit = end - start - 1;
    for (; start < end; start++, digit--) {
        num += (arg[start] - '0') * (int)pow(10, digit);
    }
    return num;
}

unsigned char *to_c(unsigned char *string, int num, int start, int digit) {
    for (; digit > 0; start++, digit--) {
        int tmp = (int)pow(10, digit - 1);
        string[start] = num / tmp + '0';
        num %= tmp;
    }
    return string;
}

int get_header_size(char *file_name) {
    FILE *fp = NULL;
    fp = open_file(fp, file_name, "r");
    int space = 0, header_size = 0;
    while (space <= 3) {
        space += isspace(getc(fp)) ? 1 : 0;
        header_size++;
    }
    fclose(fp);
    return header_size;
}

int calc_header_size(pgm_t pgm) {
    int tmp, sum = 6;
    for (tmp = pgm.width; tmp != 0; sum++, tmp /= 10) ;
    for (tmp = pgm.height; tmp != 0; sum++, tmp /= 10) ;
    for (tmp = pgm.rgb; tmp != 0; sum++, tmp /= 10) ;
    return sum;
}

pgm_t set_pgm(pgm_t pgm, char *header) {
    pgm.magic_number = header[1] - '0';
    int start = 3, end = 3;
    for (; !isspace(header[end]); end++) ;
    pgm.width = to_i(header, start, end);
    for (start = ++end; !isspace(header[end]); end++) ;
    pgm.height = to_i(header, start, end);
    for (start = ++end; !isspace(header[end]); end++) ;
    pgm.rgb = to_i(header, start, end);
    return pgm;
}

pgm_t set_data(pgm_t pgm, unsigned char *buffer_data, int data_size) {
    pgm.data = (unsigned char **)malloc(data_size * sizeof(char *));
    int i, j;
    for (i = 0; i < pgm.height; i++) {
	    pgm.data[i] = (unsigned char *)malloc(sizeof(char) * pgm.width);
	    for (j = 0; j < pgm.width; j++) {
		    pgm.data[i][j] = buffer_data[i * pgm.width + j];
	    }
    }
    return pgm;
}

unsigned char *create_header(unsigned char *string, pgm_t pgm) {
    string[0] = 'P';
    string[1] = pgm.magic_number + '0';
    string[2] = '\n';
    int start = 3, digit, tmp;
    for (digit = 0, tmp = pgm.width; tmp != 0; digit++, tmp /= 10) ;
    string = to_c(string, pgm.width, start, digit);
    start += digit;
    string[start++] = ' ';
    for (digit = 0, tmp = pgm.height; tmp != 0; digit++, tmp /= 10) ;
    string = to_c(string, pgm.height, start, digit);
    start += digit;
    string[start++] = '\n';
    for (digit = 0, tmp = pgm.rgb; tmp != 0; digit++, tmp /= 10) ;
    string = to_c(string, pgm.rgb, start, digit);
    start += digit;
    string[start] = '\n';
    return string;
}

unsigned char *create_data(unsigned char *string, pgm_t pgm) {
    int i, j;
    int header_size = calc_header_size(pgm);
    for (i = 0; i < pgm.height; i++) {
        for (j = 0; j < pgm.width; j++) {
            string[i * pgm.width + j + header_size] = pgm.data[i][j];
        }
    }
    return string;
}

pgm_t read_pgm(char *file_name) {
    pgm_t pgm;
    int header_size = get_header_size(file_name);
    FILE *fp = NULL;
    fp = open_file(fp, file_name, "rb");
    char header[header_size];
    fread(header, sizeof(char), header_size, fp);
    pgm = set_pgm(pgm, header);
    int data_size = pgm.width * pgm.height;
    unsigned char *buffer_data = (unsigned char *)malloc(data_size * sizeof(char));
    fread(buffer_data, sizeof(char), data_size, fp);
    pgm = set_data(pgm, buffer_data, data_size);
    fclose(fp);
    return pgm;
}

void write_pgm(char *file_name, pgm_t pgm) {
    FILE* fp = NULL;
    fp = open_file(fp, file_name, "wb");
    int pgm_size = pgm.width * pgm.height + calc_header_size(pgm);
    unsigned char *buffer = (unsigned char *)malloc(pgm_size * sizeof(char));
    buffer = create_header(buffer, pgm);
    buffer = create_data(buffer, pgm);
    fwrite(buffer, sizeof(char), pgm_size, fp);
    fclose(fp);
}

int main(int argc, char **argv) {
    if (argc != 3) return 1;
    char *file_name1 = argv[1];
    char *file_name2 = argv[2];
    pgm_t pgm1 = read_pgm(file_name1);
    pgm_t pgm2 = read_pgm(file_name2);

    // 画像ファイルのデータを編集します。
    // ========================================================================
    unsigned char **duped = (unsigned char **)malloc(pgm1.width * pgm1.height * sizeof(char));
    unsigned int i, j;
    for (i = 0; i < pgm1.height; i++) {
	    duped[i] = (unsigned char *)malloc(pgm1.width * sizeof(char));
	    for (j = 0; j < pgm1.width; j++) {
		    duped[i][j] = pgm1.data[i][j];
	    }
        free(pgm1.data[i]);
    }
    free(pgm1.data);
    unsigned int pgm1_width = pgm1.width;
    unsigned int pgm1_height = pgm1.height;
    pgm1.height += pgm2.height;
    pgm1.width = pgm1.width >= pgm2.width ? pgm1.width : pgm2.width;

    pgm1.data = (unsigned char **)malloc(pgm1.width * pgm1.height * sizeof(char));
    for (i = 0; i < pgm1.height; i++) {
        pgm1.data[i] = (unsigned char *)malloc(pgm1.width * sizeof(char));
        for (j = 0; j < pgm1.width; j++) {
            if (i < pgm1_height) {
                pgm1.data[i][j] = j < pgm1_width ? duped[i][j] : pgm1.rgb;
            } else {
                pgm1.data[i][j] = j < pgm2.width ? pgm2.data[i - pgm1_height][j] : pgm1.rgb;
            }
        }
    }
    // ========================================================================

    write_pgm("out.pgm", pgm1);
    return 0;
}
