#include <stdio.h>
#include <stdlib.h>
void magic_pixel_buffer_buffer(FILE *, int ,int);
int file_orig_format;
typedef struct pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;

}pixel;

int main(int argc, char *argv[])
{

  int format = atoi(argv[1]);
  if (format != 3)
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
    int width, height,max_color;
    fscanf(fh,"%d %d",&width, &height);
    fgetc(fh);
    fscanf(fh, "%d",&max_color);
    fgetc(fh);
    magic_pixel_buffer_buffer(fh, width, height);


  //input



  //output
  //FILE * fh2 = fopen(argv[3], "w");

  /*
  unsigned char gray = 30;
  fprintf(fh, "p6\n");
  fprintf(fh, "%d", gray);
  */
  //git location
  //https://github.com/gruenewald99/cs430_project1.git

}
void magic_pixel_buffer_buffer(FILE *fh, int w, int h)
{
  pixel *buffer = malloc(sizeof(pixel)* w * h);
  if(file_orig_format == '3')
  {

      int i;
      for (i=0; i<w*h; i++)
      {

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

      //gets the blue


      // for(int i = 0;i<w*h; i++)
      // {
      //   printf("%u\n",buffer[i].r);
      //   printf("%u\n",buffer[i].g);
      //   printf("%u\n",buffer[i].b);
      //
      //
      // }
      //use fgetc and atoi to read and convert ascii buffer
  }
  if(file_orig_format == 6)
  {
    



  }

}
