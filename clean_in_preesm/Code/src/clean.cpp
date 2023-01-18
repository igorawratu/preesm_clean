#include "clean.h"

#include <algorithm>
#include <limits>
#include <assert.h>

void FindPeak(std::uint32_t img_width, std::uint32_t img_height, float* img, std::uint32_t& peak_x, std::uint32_t& peak_y, float& peak_val){
	assert(img_width > 0 && img_height > 0);

	peak_val = -std::numeric_limits<float>::max();

	for(std::uint32_t y = 0; y < img_width; ++y){
		for(std::uint32_t x = 0; x < img_width; ++x){
			curr_idx = x + y * img_height; //might want to change this to a morton code hash or something for better cache coherency
			if(peak_val < img[curr_idx]){
				peak_val = img[curr_idx];
				peak_x = x;
				peak_y = y;
			}
		}
	}
}

void AddToModel(std::uint32_t model_width, std::uint32_t model_height, float* model, std::uint32_t x, std::uint32_t y, float& peak_val){
	assert(x < model_width && y < model_height);

	model[x + y * model_width] += peak_val;
}

//can subtract beam directly due to convolution with dirac
void SubtractBeamAt(std::uint32_t img_width, std::uint32_t img_height, float* img, std::uint32_t beam_width, std::uint32_t beam_height, float* beam,
		std::uint32_t x, std::uint32_t y, float intensity, float gain){
	assert(x < img_width && y < img_height);

	std::uint32_t beam_center_x = beam_width / 2;
	std::uint32_t beam_center_y = beam_height / 2;

	std::uint32_t beam_start_x = std::uint32_t(std::max(int(beam_center_x) - int(x), 0));
	std::uint32_t beam_start_y = std::uint32_t(std::max(int(beam_center_y) - int(y), 0));
	std::uint32_t beam_end_x = beam_center_x + std::uint32_t(std::min(int(beam_center_x + 1), int(img_width - x)));
	std::uint32_t beam_end_y = beam_center_y + std::uint32_t(std::min(int(beam_center_y + 1), int(img_height - x)));

	std::uint32_t img_start_x = x - (beam_width - beam_start_x);
	std::uint32_t img_start_y = y - (beam_height - beam_start_y);

	for(std:uint32_t cy = beam_start_y; cy < beam_end_y; ++cy){
		for(std::uint32_t cx = beam_start_x; cx < beam_end_x; ++cx){
			std::uint32_t beam_idx = cx + cy * beam_width;
			std::uint32_t img_idx = cx + img_start_x + (cy + img_start_y) * img_width;

			img[img_idx] -= beam[beam_idx] * gain * intensity;
		}
	}
}

void Convolve2D(std::uint32_t img_width, std::uint32_t img_height, float* img, std::uint32_t kernel_width, std::uint32_t kernel_height, float* kernel){
	assert(img_width > 0 && img_height > 0 && kernel_width > 0 && kernel_height > 0);

	img_out = new float[img_width * img_height];

	for(std::uint32_t y = 0; y < img_height; ++y){
		for(std::uint32_t x = 0; x < img_width; ++x){
			std::uint32_t kernel_center_x = kernel_width / 2;
			std::uint32_t kernel_center_y = kernel_height / 2;
			std::uint32_t img_out_idx = x + y * img_width;

			img_out[img_out_idx] = 0.f;

			for(std::uint32_t ky = kernel_height - 1; ky >= 0; --ky){
				for(std::uint32_t kx = kernel_width - 1; kx >= 0; --kx){
					int img_idx_x = x + kernel_center_x - kx;
					int img_idx_y = y + kernel_center_y - ky;

					if(img_idx_x >= 0 && img_idx_x < img_width && img_idx_y >= 0 && img_idx_y < img_height){
						std::uint32_t img_idx = img_idx_x + img_idx_y * img_width;
						kernel_idx = kx + ky * kernel_width;
						img_out[img_out_idx] += img[img_idx] * kernel[kernel_idx];
					}
				}
			}
		}
	}
}

void DisplayImage(std::uint32_t img_width, std::uint32_t img_height, float* img){

}
