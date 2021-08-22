/********************************************************************************
This is a library for manipulating images already located in RAM.

WARNING: These algorithms operate on the microcontroller's RAM buffer
			   in order to apply changes, so a fair amount of RAM (at least as big 
				 as the image buffer itself) is required to avoid crashes.


Copyright (c) 2021 Leopoldo Perizzolo - Deep Tronix
Find me @ https://rebrand.ly/deeptronix

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#include <stdlib.h>
#include "ImageManipulation.h"

ImgManip::ImgManip(){
};


void ImgManip::gray256To8bits(uint8_t *img, uint16_t img_width, uint16_t img_height,	// image width MUST be a multiple of 8.
																			bool clear_EOB){	// clear end of the (original image) buffer
  uint8_t source, destination = 0;
  uint16_t ind_byte = 0;
  for(uint16_t row = 0; row < img_height; row++){
    for(uint16_t col = 0; col < img_width; col++){
      
      if(!(col % 8)  &&  (col > 0  ||  row > 0)){	// every 8 pixels from the original image store the new value as a single 8-bit section in the same array
        img[ind_byte++] = destination;
        destination = 0;
      }
      source = img[col + row * img_width];
      destination |= ((source & 0x80) >> _mod_bitwise(col, 8));
    }
  }
  if(clear_EOB){
  	for(uint32_t p = ind_byte + 1; p < (img_height * img_width); p++){
  		img[p] = 0;
  	}
  }
}

void ImgManip::gray256To8bits(uint8_t *img, uint16_t img_width, uint16_t img_height, 	// image width MUST be a multiple of 8.
																			uint8_t *out_bf, uint8_t size_factor){
  if((size_factor & (size_factor - 1)) != 0)  return;  // halt if size_factor is not a POWER of 2
  uint8_t source, destination = 0;
  uint16_t ind_byte = 0;
  for(uint16_t row = 0; row < (img_height * size_factor); row++){
    for(uint16_t col = 0; col < (img_width * size_factor); col++){
      
      if(!(col % 8)  &&  (col > 0  ||  row > 0)){
        out_bf[ind_byte++] = destination;
        destination = 0;
      }
      source = img[(col/size_factor) + (row/size_factor) * img_width];
      destination |= ((source & 0x80) >> _mod_bitwise(col, 8));
    }
  }
}


void ImgManip::bool8bitsTo8Bytes(uint8_t cluster, uint8_t* out_values){
  out_values[0] = ~((int8_t)(((cluster >> 7) & 0x01) - 1));
  out_values[1] = ~((int8_t)(((cluster >> 6) & 0x01) - 1));
  out_values[2] = ~((int8_t)(((cluster >> 5) & 0x01) - 1));
  out_values[3] = ~((int8_t)(((cluster >> 4) & 0x01) - 1));
  out_values[4] = ~((int8_t)(((cluster >> 3) & 0x01) - 1));
  out_values[5] = ~((int8_t)(((cluster >> 2) & 0x01) - 1));
  out_values[6] = ~((int8_t)(((cluster >> 1) & 0x01) - 1));
  out_values[7] = ~((int8_t)((cluster & 0x01) - 1));
}

uint8_t ImgManip::byteTo8Bits(uint8_t* source){
	uint8_t output = 0;
	output = (source[0] & 0x80) | ((source[1] >> 1) & 0x40) | ((source[2] >> 2) & 0x20) | ((source[3] >> 3) & 0x10) | 
		((source[4] >> 4) & 0x08) | ((source[5] >> 5) & 0x04) | ((source[6] >> 6) & 0x02) | ((source[7] >> 7) & 0x01);
	return output;
}
	


uint16_t ImgManip::getNewDimension(uint16_t dimension, uint16_t mul, uint16_t div){
	uint32_t out_dim = dimension;		// expand to 32 bits, so that the next mul doesn't overflow
	out_dim = (out_dim*mul)/div;
	return out_dim;
}

uint16_t ImgManip::getNewWidth(){
	return _new_w;
}

uint16_t ImgManip::getNewHeight(){
	return _new_h;
}


int8_t ImgManip::scale(uint8_t* img, uint16_t w_st, uint16_t h_st, 
																			uint16_t mul, uint16_t div){
	
	uint16_t* img16 = (uint16_t*)img;
	return scale(img16, w_st, h_st, mul, div);
}

int8_t ImgManip::scale(uint16_t* img, uint16_t w_st, uint16_t h_st, 
																			uint16_t mul, uint16_t div){
  uint16_t pixel;
  uint32_t index = 0, orig_ind = 0;
  uint16_t x_orig, y_orig;

  uint16_t out_w = (w_st*mul) / div;
  uint16_t out_h = (h_st*mul) / div;
  
  for(uint32_t y = 0; y < out_h; y++){
    for(uint16_t x = 0; x < out_w; x++){

      x_orig = round((float)(x*div)/mul);
      y_orig = round((float)(y*div)/mul);
      orig_ind = x_orig + y_orig*w_st;
      
      pixel = img[orig_ind];   // read pixel value

      index = x + y*out_w;
      img[index] = pixel;     // replace pixel
    }
  }
  
  // update private variables:
  _new_w = out_w;
  _new_h = out_h;

  return (((index + 1) == (out_w*out_h)) - 1);		// check whether the last index corresponds to the estimated index
}


void ImgManip::rotate90deg(uint8_t* img, uint8_t* output, uint16_t w_st, uint16_t h_st){
  // w_st is the largest of the two dimensions; assuming a starting landscape orientation.

  const uint16_t out_w = h_st;   // alias to avoid confusion later on
  const uint16_t out_h = w_st;   // alias to avoid confusion later on
  uint8_t pixel;
  uint32_t inp_ind = 0, out_ind = 0;
  
  // Run along the OUTPUT image frame...
  for(uint32_t y = 0; y < out_h; y++){
    for(uint16_t x = 0; x < out_w; x++){
      // ...while reading the INPUT buffer in the rotated order
      inp_ind = x + y*out_w;
      pixel = img[inp_ind];

      // locate output index, then assign pixel value
      out_ind = y + (h_st - x - 1)*w_st;
      output[out_ind] = pixel;
    }
  }
  
}


int8_t ImgManip::offsetAndCrop(uint8_t* img, uint16_t in_w, uint16_t in_h,
                    											  uint16_t out_w, uint16_t out_h, uint16_t x_offs, uint16_t y_offs){

  uint8_t pixel;
  uint32_t inp_ind = 0, out_ind = 0;
  
  for(uint32_t y = y_offs; y < (y_offs + out_h); y++){
    for(uint16_t x = x_offs; x < (x_offs + out_w); x++){
      inp_ind = x + y*in_w;
      pixel = img[inp_ind];

      img[out_ind++] = pixel;
    }
  }
  
  return ((out_ind == (out_w*out_h)) - 1);		// check whether the last index corresponds to the estimated index
}


void ImgManip::Brightness(uint8_t *img, uint16_t img_width, uint16_t img_height, 
													float factor){		// factor = [-1.0 : 1.0]
																						// factor = -1: every pixel black ; factor = 0: original pixels ; factor = 1: every pixel white
	int16_t outp;
	uint32_t ind;
	uint8_t source;
	for(uint16_t row = 0; row < img_height; row++){
    for(uint16_t col = 0; col < img_width; col++){
    	ind = col + row * img_width;
    	source = img[ind];
    	outp = source + factor * 255.0;
    	img[ind] = _clamp(outp, 0, 255);
    }
  }
}

void ImgManip::Contrast(uint8_t *img, uint16_t img_width, uint16_t img_height, 
												float factor){		// factor = [-1.0 : 1.0]
																					// factor = -1: every pixel gray ; factor = 0: original pixels ; factor = 1: full thresholding effect (everything either black or white)
	int16_t outp;
	uint32_t ind;
	uint8_t source;
	factor++;
	for(uint16_t row = 0; row < img_height; row++){
    for(uint16_t col = 0; col < img_width; col++){
    	ind = col + row * img_width;
    	source = img[ind];
    	outp = (source - 127) * factor;
    	img[ind] = _clamp(127 + outp, 0, 255);
    }
  }
}






uint8_t ImgManip::_clamp(int16_t v, uint8_t min, uint8_t max){
  uint16_t t = v < min ? min : v;
  return t > max ? max : t;
}
	
