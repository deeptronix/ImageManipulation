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

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define _mod_bitwise(x, mod)   (x & (mod - 1))

class ImgManip{
 public:
	ImgManip();
	
	void gray256To8bits(uint8_t *img, uint16_t img_width, uint16_t img_height, bool clear_EOB = false);
	void gray256To8bits(uint8_t *img, uint16_t img_width, uint16_t img_height, uint8_t *out_bf, uint8_t size_factor);
	void bool8bitsTo8Bytes(uint8_t cluster, uint8_t* out_values);
	uint8_t byteTo8Bits(uint8_t* source);
	
	uint16_t getNewDimension(uint16_t dimension, uint16_t mul, uint16_t div);
	uint16_t getNewWidth();
	uint16_t getNewHeight();
	int8_t scale(uint8_t* img, uint16_t w_st, uint16_t h_st, uint16_t mul, uint16_t div);
	int8_t scale(uint16_t* img, uint16_t w_st, uint16_t h_st, uint16_t mul, uint16_t div);
	void rotate90deg(uint8_t* img, uint8_t* output, uint16_t w_st, uint16_t h_st);
	int8_t offsetAndCrop(uint8_t* img, uint16_t in_w, uint16_t in_h, uint16_t out_w, uint16_t out_h, uint16_t x_offs, uint16_t y_offs);
	
	void Brightness(uint8_t *img, uint16_t img_width, uint16_t img_height, float factor);
	void Contrast(uint8_t *img, uint16_t img_width, uint16_t img_height, float factor);
	
	
	
 private:
	uint8_t _clamp(int16_t v, uint8_t min, uint8_t max);
	uint16_t _new_w, _new_h;		// to be updated whenever scale() is called, and then asked with the relative functions
	
	
	
};
