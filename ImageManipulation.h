/********************************************************************************
This is a library for manipulating images already located in RAM.

WARNING: These algorithms operate on the microcontroller's RAM buffer
			   in order to apply changes, so a fair amount of RAM (at least as big 
				 as the image buffer itself) is required to avoid crashes.


Written in 2020 by Leopoldo Perizzolo: find me @ Deep Tronix:
https://rebrand.ly/deeptronix
As long as you retain this notice you can do whatever you want with this stuff. 
If we meet some day, and you think this stuff is worth it, 
you can buy me a beer in return. ( https://en.wikipedia.org/wiki/Beerware ).
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
