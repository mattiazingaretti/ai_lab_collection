#include "image.h"
#include "string.h"
#include <algorithm>
#include <iostream>

// HW0 #1
// const Image& im: input image
// int x,y: pixel coordinates
// int ch: channel of interest
// returns the 0-based location of the pixel value in the data array
int pixel_address(const Image& im, int x, int y, int ch)
  {
    return im.w*im.h*ch + im.w * y +x  ; 
  }


int integer_bound_to(int lb,int ub, int x){
  int def_x = std::max(lb, x);
  return std::min(x , ub);
}

// HW0 #1
// const Image& im: input image
// int x,y,ch: pixel coordinates and channel of interest
// returns the value of the clamped pixel at channel ch
float get_clamped_pixel(const Image& im, int x, int y, int ch){
  int def_x = integer_bound_to(0,im.w-1, x);
  int def_y =  integer_bound_to(0,im.h-1, y); 
  return im( def_x, def_y, ch);
}


// HW0 #1
// Image& im: input image
// int x,y,ch: pixel coordinates and channel of interest
void set_pixel(Image& im, int x, int y, int c, float value){

  if(x >= im.w || x < 0 || y>= im.h || y < 0){ return; }

  im(x,y,c) = value;
  
  return;

}



// HW0 #2
// Copies an image
// Image& to: destination image
// const Image& from: source image
void copy_image(Image& to, const Image& from)
  {

  int size = from.w*from.h*from.c;
  // allocating data for the new image
  to.data=(float*)calloc(size,sizeof(float));
  to.c=from.c;
  to.w = from.w;
  to.h = from.h;

  memcpy(to.data, from.data, size*sizeof(float));

  return;
  
  }
