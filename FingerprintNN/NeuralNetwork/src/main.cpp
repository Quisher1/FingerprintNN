#include <iostream>
#include "NeuralNetwork.h"
#include "src/BMP.h"
#include "src/utils.h"
#include "src/math.h"

#include <fstream>



int main()
{
	//try
	//{

		rgb color = rgb::WHITE;//(0, 0, 0);
		//BMP bmp("coins");
		
		BMP bmp("coins");

		utils::grayscale(bmp.at(0));


		float *kernel = new float[9];

		kernel[0] = -1; kernel[1] = 0; kernel[2] = 1;
		kernel[3] = -2; kernel[4] = 0; kernel[5] = 2;
		kernel[6] = -1; kernel[7] = 0; kernel[8] = 1;
		


		Matrix<float>* mat = utils::convolution(bmp.at(0), 1, kernel, 3);


		BMP im(mat[0].width(), mat[0].height(), rgb(0, 0, 0), "default", BMP24);

		for (int i = 0; i < im.height; ++i)
		{
			for (int j = 0; j < im.height; ++j)
			{
				BYTE color = utils::clamp(mat[0](j, i));
				im.setPixel(j, i, color, color, color);
			}
		}

		im.saveAs("image2");

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