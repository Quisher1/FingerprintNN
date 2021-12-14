#include <iostream>
#include <algorithm>
#include "NeuralNetwork.h"
#include "src/BMP.h"
#include "src/utils.h"
#include "src/math.h"

#include <fstream>
 
using namespace BMP_lib;

float AVG(const Matrix<float>* mat, uint x, uint y, uint width, uint height)
{
	float avg = 0;
	int count = 0;
	for (int i = x; i < width; ++i)
	{
		for (int j = y; j < height; ++j)
		{
			avg += (*mat)(j, i);
			count++;
		}
	}
	return avg / count;
}


Matrix<float> rotateMatrix(const Matrix<float>* mat, float angle)
{
	Matrix<float> out(mat->width(), mat->height(), 255);
	float x0, y0, _x, _y;
	for (int i = 0; i < mat->height(); ++i) {
		for (int j = 0; j < mat->width(); ++j) {
			x0 = (mat->width() - 1) / 2.f;
			y0 = (mat->height() - 1) / 2.f;
			_x = x0 + (j - x0)*cos(-angle * 3.14159 / 180) - (i - y0)*sin(-angle * 3.14159 / 180);
			_y = y0 + (j - x0)*sin(-angle * 3.14159 / 180) + (i - y0)*cos(-angle * 3.14159 / 180);

			//std::cout << j << " " << i << " | " << _x << " " << _y << std::endl;

			if (_x >= 0 && _x < mat->width() && _y >= 0 && _y < mat->height())
				out(_x, _y) = (*mat)(j, i);
		}
	}


	return out;
}


Matrix<float> subMatrix(const Matrix<float>* mat, uint x, uint y, uint width, uint height)
{
	Matrix<float> out(width, height);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (j + x >= mat->width() || i + y >= mat->height() || j + x < 0 || i + y < 0)
			{ }
			else
				out(j, i) = (*mat)(j + x, i + y);
		}
	}

	return out;
}

Matrix<float> subMatrix(const Matrix<BYTE>* mat, uint x, uint y, uint width, uint height)
{
	Matrix<float> out(width, height);

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			if (j + x >= mat->width() || i + y >= mat->height() || j + x < 0 || i + y < 0)
			{
			}
			else
				out(j, i) = (*mat)(j + x, i + y);
		}
	}

	return out;
}

// vertically
matf concatenateMatrix(matf mat1, matf mat2) {
	if (mat1.width() != mat2.width())
		throw std::runtime_error("Matrix height is different");

	matf out(mat1.width(), mat1.height() + mat2.height());

	for (int i = 0; i < mat1.height(); ++i) {
		for (int j = 0; j < mat1.width(); ++j) {
			out(j, i) = mat1(j, i);
		}
	}
	for (int i = mat1.height(); i < mat1.height() + mat2.height(); ++i) {
		for (int j = 0; j < mat2.width(); ++j) {
			out(j, i) = mat2(j, i - mat2.height());
		}
	}

	return out;
}


std::string intToStr(int k, int size = 4) {
	std::string o = std::to_string(k);
	for (int i = o.size(); i < size; ++i)
		o = '0' + o;
	return o;
}


