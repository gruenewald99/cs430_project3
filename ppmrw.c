//git location
//https://github.com/gruenewald99/cs430_project1.git
#include <stdio.h>
#include <stdlib.h>
//struct to hold all data in between new and old formats
typedef struct pixel
{
  unsigned char r;
  unsigned char g;
  unsigned char b;

}pixel;
//global variable so all functions can reach the data
pixel *buffer;
// function declarations
void magic_pixel_buffer_buffer(FILE *, int ,int);
void write_to_p6(FILE *, int, int);
void write_to_p3(FILE *, int, int);
//global variables for max colors and the original format
int file_orig_format, max_color;
int width, height;


int main(int argc, char *argv[])
{
//gets the format to be changed to by the arguments in argv
//then checks to make sure it is the right number
  int format = atoi(argv[1]);
  if (format != 3 && format != 6)
  {
    fprintf(stderr, "Error: cannot convert to this format \n");
    exit(1);
  }
  //gets the input file buffer and makes sure it is a ppm file
  FILE * fh = fopen(argv[2], "r");
  int mag_num = fgetc(fh);
  if (mag_num != 'P')
  {
    fprintf(stderr, "Error: this is not a PPM file. \n");
    exit(1);
  }
//saves the file format and makes sure that it is a p3 or p6
  file_orig_format =fgetc(fh);
  if(file_orig_format != '3' && file_orig_format != '6')
    {
      fprintf(stderr, "Error: This is not the right format.it must be p3 or p6 \n");
      exit(1);
    }
  //advances the file pointer by 1
  fgetc(fh);

  //while loop that skips past comments until it reaches a nl with a number
  int comment_check= fgetc(fh);
  //outside loop continues to check for comments
  while(1)
  {
    if(comment_check == 35)
    {
    //while loop to go through comments
      while(1)
      {
        comment_check = fgetc(fh);
        //if new line break loop
        if(comment_check == 10)
        {
          break;
        }
      }
    }
    break;
  }
  //gets width, height and max color and saves them to variables
  fscanf(fh,"%d %d",&width, &height);
  fgetc(fh);
  fscanf(fh, "%d",&max_color);
  //checks to see if max color value is too high
  if(max_color >255)
  {
    fprintf(stderr, "Error: This has too many color channels \n");
    exit(1);
  }
  //advances file pointer by 1
  fgetc(fh);
  //calls buffer to go through data and put it into memory
  magic_pixel_buffer_buffer(fh, width, height);

  //output file header
  FILE * fh2 = fopen(argv[3], "w");
  //checks to see what the files need to be converted to then sends
  //then calls the function to write to that format
  if(format == 6)
  {
    write_to_p6(fh2,width,height);
  }
  if(format== 3)
  {
    write_to_p3(fh2, width, height);
  }


}//end of main file
void magic_pixel_buffer_buffer(FILE *fh, int w, int h)
{
  //creates the size of the buffer necessary to
  buffer = malloc(sizeof(pixel)* w * h);
  //if file is a p3
  if(file_orig_format == '3')
  {

    int i;
    for (i=0; i<w*h; i++)
    {
      //runs through next element to get red color
      char array[5];
      int j=0;
      //puts each element into a array until it reaches a new line
      for(j=0; j<4; j++)
      {
        array[j]=fgetc(fh);
        int num = array[j];
        if(num== 10)
        {
          break;
        }
      }
      //converts array to a integer then saves the number to memory
      int num = atoi(array);
      buffer[i].r = num;

      //runs through next element to get green color
      //puts each element into a array until it reaches a new line
      for(j=0; j<4; j++)
      {
        array[j]=fgetc(fh);
        int num = array[j];
        if(num== 10)
        {
          break;
        }
      }
      //converts array to a integer then saves the number to memory
      num = atoi(array);
      buffer[i].g = num;


      //runs through next element to get green color
      //puts each element into a array until it reaches a new line
      for(j=0; j<4; j++)
      {
        array[j]=fgetc(fh);
        int num = array[j];
        if(num== 10)
        {
          break;
        }
      }
      //converts array to a integer then saves the number to memory
      num = atoi(array);
      buffer[i].b = num;

    }//end of giant for loop to run through each pixel

  }
  //if file was a p6 aka raw bits
  if(file_orig_format == '6')
  {
    //for loop that goes through each pixels worth of data
    for(int i = 0; i< w*h; i++)
    {
      //creats a temp buffer for each byte of data representing a color for the
      //pixel
      int *buf = malloc(sizeof(int)*3) ;
      *buf = 0;
      //variable i probably dont need
      int bytes_read;
      //reads in a byte that represents each color and saves it into our struct
      //in memory
      bytes_read = fread(buf, 1,1, fh);
      buffer[i].r = buf[0];
      bytes_read = fread(buf, 1,1, fh);
      buffer[i].g = buf[0];
      bytes_read = fread(buf, 1,1, fh);
      buffer[i].b = buf[0];

    }
  }

}
void write_to_p6(FILE *fh2, int w, int h)
{
  //writes to the file and puts in all the header information
  // format, width and height and max color
  fprintf(fh2, "P6\n");
  fprintf(fh2, "%d %d\n",w,h);
  fprintf(fh2, "%d\n", max_color);
  // runs through the file writing in ascii byte by byte
  //from our struct in memory
  for(int i=0 ; i<w*h; i++)
  {
    fwrite(&buffer[i].r,1,1,fh2);
    fwrite(&buffer[i].g,1,1,fh2);
    fwrite(&buffer[i].b,1,1,fh2);
  }


}
void write_to_p3(FILE *fh2, int w, int h)
{
  //writes to the file and puts in all the header information
  //format, width and height and max color
  fprintf(fh2, "P3\n" );
  fprintf(fh2, "%d %d\n",w,h);
  fprintf(fh2, "%d\n", max_color);
  // runs through the file writing in raw bits using fprintf byte by byte
  //from our struct in memory
  for(int i=0;i<h*w; i++)
  {
    fprintf(fh2, "%u\n", (unsigned char)buffer[i].r);
    fprintf(fh2, "%u\n", buffer[i].g);
    fprintf(fh2, "%u\n", buffer[i].b);

  }
//fprintf

}
