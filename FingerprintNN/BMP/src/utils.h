#pragma once

#include "BMP.h"
#include <algorithm>
#include <random>


namespace utils
{

	// TODO: functions for changing bmp image

	//TODO: may be work with matrix, instead of bmp




	// return value in range [0; 255]
	inline BYTE clamp(int color)
	{
		return color > 255 ? 255 : (color < 0 ? 0 : color);
	}

	// gauss filter core
	double gauss(int x, int y, float sigma = 0.84089642f) { // 0.84089642f | 0.65041f
		return (pow(2.7182818, -((x*x + y * y) / (2 * sigma*sigma))) / (2 * 3.14159*sigma*sigma));
	}




	void grayscale(Matrix<BYTE>* matricesRGB, float R_channel = 0.2126f, float G_channel = 0.7152f, float B_channel = 0.0722f)
	{
		BYTE gray;
		float arr[3] = { R_channel, G_channel, B_channel };

		if (matricesRGB == nullptr)
			return;

		for (int i = 0; i < matricesRGB[0].height(); ++i)
		{
			for (int j = 0; j < matricesRGB[0].width(); ++j)
			{
				gray = 0;
				for (int k = 0; k < 3; ++k)
					gray += matricesRGB[k](j, i) * arr[k];

				for (int k = 0; k < 3; ++k)
					matricesRGB[k](j, i) = gray;
			}
		}
	}

