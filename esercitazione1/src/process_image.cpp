#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <iostream>
#include "string.h"
#include <math.h>

#include "image.h"

using namespace std;



// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im){

  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  int area = im.w*im.h;

  float* r = (float*)calloc(area,sizeof(float));
  memcpy(r, im.data, area*sizeof(float));

  float* g = (float*)calloc(area,sizeof(float));
  memcpy(g, &im.data[area], area*sizeof(float));

  float* b = (float*)calloc(area,sizeof(float));
  memcpy(b, &im.data[2*area], area*sizeof(float));

  for( int i = 0; i < area; i++){
    gray.data[i] = 0.299*r[i] + 0.587*g[i] + 0.114*b[i];
  }

  return gray;
}



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }


float float_bound_to(float lb,float ub, float x){
  float def_x = std::max(lb, x);
  return std::min(x , ub);
}

// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int c, float v){

  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  int size = im.w*im.h;

  float* new_channel = (float*) calloc(size, sizeof(float));

  float* channel_img = (float*) calloc(size, sizeof(float));

  memcpy(channel_img, &im.data[size*c] , size*sizeof(float));

  for(int i = 0; i < size; i++){
    new_channel[i] = float_bound_to(0.0, 1.0 , v) + channel_img[i];
  }

  memcpy (&im.data[size*c] ,new_channel, size*sizeof(float));
  
}

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  int size = im.w*im.h;

  float* new_channel = (float*) calloc(size, sizeof(float));

  float* channel_img = (float*) calloc(size, sizeof(float));

  memcpy(channel_img, &im.data[size*c] , size*sizeof(float));

  for(int i = 0; i < size; i++){
    new_channel[i] =  v * channel_img[i];
  }

  memcpy (&im.data[size*c] ,new_channel, size*sizeof(float));
  
  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  
  for(int i = 0; i < im.w*im.h *im.c ; i++){
    im.data[i] = float_bound_to(0.0,1.0, im.data[i] );
  }
  
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im){

  assert(im.c==3 && "only works for 3-channels images");
  
  int size = im.w*im.h;

  for(int i = 0; i < size ; i++){
    float r = im.data[i];
    float g = im.data[i+size];
    float b = im.data[i+2*size];
    
    float Value = max(r,g,b);

    float minimum = min(r,g,b);
    float C = Value - minimum;

    float Saturation = r == 0 && g == 0 && b == 0  ? 0 : C / Value;

    float Hue = 0.0; 
    if(C != 0){
      
      float Hue_prime = 0;
      if(Value == r)
        Hue_prime = (g - b)/C;
      if(Value == g)
        Hue_prime = (b-r)/C + 2;
      if(Value == b)
        Hue_prime = (r-g)/C + 4;
      
      Hue = Hue_prime < 0 ? Hue_prime/6.0 + 1 : Hue_prime/6.0;
    }

    //update pixel values;
    im.data[i] = Hue;
    im.data[i+size] = Saturation;
    im.data[i+2*size] = Value;
  }


}


// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  

  int size = im.w*im.h;

  for(int i = 0; i < size ; i++){
    float h = im.data[i];
    float s = im.data[i+size];
    float v = im.data[i+2*size];

    float C = v*s;
    float X = C * (1-abs( fmod(6*h , 2.0)-1) );
    float m = v - C;

    float R_prime = 0;
    float G_prime = 0;
    float B_prime = 0;
  
    if(h < 1.0/6.0 && h>=0){
      R_prime = C;
      G_prime = X;
      B_prime = 0.0;
    }

    if (h < 2.0/6.0 && h>=1.0/6.0){
      R_prime = X;
      G_prime = C;
      B_prime = 0.0;
    
    }
    if( h < 30./6.0 && h>=2.0/6.0){
      R_prime = 0.0;
      G_prime = C;
      B_prime = X;
    }

    if (h < 4.0/6.0 && h>=3.0/6.0){
      R_prime = 0;
      G_prime = X;
      B_prime = C; 
    }
    if (h < 5.0/6.0 && h>=4.0/6.0){
      R_prime = X;
      G_prime = 0.0;
      B_prime = C;
    }
    if( h < 1.0 && h>=5.0/6.0){ 
      R_prime = C;
      G_prime = 0.0;
      B_prime = X;
    }
     
    
    im.data[i] = R_prime + m;
    im.data[i+size] = G_prime + m;
    im.data[i+2*size] = B_prime + m;
  

  }    
  
} 

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  
  
  NOT_IMPLEMENTED();
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  NOT_IMPLEMENTED();
  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
