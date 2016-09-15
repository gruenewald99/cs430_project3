#include <stdio.h>
#include <stdlib.h>

typedef struct pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;

}pixel;
pixel magic_pixel_buffer_buffer(FILE *, int ,int);
void write_to_p6(FILE *,pixel, int, int);
void write_to_p3(FILE *, pixel, int, int);
int file_orig_format, max_color;


int main(int argc, char *argv[])
{

  int format = atoi(argv[1]);
  if (format != 3 && format != 6)
  {
    fprintf(stderr, "Error: cannot convert to this format \n");
    exit(1);
  }
  //get file buffer
  FILE * fh = fopen(argv[2], "r");
  int mag_num = fgetc(fh);
  if (mag_num != 'P')
  {
    fprintf(stderr, "Error: this is not a PPM file. \n");
    exit(1);
  }

    file_orig_format =fgetc(fh);
    if(file_orig_format != '3' && file_orig_format != '6')
    {
      fprintf(stderr, "Error: This is not the right format.it must be p3 or p6 \n");
      exit(1);
    }
    fgetc(fh);

    //finds comment_check
    int comment_check= fgetc(fh);
    while(1)
    {
      if(comment_check == 35)
      {
        //while loop to go through comments
        while(1)
        {
          comment_check = fgetc(fh);
          //if
          if(comment_check == 10)
          {
            break;
          }
        }
      }//for checking for comments if statement
      break;
    }
    int width, height;
    fscanf(fh,"%d %d",&width, &height);
    fgetc(fh);
    fscanf(fh, "%d",&max_color);
    fgetc(fh);
    pixel buffer=magic_pixel_buffer_buffer(fh, width, height);




  //output
  FILE * fh2 = fopen(argv[3], "w");
  if(format == 6)
  {
    write_to_p6(fh2, buffer,width,height);
  }
  if(format== 3)
  {
    write_to_p3(fh2,buffer, width, height);
  }
  //git location
  //https://github.com/gruenewald99/cs430_project1.git

}
pixel magic_pixel_buffer_buffer(FILE *fh, int w, int h)
{
  pixel *buffer = malloc(sizeof(pixel)* w * h);
  if(file_orig_format == '3')
  {

      int i;
      for (i=0; i<w*h; i++)
      {
        //runs through the next element to get red color
        char array[5];
        int j=0;
        for(j=0; j<4; j++)
        {
          array[j]=fgetc(fh);
          int num = array[j];
          if(num== 10)
          {
            break;
          }
        }

        int num = atoi(array);
        buffer[i].r = num;

        //gets the green colors

        for(j=0; j<4; j++)
        {
          array[j]=fgetc(fh);
          int num = array[j];
          if(num== 10)
          {
            break;
          }
        }

        num = atoi(array);
        buffer[i].g = num;


        //gets the blue
        for(j=0; j<4; j++)
        {
          array[j]=fgetc(fh);
          int num = array[j];
          if(num== 10)
          {
            break;
          }
        }

        num = atoi(array);
        buffer[i].b = num;

      }



      // for(int i = 0;i<w*h; i++)
      // {
      //   printf("%u\n",buffer[i].r);
      //   printf("%u\n",buffer[i].g);
      //   printf("%u\n",buffer[i].b);
      //
      //
      // }
      //use fgetc and atoi to read and convert ascii buffer
  return *buffer;
  }
  if(file_orig_format == '6')
  {
    for(int i = 0; i< w*h; i++)
    {
      int r,g,b;
      int *buf = malloc(sizeof(int)*3) ;
      *buf = 0;
      int bytes_read;
      bytes_read = fread(buf, 1,3, fh);
      r = (*buf >> 16) & 0xFF;
      g = (*buf >> 8) & 0xFF;
      b = (*buf) & 0xFF;
      buffer[i].r = r;
      buffer[i].g = g;
      buffer[i].b = b;
    }
    return *buffer;
  }

}
void write_to_p6(FILE *fh2, pixel buffer, int w, int h)
{
    fprintf(fh2, "P%c\n", file_orig_format);
    fprintf(fh2, "%d %d\n",w,h);
    fprintf(fh2, "%d\n", max_color);
//fwrite()

}
void write_to_p3(FILE *fh2, pixel buffer, int w, int h)
{

  fprintf(fh2, "P%c\n", file_orig_format);
  fprintf(fh2, "%d %d\n",w,h);
  fprintf(fh2, "%d\n", max_color);

  


//fprintf

}
