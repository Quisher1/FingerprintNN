#pragma once

#include <initializer_list>
#include <memory>

#include "src/Matrix.h"


typedef unsigned int uint;

//TODO: activation func enum
enum ActivationFunc
{

};



class NeuralNetwork
{
public:
	NeuralNetwork(std::initializer_list<uint> layers);
	//NeuralNetwork() = delete;
	NeuralNetwork() {}
	~NeuralNetwork();

	void setInput(const matf& input);
	void setExpectedOutput(const matf& expectedOutput);


	void feedForward();
	void backpropagation();


	void randomizeWeightsBiases();
	void setLearningrate(float learning_rate);


	void infoLog();



	void save(const char* filename);
	void load(const char* filename);


	const matf getOutput();

private:
	// Mean Squared Error
	float MSE(const matf& a, const matf& b);


	inline const float sigmoid(float x) const { return (1.0f / (1 + pow(2.71828f, -x))); }
	inline const float derivativedSigmoid(float x) const { return (sigmoid(x) * (1 - sigmoid(x))); }
	
#define E 2.71828
	inline const float tanh(float x) const { return (powf(E, x) - powf(E, -x)) / (powf(E, x) + powf(E, -x)); }
	inline const float derivativedTanh(float x) const { return (1 - tanh(x) * tanh(x)); }
	
	inline const float relu(float x) const { return (x > 0 ? x : 0); }
	inline const float derivativedRelu(float x) const { return (x > 0 ? 1 : 0); }

	inline const float leakyrelu(float x) const { return (x > 0 ? x : 0.01f * x); }
	inline const float derivativeLeakyRelu(float x) const { return (x > 0 ? 1 : 0.01f); }

	inline const float sinF(float x) const { return sin(x); }
	inline const float derivativeSin(float x) const { return cos(x); }

	//float relu(float x);
	//float athan(float x);



	// TODO: create function for applying any function on any matrix\vector
	// something like a apply(const Matrix<T>& mat, void(float)* func);
	//					apply(const Vector<T>& mat, void(float)* func);

	// TODO: add additional activation functions
	// TODO: create pointer to used activation func  void(float)


	matf sigmoid(const matf& mat);
	matf derivativedSigmoid(const matf& mat);

	//matf relu(const matf& x);
	//matf athan(const matf& x);

private:
	//float m_error;


	uint m_layersCount = 0;
	uint* m_layers = nullptr;


	float m_learningRate = 0.1;

	//TODO: may be change m_neurons from matf to vecf

	matf* m_deltaNeurons = nullptr;

	matf* m_neurons = nullptr;
	matf* m_weights = nullptr;
	matf* m_biases = nullptr;

	matf* m_input = nullptr;
	matf* m_output = nullptr;


	matf m_expextedOutput;
};