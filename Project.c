#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE * fh = fopen("filename.ppm", "r");
  int c = fgetc(fh);
  if (c != 'P')
  {
    fprintf(stderr, "Error: this is not a PPM file. \n");
  }
  unsigned char gray = 30;
  fprintf(fh, "p6\n");
  fprintf(fh, "%d", gray)


}
