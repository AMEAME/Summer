#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
  char *input_filename = argv[1];
  char *output_filename = argv[2];
  
  char magic_number[3];
  int width, height;
  int rgb;

  // 読み込み
  FILE *file1;
  if((file1 = fopen(input_filename, "rb")) == NULL) {
		perror(input_filename);
    exit(EXIT_FAILURE);
	}

  fscanf(file1,"%s", magic_number);
  fscanf(file1,"%d %d", &width, &height);
  fscanf(file1,"%d", &rgb);
  unsigned char data[height][width];
  int x, y;
  for(y = 0; y < height; y++) {
	  for (x = 0; x < width; x++) {
      fscanf(file1, "%c", &(data[y][x]));
	  }
  }
  fclose(file1);

  //書き込み
  FILE *file2;
  if((file2 = fopen(output_filename, "wb")) == NULL) {
    perror(output_filename);
    exit(EXIT_FAILURE);
  }
  fprintf(file2,"%s\n%d %d\n%d\n", magic_number, width
          , height, rgb);
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      fprintf(file2, "%c", data[y][x]);
    }
  }
  fclose(file2);  

  return 0;
}
