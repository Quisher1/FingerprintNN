#pragma once

#include <initializer_list>
#include <memory>

#include "src/Matrix.h"


typedef unsigned int uint;


enum ActivationFunc
{

};



class NeuralNetwork
{
public:
	NeuralNetwork(std::initializer_list<uint> layers);
	NeuralNetwork() = delete;

	~NeuralNetwork();

	void setInput(const matf& input);
	void setExpectedOutput(const matf& expectedOutput);


	void feedForward();
	void backpropagation();


	void randomizeWeightsBiases();



	void infoLog();


	const matf getOutput();

	// Mean Squared Error
	float MSE(const matf& a, const matf& b);
private:





	inline const float sigmoid(float x) const { return (1.0f / (1 + pow(2.71828f, -x))); }
	inline const float derivativedSigmoid(float x) const { return (sigmoid(x) * (1 - sigmoid(x))); }
	//float relu(float x);
	//float athan(float x);



	// TODO: create function for applying any function on any matrix\vector
	// something like a apply(const Matrix& mat, void(float)* func);

	matf sigmoid(const matf& mat);
	matf derivativedSigmoid(const matf& mat);

	matf applyFunction(const matf& mat, float(*func)(float));

	//matf relu(const matf& x);
	//matf athan(const matf& x);

private:

	uint m_layersCount;
	uint* m_layers = nullptr;


	float learningRate = 1;

	//TODO: may be change m_neurons from matf to vecf

	matf* m_deltaNeurons = nullptr;

	matf* m_neurons = nullptr;
	matf* m_weights = nullptr;
	matf* m_biases = nullptr;
	
	matf* m_input = nullptr;
	matf* m_output = nullptr;


	matf m_expextedOutput;
};