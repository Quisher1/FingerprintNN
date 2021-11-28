#include <iostream>
#include "NeuralNetwork.h"
#include "src/BMP.h"
#include "src/utils.h"
#include "src/math.h"

#include <fstream>
 

//DFT

#define PI 3.14159265

static std::pair<matf, matf> DFT1D(const matf& data) {
	
	std::pair<matf, matf> Real_Imaginary;

	Real_Imaginary.first = matf(data.width(), data.height());
	Real_Imaginary.second = matf(data.width(), data.height());

	uint N = data.width();
	uint M = data.height();

	float Resum, Imsum;

	for (int i = 0; i < M; ++i) 
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += pow(-1, n) * data(n, i) * std::cosf(-(2 * PI * j * n) / N);
				Imsum += pow(-1, n) * data(n, i) * std::sinf(-(2 * PI * j * n) / N);
			}
			Real_Imaginary.first(j, i)  = Resum / N;
			Real_Imaginary.second(j, i) = Imsum / N;
		}
	}
	return Real_Imaginary;
}


static std::pair<matf, matf> DFT2D(const matf& data) {
	std::pair<matf, matf> Real_Imaginary_1D = DFT1D(data);

	std::pair<matf, matf> Real_Imaginary_2D;
	Real_Imaginary_2D.first = matf(data.width(), data.height());
	Real_Imaginary_2D.second = matf(data.width(), data.height());

	uint N = data.width();
	uint M = data.height();

	float Resum, Imsum;
	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < M; ++i)
		{
			Resum = Imsum = 0;
			for (int m = 0; m < M; ++m)
			{
				Resum += pow(-1, m) * (Real_Imaginary_1D.first(j, m) * std::cosf( (2 * PI * i * m) / M) - Real_Imaginary_1D.second(j, m) * std::sinf(-(2 * PI * i * m) / M));
				Imsum += pow(-1, m) * (Real_Imaginary_1D.first(j, m) * std::sinf(-(2 * PI * i * m) / M) + Real_Imaginary_1D.second(j, m) * std::cosf( (2 * PI * i * m) / M));
			}
			Real_Imaginary_2D.first(j, i)  = Resum / M;
			Real_Imaginary_2D.second(j, i) = Imsum / M;
		}
	}
	return Real_Imaginary_2D;
}

static matf IDFT1D(const std::pair<matf, matf>& data) {
	matf result = matf(data.first.width(), data.first.height());

	uint N = result.width();
	uint M = result.height();

	float Resum, Imsum;

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += data.first(n, i) * cosf(2 * PI * j * n / N) - data.second(n, i) * sinf(2 * PI * j * n / N);
				Imsum += data.first(n, i) * sinf(2 * PI * j * n / N) + data.second(n, i) * cosf(2 * PI * j * n / N);
			}
			result(j, i) = pow(-1, j) * (Resum + Imsum);
		}
	}
	return result;
}
static matf IDFT2D(const std::pair<matf, matf>& data) {
	matf result = matf(data.first.width(), data.first.height());

	uint N = result.width();
	uint M = result.height();

	float Resum, Imsum;

	
	std::pair<matf, matf> inverse_Real_Imaginary_1D;
	inverse_Real_Imaginary_1D.first = matf(data.first.width(), data.first.height());
	inverse_Real_Imaginary_1D.second = matf(data.first.width(), data.first.height());
	// inverse 1D calculations
	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			Resum = Imsum = 0;
			for (int n = 0; n < N; ++n)
			{
				Resum += data.first(n, i) * cosf(2 * PI * j * n / N) - data.second(n, i) * sinf(2 * PI * j * n / N);
				Imsum += data.first(n, i) * sinf(2 * PI * j * n / N) + data.second(n, i) * cosf(2 * PI * j * n / N);
			}
			inverse_Real_Imaginary_1D.first(j, i)  = Resum;
			inverse_Real_Imaginary_1D.second(j, i) = Imsum;
		}
	}
	////

	for (int j = 0; j < N; ++j)
	{
		for (int i = 0; i < M; ++i)
		{
			Resum = Imsum = 0;
			for (int m = 0; m < M; ++m)
			{
				Resum += inverse_Real_Imaginary_1D.first(j, m) * cosf(2 * PI * i * m / M) - inverse_Real_Imaginary_1D.second(j, m) * sinf(2 * PI * i * m / M);
				Imsum += inverse_Real_Imaginary_1D.first(j, m) * sinf(2 * PI * i * m / M) + inverse_Real_Imaginary_1D.second(j, m) * cosf(2 * PI * i * m / M);
			}
			result(j, i) = pow(-1, j + i) * (Resum + Imsum);
		}
	}
	return result;
}

