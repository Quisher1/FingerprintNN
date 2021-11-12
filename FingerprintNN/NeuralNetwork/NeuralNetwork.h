#pragma once

#include <initializer_list>
#include <memory>

//#include "../../Matrix/Matrix/Math/Matrix.h"


typedef unsigned int uint;
//using namespace Math;

enum ActivationFunc
{

};



class NeuralNetwork
{
public:
	NeuralNetwork(std::initializer_list<uint> layers);

	~NeuralNetwork();

	//void setInput(const matf& input);
	//void setExpectedOutput(const matf& expectedOutput);


	//void feedForward();
	//void backpropagation();

private:

	float sigmoid(float x) { return (1.0f / (1 + pow(2.71828f, -x))); }
	//float relu(float x);
	//float athan(float x);



private:

	uint m_layersCount;
	uint* m_layers = nullptr;

	//TODO: may be change m_neurons from matf to vecf

	//matf* m_neurons = nullptr;
	//matf* m_weights = nullptr;
	//matf* m_biases = nullptr;
	//
	//matf* m_input = nullptr;
	//matf* m_output = nullptr;

};