int main()
{


	//EdgeHandling tests
	/*
	BMP img("image1");
	int windowSize = 3;

	matf* window = new matf(windowSize, windowSize);
	float sum = 0;
	int half = windowSize / 2;
	for (int i = 0; i < windowSize; ++i) {
		for (int j = 0; j < windowSize; ++j) {
			(*window)(j, i) = 1;// utils::gauss(j - half, i - half);
		}
	}


	matf* mm = utils::convolution(img.at(0), 3, *window, utils::MIRROR, -1);

	for (int k = 0; k < 3; ++k)
	{
		for (int i = 0; i < mm[k].height(); ++i)
		{
			for (int j = 0; j < mm[k].width(); ++j)
			{
				(*img.at(k))(j, i) = mm[k](j, i);
			}
		}
	}


	img.saveAs("def2");
	
	return 0;*/


	// DFT tests
	/*BMP img("car");
	utils::grayscale(img.at(0));

	//utils::powerLaw(img.at(0), 3, 2, 4);

	img.saveAs("def1");


	matf* imat = new matf(img.width, img.height);

	for (int i = 0; i < img.height; ++i)
	{
		for (int j = 0; j < img.width; ++j)
		{
			(*imat)(j, i) = (*img.at(0))(j, i);
		}
	}
	
	auto dft = DFT2D(*imat);

	Matrix<BYTE>* i1 = new Matrix<BYTE>(img.width, img.height);
	Matrix<BYTE>* i2 = new Matrix<BYTE>(img.width, img.height);

	for (int i = 0; i < img.height; ++i)
	{
		for (int j = 0; j < img.width; ++j)
		{
			(*i1)(j, i) = utils::clamp(sqrt(dft.first(j, i) * dft.first(j, i) + dft.second(j, i) * dft.second(j, i)) / sqrt(dft.first.width() * dft.first.height()));
			(*i2)(j, i) = utils::clamp(90 * atan2f(dft.second(j, i), dft.first(j, i)));
		}
	}

	//utils::powerLaw(i1, 1, 1, 0.1); //works worse than logTransform
	//utils::powerLaw(i2, 1, 1, 0.1); 

	utils::logTransformation(i1, 1, 255);
	utils::logTransformation(i2, 1, 255);

	BMP(*i1).saveAs("im3");
	BMP(*i2).saveAs("im4");

	auto a = IDFT2D(dft);
	Matrix<BYTE>* inverse = new Matrix<BYTE>(img.width, img.height);
	for (int i = 0; i < img.height; ++i)
	{
		for (int j = 0; j < img.width; ++j)
		{
			(*inverse)(j, i) = a(j, i);
		}
	}
	BMP(*inverse).saveAs("inverse");

	return 0;*/


	// AVG angle of image | gradient | conv | squaredGradVectors
	/*BMP image("n0012");

	utils::grayscale(image.at(0));


	utils::powerLaw(image.at(0), 3, 2, 4);
	//
	image.saveAs("1234");


	//utils::medianFilter(image.at(0));


	Matrix<float> sobelX(3, 3);
	Matrix<float> sobelY(3, 3);

	sobelX(0, 2) = -1; sobelX(1, 2) = 0; sobelX(2, 2) = 1;
	sobelX(0, 1) = -2; sobelX(1, 1) = 0; sobelX(2, 1) = 2;
	sobelX(0, 0) = -1; sobelX(1, 0) = 0; sobelX(2, 0) = 1;
	
	sobelY(0, 2) = 1; sobelY(1, 2) = 2; sobelY(2, 2) = 1;
	sobelY(0, 1) = 0; sobelY(1, 1) = 0; sobelY(2, 1) = 0;
	sobelY(0, 0) = -1; sobelY(1, 0) = -2; sobelY(2, 0) = -1;

	//sobelX(0, 2) = -3; sobelX(1, 2) = 0; sobelX(2, 2) = 3;
	//sobelX(0, 1) = -10; sobelX(1, 1) = 0; sobelX(2, 1) = 10;
	//sobelX(0, 0) = -3; sobelX(1, 0) = 0; sobelX(2, 0) = 3;
	//
	//sobelY(0, 2) = 3; sobelY(1, 2) = 10; sobelY(2, 2) = 3;
	//sobelY(0, 1) = 0; sobelY(1, 1) = 0; sobelY(2, 1) = 0;
	//sobelY(0, 0) = -3; sobelY(1, 0) = -10; sobelY(2, 0) = -3;


	// Image Gradient
	matf* convX = utils::convolution(image.at(0), 1, sobelX);
	matf* convY = utils::convolution(image.at(0), 1, sobelY);

	matf* squaredX = nullptr;
	matf* squaredY = nullptr;
	squaredX = new Matrix<float>(convX->width(), convX->height());
	squaredY = new Matrix<float>(convX->width(), convX->height());


	uint s = 15;
	uint ss =  s / 2;
	//utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::EXTEND,  11);
	utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::MIRROR,  s);


	matf* angles = nullptr;
	angles = new Matrix<float>(convX->width(), convX->height());

	utils::anglesOfSquaredGradients(squaredX, squaredY, angles);

	Matrix<BYTE> mat(convX->width(), convY->height(), 1);

	float avg_angle = 0;

	int count = 0;
	for (int i = ss; i < mat.height() - ss; ++i)
	{
		for (int j = ss; j < mat.width() - ss; ++j)
		{
			//mat(j, i) = sqrt(squaredX->operator()(j, i) * squaredX->operator()(j, i) + squaredX->operator()(j, i) * squaredX->operator()(j, i));
			//mat(j, i) = abs(squaredX->operator()(j, i)) + abs(squaredY->operator()(j, i));
			//mat(j, i) = sqrt(convX->operator()(j, i) * convX->operator()(j, i) + convY->operator()(j, i) * convY->operator()(j, i));
			//mat(j, i) = abs(convX->operator()(j, i)) + abs(convY->operator()(j, i));

			//mat(j, i) = 128 + (((*angles)(j, i) * 180.0f / PI) / 90.0f) * 128;
			avg_angle += ((*angles)(j, i) * 180.0f / PI);
			count++;
			printf("%.2f ", (((*angles)(j, i) * 180.0f / PI)));
		}printf("\n");
	}
	
	std::cout << (avg_angle / float(count)) << std::endl;

	return 0;*/

	
	// rotation test
	/*BMP im("1234");
	
	matf* m = new matf(im.width, im.height);

	for (int i = 0; i < im.height; ++i)
	{
		for (int j = 0; j < im.width; ++j)
		{
			(*m)(j, i) = (*im.at(0))(j, i);
		}
	}

	

	Matrix<BYTE>* mout = new Matrix<BYTE>(im.width, im.height);

	matf r = rotateMatrix(m, 90);


	for (int i = 0; i < im.height; ++i)
	{
		for (int j = 0; j < im.width; ++j)
		{
			(*mout)(j, i) = r(j, i);
		}
	}

	utils::medianFilter(mout, 1);

	BMP(*mout).saveAs("rotated");

	return 0;*/
	

//#define TRAINING

	Matrix<float> sobelX(3, 3);
	Matrix<float> sobelY(3, 3);
	
	sobelX(0, 2) = -1; sobelX(1, 2) = 0; sobelX(2, 2) = 1;
	sobelX(0, 1) = -2; sobelX(1, 1) = 0; sobelX(2, 1) = 2;
	sobelX(0, 0) = -1; sobelX(1, 0) = 0; sobelX(2, 0) = 1;
	
	sobelY(0, 2) = 1; sobelY(1, 2) = 2; sobelY(2, 2) = 1;
	sobelY(0, 1) = 0; sobelY(1, 1) = 0; sobelY(2, 1) = 0;
	sobelY(0, 0) = -1; sobelY(1, 0) = -2; sobelY(2, 0) = -1;

	matf correct(1, 2, 0.0f), wrong(1, 2, 0.0f);
	correct(0, 0) = 0;	correct(0, 1) = 1;
	wrong(0, 0) = 1;	wrong(0, 1) = 0;

#ifdef TRAINING


	uint CorrectSetSize = 400;
	uint WrongSetSize = 700;
	
	
	// Image Gradient
	matf* convX = nullptr;
	matf* convY = nullptr;
	// Squared Gradient
	matf* squaredX = nullptr;
	matf* squaredY = nullptr;
	// Angles of squaderd Gradient
	matf* angles = nullptr;

	// training set preparation
	std::pair<Matrix<float>, Matrix<float>>* input_output = new std::pair<Matrix<float>, Matrix<float>>[(CorrectSetSize + WrongSetSize) * 2 + 2];
	
	std::string path = "learningset/";


	float avg_angle = 0;
	uint WindowSize = 11;
	uint columnsNumberOfDFT = 5;

	matf imageMat;


	for (int it = 1; it <= CorrectSetSize + WrongSetSize; ++it)
	{
		BMP image(path + (it <= CorrectSetSize ? "correct/" : "wrong/") + "orig" + intToStr((it <= CorrectSetSize ? it : it - CorrectSetSize)));
		utils::powerLaw(image.at(0), 3, 2, 4);

		imageMat = matf(image.width, image.height);
		for (int i = 0; i < image.height; ++i)
			for (int j = 0; j < image.width; ++j)
				imageMat(j, i) = (*image.at(0))(j, i) - 128;


		// Image gradient
		convX = utils::convolution(image.at(0), 1, sobelX);
		convY = utils::convolution(image.at(0), 1, sobelY);

		squaredX = new matf(convX->width(), convX->height());
		squaredY = new matf(convY->width(), convY->height());
		// squared
		utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::MIRROR, WindowSize);
		delete[] convX; delete[] convY;
		convX = nullptr; convY = nullptr;

		angles = new matf(image.width, image.height);
		// angle
		utils::anglesOfSquaredGradients(squaredX, squaredY, angles);
		delete squaredX; delete squaredY;
		squaredX = nullptr; squaredY = nullptr;

		// AVG of angles matrix
		avg_angle = AVG(angles, WindowSize / 2, WindowSize / 2, 
								angles->width() - WindowSize / 2, angles->height() - WindowSize / 2);

		delete angles;
		// rotate Matrix
		matf rotated = rotateMatrix(&imageMat, avg_angle);

		// DFT of matrix
		std::pair<matf, matf> dft = DFT2D(rotated);

		// vectorize Vector
		matf vectorMag = matf(1, columnsNumberOfDFT * dft.first.height()),
			vectorPh = matf(1, columnsNumberOfDFT * dft.first.height());

		uint k = 0;
		for (int i = 0; i < image.height; ++i)
			for (int j = image.width/2 - columnsNumberOfDFT; j < image.width/2; ++j)
			{
				vectorMag(0, k) = dft.first(j, i);
				vectorPh(0, k)  = dft.second(j, i);
				k++;
			}

		input_output[it].first = concatenateMatrix(vectorMag, vectorPh);
		input_output[it].second = (it <= CorrectSetSize ? correct : wrong);

		std::cout << it << " " << path + (it <= CorrectSetSize ? "correct/" : "wrong/") + "orig" + intToStr((it <= CorrectSetSize ? it : it - CorrectSetSize)) << std::endl;
	}
	
	for (int it = 1; it <= CorrectSetSize + WrongSetSize; ++it)
	{
		BMP image(path + (it <= CorrectSetSize ? "correct/" : "wrong/") + "orig" + intToStr((it <= CorrectSetSize ? it : it - CorrectSetSize)));
		utils::powerLaw(image.at(0), 3, 2, 4);
		utils::negative(image.at(0));

		imageMat = matf(image.width, image.height);
		for (int i = 0; i < image.height; ++i)
			for (int j = 0; j < image.width; ++j)
				imageMat(j, i) = (*image.at(0))(j, i) - 128;


		// Image gradient
		convX = utils::convolution(image.at(0), 1, sobelX);
		convY = utils::convolution(image.at(0), 1, sobelY);

		squaredX = new matf(convX->width(), convX->height());
		squaredY = new matf(convY->width(), convY->height());
		// squared
		utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::MIRROR, WindowSize);
		delete[] convX; delete[] convY;
		convX = nullptr; convY = nullptr;

		angles = new matf(image.width, image.height);
		// angle
		utils::anglesOfSquaredGradients(squaredX, squaredY, angles);
		delete squaredX; delete squaredY;
		squaredX = nullptr; squaredY = nullptr;

		// AVG of angles matrix
		avg_angle = AVG(angles, WindowSize / 2, WindowSize / 2,
			angles->width() - WindowSize / 2, angles->height() - WindowSize / 2);

		delete angles;
		// rotate Matrix
		matf rotated = rotateMatrix(&imageMat, avg_angle);

		// DFT of matrix
		std::pair<matf, matf> dft = DFT2D(rotated);

		// vectorize Vector
		matf vectorMag = matf(1, columnsNumberOfDFT * dft.first.height()),
			vectorPh = matf(1, columnsNumberOfDFT * dft.first.height());

		uint k = 0;
		for (int i = 0; i < image.height; ++i)
			for (int j = image.width / 2 - columnsNumberOfDFT; j < image.width / 2; ++j)
			{
				vectorMag(0, k) = dft.first(j, i);
				vectorPh(0, k) = dft.second(j, i);
				k++;
			}

		input_output[it + CorrectSetSize + WrongSetSize].first = concatenateMatrix(vectorMag, vectorPh);
		input_output[it + CorrectSetSize + WrongSetSize].second = (it <= CorrectSetSize ? correct : wrong);

		std::cout << it + CorrectSetSize + WrongSetSize << " " << path + (it <= CorrectSetSize ? "correct/" : "wrong/") + "orig" + intToStr((it <= CorrectSetSize ? it : it - CorrectSetSize)) << std::endl;
	}


	NeuralNetwork net({ columnsNumberOfDFT * 32 * 2, 4, 4, 4, 2});

	net.randomizeWeightsBiases();
	net.setLearningrate(0.1f);


	try
	{

		int it = 1;
		for (int i = 0; i < 100000; ++i)
		{
			net.setInput(input_output[it].first);

			net.setExpectedOutput(input_output[it].second);

			net.feedForward();

			net.backpropagation();

			net.feedForward();

			float error = 0;
			for (int j = 0; j < 2; ++j)
				error += 0.5f * powf(net.getOutput()(0, j) - input_output[it].second(0, j), 2);


			std::cout << i << " -  error: " << error << std::endl;

			it++;
			if (it >= 850) it = 1;

			if (i % 10000 == 0)
			{
				std::random_device rd;
				auto rng = std::default_random_engine{ rd() };
				std::shuffle(&input_output[1], &input_output[(CorrectSetSize + WrongSetSize) * 2], rng);
			}
		}

	}
	catch (std::runtime_error &err)
	{
		std::cout << err.what() << std::endl;
	}

	net.save("nnSave");

