/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 * 
 * Each of the functions below can be considered a start for you. 
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
 
 struct bpp24{
  unsigned char b;
  unsigned char g;
  unsigned char r;
 };

int bmp_open( char* bmp_filename,        unsigned int *width, 
              unsigned int *height,      unsigned int *bits_per_pixel, 
              unsigned int *padding,     unsigned int *data_size, 
              unsigned int *data_offset, unsigned char** img_data ){

  // YOUR CODE FOR Q1 SHOULD REPLACE EVERYTHING FROM HERE
  //test "BM"
 
  int bmpwidth,bmpheight,linebyte;  

  char *p;
  char *tname = (char*)malloc(strlen(bmp_filename)*sizeof(char)); // allocate space
  strcpy(tname,bmp_filename); // copy the string of bmp_filename
  p = tname;
  // convert upper case to  lower case
  while(*p != '\0'){
    if (*p >= 'A' && *p <= 'Z'){
      *p += 32;
    }
    p++;
  }

  // check whether "bm" is the substring of image name
  p = strstr(tname,"bm");
  if(p == NULL){
    return 1;
  }

  // open the image
  int fd = open(bmp_filename,O_RDONLY);
  int w = 0;
  int h = 0;
  int bpp = 0;
  int offset = 0;
  int dsize = 0;
  int pad = 0;
  // read the info
  lseek(fd, 0x02, SEEK_SET); read(fd, &dsize, 4);
  lseek(fd, 0x0A, SEEK_SET); read(fd, &offset, 4);
  lseek(fd, 0x12, SEEK_SET); read(fd, &w, 4); 
  lseek(fd, 0x16, SEEK_SET); read(fd, &h, 4); 
  lseek(fd, 0x1c, SEEK_SET); read(fd, &bpp, 2); 

  // compute padding
  int s = w * (bpp/8);
  if(s % 4 != 0){
    pad = 4 - w*bpp/8 % 4;
  }

  *width = w;  
  *height = h;  
  *bits_per_pixel = bpp;  
  *padding=pad;
  *data_size=dsize;
  *data_offset=offset;

  char* text;
  FILE *pf = fopen(bmp_filename,"r");
  fseek(pf,0,SEEK_END);
  long lSize = ftell(pf);
  text=(char*)malloc(lSize);
  rewind(pf); 
  fread(text,sizeof(char),lSize,pf);
  *img_data = text;
  
  // TO HERE
  close(fd);
  fclose(pf);
  return 0;  
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){

  if( *img_data != NULL ){
    free( *img_data );
    *img_data = NULL;
  }
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename, 
              unsigned int x_min, unsigned int y_min, unsigned int x_max, unsigned int y_max,
              unsigned char red, unsigned char green, unsigned char blue )
{
  unsigned int img_width;
  unsigned int img_height;
  unsigned int bits_per_pixel;
  unsigned int data_size;
  unsigned int padding;
  unsigned int data_offset;
  unsigned char* img_data    = NULL;
  
  int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data ); 
  
  if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
 
  // Copy data to new image
  char* newimage = (char*)malloc(sizeof(char)*(data_size));
  memcpy(newimage,img_data,data_size);

  char *p = newimage + data_offset;

  int total = img_height*img_width;
  int count = 0;
  int row = 0;
  int col = 0;

  // iterate pixel by pixel
  while(count < total){
    row = count/img_width;
    col = count%img_width;
    if( row >= y_min && row <= y_max && col>=x_min && col <= x_max){
      // image data format: g b r g b r g b r
      *(p+3*count+0) = green;
      *(p+3*count+1) = blue;
      *(p+3*count+2) = red;
    }
    count ++ ;
  }

  // write the data into new image
  FILE *fp = fopen(output_bmp_filename,"w+");
  fwrite(newimage,sizeof(char),data_size,fp);

  // TO HERE!
  
  bmp_close( &img_data );
  fclose(fp);
  
  return 0;
} 


int Min(int x, int y){
  if(x>y)
    return y;
  else
    return x;
}      

int Max(int x, int y){
  if(x>y)
    return x;
  else
    return y;
}     

