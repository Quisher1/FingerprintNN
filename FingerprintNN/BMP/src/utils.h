#pragma once

#include "BMP.h"

// TODO: functions for changing bmp image


void grayscale(BMP& bmp, float R_channel = 0.2126f, float G_channel = 0.7152f, float B_channel = 0.0722f)
{
	BYTE gray;
	for (int i = 0; i < bmp.height(); ++i)
	{
		for (int j = 0; j < bmp.width(); ++j)
		{
			if (bmp.bitsPerPixel() == 16)
			{
				rgb555 c(bmp.getPixel(j, i), 16);
				gray = c.r * R_channel + c.g * G_channel + c.b * B_channel;
				bmp.setPixel(j, i, gray, gray, gray);
			}
			else if(bmp.bitsPerPixel() > 16)
			{
				rgba c(bmp.getPixel(j, i), 32);
				gray = c.r * R_channel + c.g * G_channel + c.b * B_channel;
				bmp.setPixel(j, i, gray, gray, gray, c.a);
			}
		}
	}
}

void noise(BMP& bmp, float mean = 0.f, float standardDeviation = 3.16f)
{


}

void negative(BMP& bmp)
{
	for (int i = 0; i < bmp.height(); ++i)
	{
		for (int j = 0; j < bmp.width(); ++j)
		{
			if (bmp.bitsPerPixel() == 16)
			{
				rgb555 c(bmp.getPixel(j, i), 16);
				bmp.setPixel(j, i, 255 - c.r, 255 - c.g, 255 - c.b);
			}
			else if (bmp.bitsPerPixel() > 16)
			{
				rgba c(bmp.getPixel(j, i), 32);
				bmp.setPixel(j, i, 255 - c.r, 255 - c.g, 255 - c.b, c.a);
			}
		}
	}
}

void logTransformation(BMP& bmp)
{

}

void applyFilter(BMP& bmp, const float* kernel, float scale = 1, int stride = 1)
{

}

void gaussianBlur(BMP& bmp, int width, int height, float sigma = 0.84089642)
{

}