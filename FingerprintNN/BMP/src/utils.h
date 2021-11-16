#pragma once

#include "BMP.h"
#include <algorithm>

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

void logTransformation(BMP& grayscaleBmp, BYTE maxColor = 255)
{
	BYTE gray = 0;
	float c = 255.0f / std::log10(1 + int(maxColor));
	
	for (int i = 0; i < grayscaleBmp.height(); ++i)
	{
		for (int j = 0; j < grayscaleBmp.width(); ++j)
		{
			if (grayscaleBmp.bitsPerPixel() == 16)
			{
				rgb555 color(grayscaleBmp.getPixel(j, i), 16);
				gray = color.r & color.g & color.b;
				gray = c * std::log10(1 + int(gray));
				grayscaleBmp.setPixel(j, i, gray, gray, gray);
			}
			else if (grayscaleBmp.bitsPerPixel() > 16)
			{
				rgba color(grayscaleBmp.getPixel(j, i), 32);
				gray = color.r & color.g & color.b;
				gray = c * std::log10(1 + int(gray));
				grayscaleBmp.setPixel(j, i, gray, gray, gray, color.a);
			}
		}
	}
}


// TODO: may be make function BMP.getPixel return only rgba color in range[0; 255], instead of DWORD. 


double gauss(int x, int y, float sigma = 0.84089642f) { // 0.84089642f | 0.65041f
	return (pow(2.7182818, -((x*x + y * y) / (2 * sigma*sigma))) / (2 * 3.14159*sigma*sigma));
}

void gaussianBlur(BMP& bmp, int windowSize, float windowDivisionFactor = -1, float sigma = 0.84089642)
{
	float* window = new float[windowSize * windowSize];
	float sum = 0;
	for (int i = 0; i < windowSize; ++i)
		for (int j = 0; j < windowSize; ++j)
			window[i * windowSize + j] = gauss(j - windowSize / 2, i - windowSize / 2);

	BMP newbmp = bmp;
	float r, g, b;
	BYTE a = 0;

	if (windowDivisionFactor < 0)
	{
		windowDivisionFactor = 0;
		for (int i = 0; i < windowSize * windowSize; ++i)
			windowDivisionFactor += window[i];
	}
	
	for (int i = 0; i < bmp.height(); ++i)
	{
		for (int j = 0; j < bmp.width(); ++j)
		{
			r = g = b = 0;
			for (int y = -1 * windowSize / 2; y <= windowSize / 2; ++y)
			{
				for (int x = -1 * windowSize / 2; x <= windowSize / 2; ++x)
				{
					if (i + y < 0 || j + x < 0 || i + y >= bmp.height() || j + x >= bmp.width())
					{

					}
					else
					{
						if (bmp.bitsPerPixel() == 16)
						{
							rgb555 color(bmp.getPixel(j + x, i + y), 16);
							r += color.r * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
							g += color.g * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
							b += color.b * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
						}
						else if (bmp.bitsPerPixel() > 16)
						{
							rgba color(bmp.getPixel(j + x, i + y), 32);
							r += color.r * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
							g += color.g * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
							b += color.b * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
							a = color.a;
						}
					}
				}
			}
			r /= windowDivisionFactor;
			g /= windowDivisionFactor;
			b /= windowDivisionFactor;
			newbmp.setPixel(j, i, r, g, b, a);
		}
	}
	bmp = newbmp;

	delete[] window;
}
