#include <iostream>
#include "src/NeuralNetwork.h"
#include "src/BMP.h"
#include "src/utils.h"

int main()
{

	/*BMP image("forest");

	grayscale(image);

	image.saveAs("newImage1");

	logTransformation(image);


	image.saveAs("newImage2");*/


	BMP image("vegetables");




	gaussianBlur(image, 3);

	image.saveAs("newImage");



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


	for (int i = 0; i < 500000; ++i)
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


	net.infoLog();*/

	return 0;
}