#else


	std::cout << "correct:\n" << correct << "\nwrong\n" << wrong << std::endl;
	
	try {

		BMP image("00001_01");

		BMP result = image;

		utils::powerLaw(image.at(0), 3, 2, 4);

		image.saveAs("powerLaw");


		NeuralNetwork net;
		net.load("nnSave");

		//BMP image("learningset/wrong/orig" + intToStr(145));


		uint WindowSize = 11;
		uint columnsNumberOfDFT = 5;

		int subImageWidth = 32,
			subImageHeight = 32;

		

		for (int y = 0; y < int(image.height) - subImageHeight / 2; y += 4)
		{
			for (int x = 0; x <  int(image.width) - subImageWidth / 2; x += 4)
			{
				matf imageMat = subMatrix(image.at(0), x, y, subImageWidth, subImageHeight);
				/*matf imageMat = matf(subImageWidth, subImageHeight);
				for (int i = 0; i < subImageHeight; ++i)
					for (int j = 0; j < subImageWidth; ++j)
						imageMat(j, i) = (*image.at(0))(j, i);*/

				Matrix<BYTE> imageByteMat = Matrix<BYTE>(subImageWidth, subImageHeight);
				for (int i = 0; i < subImageHeight; ++i)
					for (int j = 0; j < subImageWidth; ++j)
						imageByteMat(j, i) = imageMat(j, i);


						// Image gradient
				matf* convX = utils::convolution(&imageByteMat, 1, sobelX);
				matf* convY = utils::convolution(&imageByteMat, 1, sobelY);

				matf* squaredX = new matf(convX->width(), convX->height());
				matf* squaredY = new matf(convY->width(), convY->height());
				// squared
				utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::MIRROR, WindowSize);
				delete[] convX; delete[] convY;
				convX = nullptr; convY = nullptr;

				matf* angles = new matf(subImageWidth, subImageHeight);
				// angle
				utils::anglesOfSquaredGradients(squaredX, squaredY, angles);
				delete squaredX; delete squaredY;
				squaredX = nullptr; squaredY = nullptr;

				// AVG of angles matrix
				float avg_angle = AVG(angles, WindowSize / 2, WindowSize / 2,
					angles->width() - WindowSize / 2, angles->height() - WindowSize / 2);

				delete angles;

				// rotate Matrix
				matf rotated = rotateMatrix(&imageMat, avg_angle);

				// DFT of matrix
				std::pair<matf, matf> dft = DFT2D(rotated);

				// vectorize Vector
				matf vectorMag = matf(1, columnsNumberOfDFT * dft.first.height());
				matf vectorPh = matf(1, columnsNumberOfDFT * dft.first.height());

				uint k = 0;
				for (int i = 0; i < subImageHeight; ++i)
					for (int j = subImageWidth / 2 - columnsNumberOfDFT; j < subImageWidth / 2; ++j)
					{
						vectorMag(0, k) = dft.first(j, i);
						vectorPh(0, k) = dft.second(j, i);
						k++;
					}

				matf InputMatrix = concatenateMatrix(vectorMag, vectorPh);


				net.setInput(InputMatrix);

				net.feedForward();

				//std::cout << net.getOutput() << std::endl;


				if (net.getOutput()(0, 1) > net.getOutput()(0, 0) &&
					net.getOutput()(0, 1) > 0.75f)
				{
					result.setPixel(x, y, 0, 0, 255);
				}

			}
		}
		result.saveAs("result");

		/*NeuralNetwork net;
		net.load("nnSave");

		//BMP image("learningset/wrong/orig" + intToStr(145));
		BMP image("testingset/orig" + intToStr(3));


		uint WindowSize = 11;
		uint columnsNumberOfDFT = 3;

		utils::powerLaw(image.at(0), 3, 2, 4);

		matf imageMat = matf(image.width, image.height);
		for (int i = 0; i < image.height; ++i)
			for (int j = 0; j < image.width; ++j)
				imageMat(j, i) = (*image.at(0))(j, i);



		// Image gradient
		matf* convX = utils::convolution(image.at(0), 1, sobelX);
		matf* convY = utils::convolution(image.at(0), 1, sobelY);

		matf* squaredX = new matf(convX->width(), convX->height());
		matf* squaredY = new matf(convY->width(), convY->height());
		// squared
		utils::squaredGradientVectors(convX, convY, squaredX, squaredY, utils::MIRROR, WindowSize);
		delete[] convX; delete[] convY;
		convX = nullptr; convY = nullptr;

		matf* angles = new matf(image.width, image.height);
		// angle
		utils::anglesOfSquaredGradients(squaredX, squaredY, angles);
		delete squaredX; delete squaredY;
		squaredX = nullptr; squaredY = nullptr;

		// AVG of angles matrix
		float avg_angle = AVG(angles, WindowSize / 2, WindowSize / 2,
			angles->width() - WindowSize / 2, angles->height() - WindowSize / 2);

		// rotate Matrix
		matf rotated = rotateMatrix(&imageMat, avg_angle);

		// DFT of matrix
		std::pair<matf, matf> dft = DFT2D(rotated);

		// vectorize Vector
		matf vectorMag = matf(1, columnsNumberOfDFT * dft.first.height());
		matf vectorPh = matf(1, columnsNumberOfDFT * dft.first.height());

		uint k = 0;
		for (int i = 0; i < image.height; ++i)
			for (int j = image.width / 2 - columnsNumberOfDFT; j < image.width / 2; ++j)
			{
				vectorMag(0, k) = dft.first(j, i);
				vectorPh(0, k) = dft.second(j, i);
				k++;
			}

		matf InputMatrix = concatenateMatrix(vectorMag, vectorPh);


		net.setInput(InputMatrix);

		net.feedForward();

		std::cout << net.getOutput() << std::endl;*/
	}
	catch (std::runtime_error &err)
	{
		std::cout << err.what() << std::endl;
	}

#endif // TRAINING


	return 0;
}


// Без негативных изображений получился классификатор линий, на черные он отзывался, на белые нет

// с негативом уже лучше, но все еще худо

// нормализация относительно середины координат (т.е. 0 (т.е. "color - 128")) дало 