	void negative(Matrix<BYTE>* matrices, uint numOfMatrices = 3)
	{
		if (matrices == nullptr)
			return;

		for (int k = 0; k < numOfMatrices; ++k)
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
						matrices[k](j, i) = 255 - matrices[k](j, i);
				}
			}
	}

	void powerLaw(Matrix<BYTE>* matrices, uint numOfMatrices = 3, float c = 1, float gamma = 1)
	{
		if (matrices == nullptr)
			return;

		for (int k = 0; k < numOfMatrices; ++k)
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					matrices[k](j, i) = clamp(255 * (c * powf(matrices[k](j, i) / 255.0f, gamma)));
				}
			}
	}



	void logTransformation(Matrix<BYTE>* matrices, uint numOfMatrices = 3, BYTE maxColor = 255)
	{
		BYTE color = 0;
		float c = 255.0f / std::log10(1 + int(maxColor));
	
		if (matrices == nullptr)
			return;

		for (int k = 0; k < numOfMatrices; ++k)
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					matrices[k](j, i) = clamp(c * std::log10(1 + int(matrices[k](j, i))));
				}
			}
	}

	void gaussianBlur(Matrix<BYTE>* matrices, uint numOfMatrices = 3, int windowSize = 3, float windowDivisionFactor = -1, float sigma = 0.84089642)
	{

		if (matrices == nullptr)
			return;

		float* window = new float[windowSize * windowSize];
		float sum = 0;
		int half = windowSize / 2;
		for (int i = 0; i < windowSize; ++i)
			for (int j = 0; j < windowSize; ++j)
				window[i * windowSize + j] = gauss(j - half, i - half);
	
	
		if (windowDivisionFactor <= 0)
		{
			windowDivisionFactor = 0;
			for (int i = 0; i < windowSize * windowSize; ++i)
				windowDivisionFactor += window[i];
		}

		Matrix<BYTE>* memory = new Matrix<BYTE>[numOfMatrices];
		for (int i = 0; i < numOfMatrices; ++i)
			memory[i] = Matrix<BYTE>(matrices[i].width(), matrices[i].height());

		
		float color = 0;
		for (int k = 0; k < numOfMatrices; ++k) {
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					color = 0;
					for (int y = -1 * half; y <= half; ++y)
					{
						for (int x = -1 * half; x <= half; ++x)
						{
							if (i + y < 0 || j + x < 0 || i + y >= matrices[k].height() || j + x >= matrices[k].width())
							{

							}
							else
							{
								color += matrices[k](j + x, i + y) * window[(y + half) * windowSize + x + half];
							}
						}
					}
					memory[k](j, i) = color / windowDivisionFactor;
				}
			}
		}
		for (int i = 0; i < numOfMatrices; ++i)
			matrices[i] = memory[i];
		delete[] window;
		delete[] memory;
	}

	// Gaussian noise
	void noise(Matrix<BYTE>* matrices, uint numOfMatrices = 3, float mean = 0.0f, float stddiv = 25, float threshold = 5) 
	{
		if (matrices == nullptr)
			return;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution<float> d(mean, stddiv);
	
		std::uniform_real_distribution<float> distribution(0, 100);
		std::mt19937 engine(rd()); // Mersenne twister MT19937
	
		int rand;
		for (int i = 0; i < matrices[0].height(); ++i) {
			for (int j = 0; j < matrices[0].width(); ++j) {

				if (distribution(engine) <= threshold)
				{
					rand = d(gen);
					for (int k = 0; k < numOfMatrices; ++k)						
						matrices[k](j, i) = clamp(matrices[k](j, i) + rand);
				}
			}
		}
	}



	int compare(const void * a, const void * b)
	{
		return (*(BYTE*)a - *(BYTE*)b);
	}
	
	void medianFilter(Matrix<BYTE>* matrices, uint numOfMatrices = 3, int windowSize = 3)
	{
		if (matrices == nullptr)
			return;

		Matrix<BYTE>* memory = new Matrix<BYTE>[numOfMatrices];
		for (int i = 0; i < numOfMatrices; ++i)
			memory[i] = Matrix<BYTE>(matrices[i].width(), matrices[i].height());
		int half = windowSize / 2;
		BYTE *arr = new BYTE[windowSize * windowSize];
		int it = 0;
	
		for (int k = 0; k < numOfMatrices; ++k)
		{
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					it = 0;
					for (int y = -1 * half; y <= half; ++y)
					{
						for (int x = -1 * half; x <= half; ++x)
						{
							if (j + x < 0 || j + x >= matrices[k].width() || i + y < 0 || i + y >= matrices[k].height())
								arr[it] = 0;
							else
								arr[it] = matrices[k](j + x, i + y);
							++it;
						}
					}
					//std::sort(arr, arr + windowSize * windowSize - 1);
					qsort(arr, windowSize * windowSize, sizeof(BYTE), compare);
					memory[k](j, i) = arr[(windowSize * windowSize) / 2];
				}
			}
		}
		for (int i = 0; i < numOfMatrices; ++i)
			matrices[i] = memory[i];
		delete[] arr;
		delete[] memory;
	}


	Matrix<float>* convolution(const Matrix<BYTE>* matrices, uint numOfMatrices, const float* kernel, uint kernelSize, float divideFactor = -1)
	{
		if (matrices == nullptr)
			return nullptr;
		if (kernelSize & 1 == 0)
			return nullptr;

		Matrix<float>* output = new Matrix<float>[numOfMatrices];

		for (int i = 0; i < numOfMatrices; ++i)
			output[i] = Matrix<float>(matrices[i].width(), matrices[i].height(), 0.0f);


		if (divideFactor <= 0)
		{
			divideFactor = 0;
			for (int i = 0; i < kernelSize * kernelSize; ++i)
				divideFactor += kernel[i];
		}

		int half = kernelSize / 2;
		float sum = 0;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					sum = 0;
					for (int y = -half; y < half; ++y)
					{
						for (int x = -half; x < half; ++x)
						{
							if (i + y < 0 || j + x < 0 || i + y >= matrices[k].height() || j + x >= matrices[k].width())
							{

							}
							else
							{
								sum += matrices[k](j + x, i + y) * kernel[(y + half) * kernelSize + x + half];
							}
						}
					}
					output[i](j, i) = sum / divideFactor;
				}
			}
		}

		return output;
	}


	Matrix<int>* gradient(const Matrix<BYTE>* matrices, uint numOfMatrices = 3)
	{
		
		return nullptr;
	}

	//void grayscale(BMP& bmp, float R_channel = 0.2126f, float G_channel = 0.7152f, float B_channel = 0.0722f)
	//{
	//	BYTE gray;
	//	for (int i = 0; i < bmp.height(); ++i)
	//	{
	//		for (int j = 0; j < bmp.width(); ++j)
	//		{
	//			if (bmp.bitsPerPixel() == 16)
	//			{
	//				rgb555 c(bmp.getPixel(j, i), 16);
	//				gray = c.r * R_channel + c.g * G_channel + c.b * B_channel;
	//				bmp.setPixel(j, i, gray, gray, gray);
	//			}
	//			else if (bmp.bitsPerPixel() > 16)
	//			{
	//				rgba c(bmp.getPixel(j, i), 32);
	//				gray = c.r * R_channel + c.g * G_channel + c.b * B_channel;
	//				bmp.setPixel(j, i, gray, gray, gray, c.a);
	//			}
	//		}
	//	}
	//}
	

	//void negative(BMP& bmp)
	//{
	//	for (int i = 0; i < bmp.height(); ++i)
	//	{
	//		for (int j = 0; j < bmp.width(); ++j)
	//		{
	//			if (bmp.bitsPerPixel() == 16)
	//			{
	//				rgb555 c(bmp.getPixel(j, i), 16);
	//				bmp.setPixel(j, i, 255 - c.r, 255 - c.g, 255 - c.b);
	//			}
	//			else if (bmp.bitsPerPixel() > 16)
	//			{
	//				rgba c(bmp.getPixel(j, i), 32);
	//				bmp.setPixel(j, i, 255 - c.r, 255 - c.g, 255 - c.b, c.a);
	//			}
	//		}
	//	}
	//}


	//void logTransformation(BMP& grayscaleBmp, BYTE maxColor = 255)
	//{
	//	BYTE gray = 0;
	//	float c = 255.0f / std::log10(1 + int(maxColor));
	//
	//	for (int i = 0; i < grayscaleBmp.height(); ++i)
	//	{
	//		for (int j = 0; j < grayscaleBmp.width(); ++j)
	//		{
	//			if (grayscaleBmp.bitsPerPixel() == 16)
	//			{
	//				rgb555 color(grayscaleBmp.getPixel(j, i), 16);
	//				gray = color.r & color.g & color.b;
	//				gray = c * std::log10(1 + int(gray));
	//				grayscaleBmp.setPixel(j, i, gray, gray, gray);
	//			}
	//			else if (grayscaleBmp.bitsPerPixel() > 16)
	//			{
	//				rgba color(grayscaleBmp.getPixel(j, i), 32);
	//				gray = color.r & color.g & color.b;
	//				gray = c * std::log10(1 + int(gray));
	//				grayscaleBmp.setPixel(j, i, gray, gray, gray, color.a);
	//			}
	//		}
	//	}
	//}


	//void gaussianBlur(BMP& bmp, int windowSize = 3, float windowDivisionFactor = -1, float sigma = 0.84089642)
	//{
	//	float* window = new float[windowSize * windowSize];
	//	float sum = 0;
	//	for (int i = 0; i < windowSize; ++i)
	//		for (int j = 0; j < windowSize; ++j)
	//			window[i * windowSize + j] = gauss(j - windowSize / 2, i - windowSize / 2);
	//
	//	BMP newbmp = bmp;
	//	float r, g, b;
	//	BYTE a = 0;
	//
	//	if (windowDivisionFactor < 0)
	//	{
	//		windowDivisionFactor = 0;
	//		for (int i = 0; i < windowSize * windowSize; ++i)
	//			windowDivisionFactor += window[i];
	//	}
	//
	//	for (int i = 0; i < bmp.height(); ++i)
	//	{
	//		for (int j = 0; j < bmp.width(); ++j)
	//		{
	//			r = g = b = 0;
	//			for (int y = -1 * windowSize / 2; y <= windowSize / 2; ++y)
	//			{
	//				for (int x = -1 * windowSize / 2; x <= windowSize / 2; ++x)
	//				{
	//					if (i + y < 0 || j + x < 0 || i + y >= bmp.height() || j + x >= bmp.width())
	//					{
	//
	//					}
	//					else
	//					{
	//						if (bmp.bitsPerPixel() == 16)
	//						{
	//							rgb555 color(bmp.getPixel(j + x, i + y), 16);
	//							r += color.r * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//							g += color.g * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//							b += color.b * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//						}
	//						else if (bmp.bitsPerPixel() > 16)
	//						{
	//							rgba color(bmp.getPixel(j + x, i + y), 32);
	//							r += color.r * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//							g += color.g * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//							b += color.b * window[(y + windowSize / 2) * windowSize + x + windowSize / 2];
	//							a = color.a;
	//						}
	//					}
	//				}
	//			}
	//			r /= windowDivisionFactor;
	//			g /= windowDivisionFactor;
	//			b /= windowDivisionFactor;
	//			newbmp.setPixel(j, i, r, g, b, a);
	//		}
	//	}
	//	bmp = newbmp;
	//
	//	delete[] window;
	//}



	//void noise(BMP &image, float mean = 0.0f, float stddiv = 0.316f, float threshold = 50) { // Gaussian noise
	//
	//	std::random_device rd;
	//	std::mt19937 gen(rd());
	//	std::normal_distribution<float> d(mean, stddiv);
	//
	//	std::uniform_real_distribution<float> distribution(0, 100);
	//	std::mt19937 engine(rd()); // Mersenne twister MT19937
	//
	//
	//	int rand;
	//	for (int i = 0; i < image.height(); ++i) {
	//		for (int j = 0; j < image.width(); ++j) {
	//			//std::cout << distribution(engine) << std::endl;
	//			if (distribution(engine) <= threshold)
	//			{
	//				rand = d(gen);
	//				if (image.bitsPerPixel() == 16)
	//				{
	//					rgb555 c(image.getPixel(j, i), image.bitsPerPixel());
	//					image.setPixel(j, i, clamp(c.r + rand), clamp(c.g + rand), clamp(c.b + rand));
	//				}
	//				else if (image.bitsPerPixel() > 16)
	//				{
	//					rgba c(image.getPixel(j, i), image.bitsPerPixel());
	//					image.setPixel(j, i, clamp(c.r + rand), clamp(c.g + rand), clamp(c.b + rand), c.a);
	//				}
	//			}
	//		}
	//	}
	//}
	

	//int compare(const void * a, const void * b)
	//{
	//	return (*(int*)a - *(int*)b);
	//}
	//
	//void medianFilter(BMP& bmp, int windowSize = 3)
	//{
	//	BMP newbmp = bmp;
	//	DWORD *arr = new DWORD[windowSize * windowSize];
	//	int k = 0;
	//
	//	for (int i = 0; i < bmp.height(); ++i)
	//	{
	//		for (int j = 0; j < bmp.width(); ++j)
	//		{
	//			k = 0;
	//			for (int y = -1 * windowSize / 2; y <= windowSize / 2; ++y)
	//			{
	//				for (int x = -1 * windowSize / 2; x <= windowSize / 2; ++x)
	//				{
	//					if (j + x < 0 || j + x >= bmp.width() || i + y < 0 || i + y >= bmp.height())
	//						arr[k] = 0;
	//					else
	//						arr[k] = bmp.getPixel(j + x, i + y);
	//					++k;
	//				}
	//			}
	//
	//			//std::sort(arr, arr + windowSize * windowSize - 1);
	//			qsort(arr, windowSize * windowSize - 1, sizeof(DWORD), compare);
	//
	//			if (bmp.bitsPerPixel() == 16)
	//			{
	//				rgb555 color((DWORD)arr[windowSize * windowSize / 2], bmp.bitsPerPixel());
	//				newbmp.setPixel(j, i, color.r, color.g, color.b, 0);
	//			}
	//			else if (bmp.bitsPerPixel() > 16)
	//			{
	//				rgba color(arr[windowSize * windowSize / 2], bmp.bitsPerPixel());
	//				newbmp.setPixel(j, i, color.r, color.g, color.b, color.a);
	//			}
	//		}
	//	}
	//	bmp = newbmp;
	//	delete[] arr;
	//}

}