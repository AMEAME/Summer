#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 


void _file_error(char *file_name, char *mode)
{
    FILE *fp;
    if ((fp = fopen(file_name, mode)) == NULL)
    {
        printf("Cannot open input file '%s'", file_name);
        exit(EXIT_FAILURE);
    }
}

int get_file_size(char *file_name)
{
    _file_error(file_name, "r");
    FILE *fp = fopen(file_name, "r");
    int counter = 0;
    while (getc(fp) != EOF) counter++;
    fclose(fp);
    return counter;
}

char *read(char *file_name)
{
    char ch;
    char *string = malloc(sizeof(char)
        * (get_file_size(file_name) + 1));
    FILE *fp = fopen(file_name, "r");
    char *nil = "";
    strcpy(string, nil);
    while ((ch = getc(fp)) != EOF)
    {
        char s[] = {ch, '\0'};
        strcat(string, s);
    }
    fclose(fp);
    return string;
}

void write(char *file_name, char *string)
{
    _file_error(file_name, "w");
    FILE *fp = fopen(file_name, "w");
    fputs(string, fp);
    fclose(fp);
}

int main(int argc, char **argv)
{
    if (argc != 2) return 1;
    char *file_name = argv[1];
    char *string = read(file_name);
    int i;
    for (i = 0; i < strlen(string); i++)
    {
        string[i] = string[i] >= 0 ? CHAR_MAX : CHAR_MIN;
    }
    write("out.png", string);
    return 0;
}
