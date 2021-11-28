#pragma once

#include "BMP.h"
#include <algorithm>
#include <random>


namespace utils
{
	

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


	Matrix<float>* convolution(const Matrix<BYTE>* matrices, uint numOfMatrices, const matf& kernel, float divideFactor = -1)
	{
		if (matrices == nullptr)
			return nullptr;
		if (kernel.width() != kernel.height() || (kernel.width() & 1 == 0 && kernel.height() & 1 == 0))
			return nullptr;

		uint kernelSize = kernel.width();


		Matrix<float>* output = new Matrix<float>[numOfMatrices];


		if (divideFactor <= 0)
		{
			divideFactor = 0;
			for (int i = 0; i < kernelSize; ++i)
				for (int j = 0; j < kernelSize; ++j)
					divideFactor += kernel(j, i);
		}

		int half = kernelSize / 2;
		float sum = 0;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			output[k] = Matrix<float>(matrices[k].width(), matrices[k].height(), 0.0f);
			for (int i = 0; i < matrices[k].height(); ++i)
			{
				for (int j = 0; j < matrices[k].width(); ++j)
				{
					sum = 0;
					for (int y = -1 * half; y <= half; ++y)
					{
						for (int x = -1 * half; x <= half; ++x)
						{
							if (i + y < 0 || j + x < 0 || i + y >= matrices[k].height() || j + x >= matrices[k].width())
							{}
							else
							{
								sum += matrices[k](j + x, i + y) * kernel(x + half, y + half);
							}
						}
					}
					output[k](j, i) = sum / divideFactor;
				}
			}
		}

		return output;
	}


	enum SubSampling
	{
		AVG,
		MAX
	};

	
	// TODO: create subsampling function
	/*Matrix<BYTE>* subSampling(const Matrix<BYTE>* matrices, uint numOfMatrices = 3, uint step = 1, uint size = 2, SubSampling type = SubSampling::MAX)
	{
		if (matrices == nullptr)
			return nullptr;

		Matrix<BYTE>* output = new Matrix<BYTE>[numOfMatrices];

		BYTE max = 0;
		uint AVG = 0;


		int oi = 0, oj = 0;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			output[k] = Matrix<BYTE>(matrices[k].width() / size, 1);
			for (int i = 0; i < matrices[k].height(); i += step)
			{
				oj = 0;
				for (int j = 0; j < matrices[k].width(); j += step)
				{
					max = 0;
					AVG = 0;
					for (int y = i; y < i + size; ++y)
					{
						for (int x = j; x < j + size; ++x)
						{
							if (matrices[k](j, i) > max)
								max = matrices[k](j, i);
							AVG += matrices[k](j, i) / (size * size);
						}
					}
					output[k](oj, oi) = type == SubSampling::MAX ? max : AVG;
					oj++;
				}
				oi++;
			}
		}

		return output;
	}*/


	/*Matrix<int>* gradientX(const Matrix<BYTE>* matrices, uint numOfMatrices = 3)
	{
		
		return nullptr;
	}*/

	/*Matrix<int>* gradientY(const Matrix<BYTE>* matrices, uint numOfMatrices = 3)
	{

		return nullptr;
	}*/


	// abs difference
	void difference(Matrix<BYTE>* matricesA, const Matrix<BYTE>* matricesB, uint numOfMatrices = 3, float threshold = 0)
	{
		if (matricesA == nullptr || matricesB == nullptr)
			return;
		if (matricesA->width() != matricesB->width() || matricesA->height() != matricesB->height())
			return;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			for (int i = 0; i < matricesA[k].height(); ++i)
			{
				for (int j = 0; j < matricesA[k].width(); ++j)
				{
					if(abs(matricesA[k](j, i) * 100 / 255 - matricesB[k](j, i) * 100 / 255) < threshold)
						matricesA[k](j, i) = 0;
					else
						matricesA[k](j, i) = clamp(abs((int)matricesA[k](j, i) - (int)matricesB[k](j, i)));

				}
			}
		}
	}

	void substract(Matrix<BYTE>* matricesA, const Matrix<BYTE>* matricesB, uint numOfMatrices = 3)
	{
		if (matricesA == nullptr || matricesB == nullptr)
			return;
		if (matricesA->width() != matricesB->width() || matricesA->height() != matricesB->height())
			return;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			for (int i = 0; i < matricesA[k].height(); ++i)
			{
				for (int j = 0; j < matricesA[k].width(); ++j)
				{
					matricesA[k](j, i) = clamp(matricesA[k](j, i) - matricesB[k](j, i));
				}
			}
		}
	}
	void add(Matrix<BYTE>* matricesA, const Matrix<BYTE>* matricesB, uint numOfMatrices = 3)
	{
		if (matricesA == nullptr || matricesB == nullptr)
			return;
		if (matricesA->width() != matricesB->width() || matricesA->height() != matricesB->height())
			return;

		for (int k = 0; k < numOfMatrices; ++k)
		{
			for (int i = 0; i < matricesA[k].height(); ++i)
			{
				for (int j = 0; j < matricesA[k].width(); ++j)
				{
					matricesA[k](j, i) = clamp(matricesA[k](j, i) + matricesB[k](j, i));
				}
			}
		}
	}




	// TODO: 2D matrix to 1D matrix / vector




}