int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){

  unsigned int img_width1;
  unsigned int img_height1;
  unsigned int bits_per_pixel1;
  unsigned int data_size1;
  unsigned int padding1;
  unsigned int data_offset1;
  unsigned char* img_data1    = NULL;
  
  int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
  
  unsigned int img_width2;
  unsigned int img_height2;
  unsigned int bits_per_pixel2;
  unsigned int data_size2;
  unsigned int padding2;
  unsigned int data_offset2;
  unsigned char* img_data2    = NULL;
  
  open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 ); 
  
  if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
  
  // YOUR CODE FOR Q3 SHOULD REPLACE EVERYTHING FROM HERE

  // cooperations x,y of left down point and right up point of image 1
  int x1=0;
  int y1=0;
  int x2=img_width1;
  int y2=img_height1;

  // cooperations x,y of left down point and right up point of image 2
  int nx1 = x_offset;
  int ny1 = y_offset;
  int nx2 = x_offset + img_width2;
  int ny2 = y_offset + img_height2;

  int largew= Max(nx2,x2)-Min(nx1,x1);
  int largeh= Max(ny2,y2)-Min(ny1,y1);;


  // compute the shift distance

  int basex = 0;
  int basey = 0;
  int basex2=0;
  int basey2=0;
  if(nx1 > x1){
    basex = 0;
    basex2 = nx1;
  }
  else{
    basex = -nx1;
    basex2 = 0;
  }

  if(ny1 > y1){
      basey = 0;
      basey2 = ny1;
    }
  else{
    basey = -ny1;
    basey2 = 0;
  }
  
  // Note The lenght of every line of data section of bmp must be multiple of 4bytes
  // if not, must add padding as supplement
  int rowsize = 3*largew;
  if(rowsize % 4 != 0){
    rowsize += 4-rowsize % 4;
  }
  int newsize = data_offset1 + largeh*rowsize;
  char* newimage = (char*)malloc(sizeof(char)*(newsize));
  memcpy(newimage,img_data1,data_offset1);

  // Note ! The lenght of every line of data section of bmp must be multiple of 4bytes
  // The following is a convenient way to get the length of line
  int linesize1 = (data_size1 - data_offset1)/img_height1;
  int linesize2 = (data_size2 - data_offset2)/img_height2;

  char *p = newimage + data_offset1; // start position of data section of new image
  int i=0;
  int j=0;
  // iterate in the new image
  for(i=largeh-1;i>=0;i--){
    for(j=0;j<largew;j++){
      // draw image 1
      if(i>=basey && i<basey+img_height1 && j>=basex && j< basex+img_width1){
        int ni = i-basey;
        int nj = j-basex;
        p[(i)*rowsize+3*j + 0] = img_data1[data_offset1 + ni*linesize1 + 3*nj + 0];
        p[(i)*rowsize+3*j + 1] = img_data1[data_offset1 + ni*linesize1 + 3*nj + 1];
        p[(i)*rowsize+3*j + 2] = img_data1[data_offset1 + ni*linesize1 + 3*nj + 2];
      }
      // draw image 2
      if(i>=basey2 && j>=basex2 && i<basey2+img_height2 && j<basex2+img_width2){
        int yy = i- basey2;
        int xx = j- basex2;
        p[i*rowsize+3*j + 0] = img_data2[data_offset2 + yy*linesize2 + 3*xx + 0];
        p[i*rowsize+3*j + 1] = img_data2[data_offset2 + yy*linesize2 + 3*xx + 1];
        p[i*rowsize+3*j + 2] = img_data2[data_offset2 + yy*linesize2 + 3*xx + 2];
      }

    }
  }

// write the data into new image
  FILE *fp = fopen(bmp_result,"w+");
  fwrite(newimage,sizeof(char),newsize,fp);

  // for debug
  // printf("%d\n",data_offset1 );
  // printf("%d %d\n",img_width1, img_height1);
  // printf("%d %d\n",img_width2, img_height2);
  // printf("%d %d\n",largew, largeh);
  // printf("%d\n",rowsize);
  // printf("%d\n",newsize);
  // printf("%d\n",linesize1);
  // printf("%d %d %d %d\n",basex, basey, basex2, basey2);

  // revise the data in the head of file
  int fd = open(bmp_result,O_WRONLY);
  int realw = largew;
  int realh = largeh;
  lseek(fd, 0x02, SEEK_SET); write(fd, &newsize, 4);
  lseek(fd, 0x12, SEEK_SET); write(fd, &largew, 4); 
  lseek(fd, 0x16, SEEK_SET); write(fd, &largeh, 4); 
  //lseek(fd, 0x0A, SEEK_SET); read(fd, &data_offset1, 4);

  bmp_close( &img_data1 );
  bmp_close( &img_data2 );
  
  return 0;
}                  