int main()
{
	//try
	//{



	//std::cout << InverseFourier2D(Fourier2D(mat)) << std::endl;


		//rgb color = rgb::WHITE;//(0, 0, 0);
		//
		//BMP bmp1("1");
		//
		//BMP bmp2("2");
		//utils::gaussianBlur(res.at(0));
		//utils::medianFilter(res.at(0));

		
		//utils::gaussianBlur(res.at(0));


		//utils::difference(bmp2.at(0), bmp1.at(0));


		//(*bmp2.at(0)) *= 1;

		//for (int i = 0; i < bmp2.at(0)->height(); ++i)
		//	for (int j = 0; j < bmp2.at(0)->width(); ++j) {
		//		(*bmp2.at(0))(j, i) = utils::clamp((*bmp2.at(0))(j, i) * 1000);
		//		(*bmp2.at(1))(j, i) = utils::clamp((*bmp2.at(1))(j, i) * 1000);
		//		(*bmp2.at(2))(j, i) = utils::clamp((*bmp2.at(2))(j, i) * 1000);
		//	}


		//(*bmp2.at(1)) *= 0;
		//(*bmp2.at(2)) *= 0;


		//utils::medianFilter(bmp2.at(0), 3, 5);

		//utils::add(bmp1.at(0), bmp2.at(0));
		


		//bmp1.saveAs("image1");
		//bmp2.saveAs("image2");



		//utils::grayscale(bmp.at(0));

		//matf kernel(3, 3);
		//kernel(0, 0) = -1; kernel(1, 0) = 0; kernel(2, 0) = 1;
		//kernel(0, 1) = -2; kernel(1, 1) = 0; kernel(2, 1) = 2;
		//kernel(0, 2) = -1; kernel(1, 2) = 0; kernel(2, 2) = 1;
		//




		//Matrix<float>* mat = utils::convolution(bmp.at(0), 1, kernel, 1020);
		//kernel = kernel.transpose();
		//Matrix<float>* mat1 = utils::convolution(bmp.at(0), 1, kernel, 1020);


		//BMP res(mat[0].width(), mat[0].height(), rgb(0, 0, 0), "default", BMP24);

		//for (int i = 0; i < res.height; ++i)
		//{
		//	for (int j = 0; j < res.width; ++j)
		//	{
		//		BYTE color = utils::clamp((abs(mat[0](j, i)) + abs(mat1[0](j, i))) * 255 * 5);
		//		res.setPixel(j, i, color, color, color);
		//	}
		//}

		////utils::negative(res.at(0));
		////utils::medianFilter(res.at(0));

		//utils::gaussianBlur(res.at(0));

		//bmp.saveAs("image2");

		//bmp.saveAs("image2");



		//Matrix<BYTE>& matR = *bmp.at(0);
		//Matrix<BYTE>& matG = *bmp.at(1);
		//Matrix<BYTE>& matB = *bmp.at(2);


		//DWORD color;
		//for (int i = 0; i < bmp.height; ++i)
		//{
		//	for (int j = 0; j < bmp.width; ++j)
		//	{
		//		color = matR(j, i)*0.2126f + matG(j, i)*0.7152f + matB(j, i)*0.0722f;

		//		//std::cout << (int)matR(j, i) << " " << (int)matG(j, i) << " " << (int)matG(j, i) << std::endl;

		//		matR(j, i) = utils::clamp(2 * matR(j, i));
		//		matG(j, i) = utils::clamp(2 * matG(j, i));
		//		matB(j, i) = utils::clamp(2 * matB(j, i));

		//		//std::cout << (int)matR(j, i) << " " << (int)matG(j, i) << " " << (int)matG(j, i) << std::endl << std::endl;
		//	}
		//}

		//utils::powerLaw(bmp.at(0), 3, 1, 2.1);

		//bmp.saveAs("image2");

	//}
	//catch (std::runtime_error& err)
	//{
	//	std::cout << err.what() << std::endl;
	//}


	/*BMP image("vegetables");
	grayscale(image);
	noise(image, 0, 30, 10);
	image.saveAs("coinsNoise");
	medianFilter(image, 3);

	image.saveAs("coinsMedian");*/








	/*NeuralNetwork net({2, 2, 1});
	net.randomizeWeightsBiases();

	matf*inp = new matf[4];
	matf*out = new matf[4];

	inp[0] = matf(1, 2);
	inp[1] = matf(1, 2);
	inp[2] = matf(1, 2);
	inp[3] = matf(1, 2);

	out[0] = matf(1, 1);
	out[1] = matf(1, 1);
	out[2] = matf(1, 1);
	out[3] = matf(1, 1);


	inp[0](0, 0) = 0; inp[0](0, 1) = 0;
	inp[1](0, 0) = 0; inp[1](0, 1) = 1;
	inp[2](0, 0) = 1; inp[2](0, 1) = 0;
	inp[3](0, 0) = 1; inp[3](0, 1) = 1;

	out[0](0, 0) = 0;
	out[1](0, 0) = 1;
	out[2](0, 0) = 1;
	out[3](0, 0) = 0;



	net.infoLog();


	int k = 0;
	net.setInput(inp[k]);
	net.setExpectedOutput(out[k]);


	net.feedForward();
	std::cout << net.getOutput() << std::endl;


	for (int i = 0; i < 50000; ++i)
	{
		net.setInput(inp[k]);
		net.setExpectedOutput(out[k]);

		net.feedForward();

		net.backpropagation();
		net.feedForward();
		std::cout << net.getOutput();

		k++;
		if (k == 4)
		{
			k = 0;
			std::cout << std::endl;
		}
	}

	net.save("test");


	net.infoLog();*/

	return 0;
}