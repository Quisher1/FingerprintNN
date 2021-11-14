#include "NeuralNetwork.h"
#include <cstring>
#include <random>

NeuralNetwork::NeuralNetwork(std::initializer_list<uint> layers)
{
	m_layersCount = layers.size();
	m_layers = new uint[m_layersCount];

	std::copy(layers.begin(), layers.end(), m_layers);

	m_deltaNeurons = new matf[m_layersCount];
	m_neurons = new matf[m_layersCount];
	m_weights = new matf[m_layersCount];
	m_biases = new matf[m_layersCount];

	for (int i = 0; i < m_layersCount; ++i)
	{
		m_neurons[i] = matf(1, m_layers[i]);
		m_deltaNeurons[i] = matf(1, m_layers[i]);
	}

	m_weights[0] = matf(0, 0);
	m_biases[0] = matf(0, 0);
	for (int i = 1; i < m_layersCount; ++i)
	{
		m_weights[i] = matf(m_layers[i-1], m_layers[i]);
		m_biases[i] = matf(1, m_layers[i]);
	}

	m_input = &m_neurons[0];
	m_output = &m_neurons[m_layersCount - 1];
}

NeuralNetwork::~NeuralNetwork()
{
	if (m_weights != nullptr)
	{
		delete[] m_deltaNeurons;
		delete[] m_neurons;
		delete[] m_weights;
		delete[] m_biases;
	}
}


void NeuralNetwork::setInput(const matf& input)
{
	if (m_neurons[0].width() != input.width() || m_neurons[0].height() != input.height())
		throw std::runtime_error("input sizes don\'t match");
	m_neurons[0] = input;
}
void NeuralNetwork::setExpectedOutput(const matf& expectedOutput)
{
	m_expextedOutput = expectedOutput;
}


void NeuralNetwork::feedForward()
{
	m_neurons[1] = m_weights[1] * m_neurons[0] + m_biases[1];
	for (int i = 1; i < m_layersCount - 1; ++i)
		m_neurons[i + 1] = m_weights[i + 1] * sigmoid(m_neurons[i]) + m_biases[i + 1];
}

void NeuralNetwork::backpropagation()
{	
	m_deltaNeurons[m_layersCount - 1] = elementwiseMultiplication(sigmoid(m_neurons[m_layersCount - 1]) - m_expextedOutput, derivativedSigmoid(m_neurons[m_layersCount - 1]));
	m_weights[m_layersCount - 1] -= learningRate * m_deltaNeurons[m_layersCount - 1] * sigmoid(m_neurons[m_layersCount - 2]).transpose();;
	m_biases[m_layersCount - 1] -= learningRate * m_deltaNeurons[m_layersCount - 1];
	
	
	for (int i = m_layersCount - 2; i > 0; --i)
	{
		m_deltaNeurons[i] = elementwiseMultiplication(m_weights[i + 1].transpose() * m_deltaNeurons[i + 1], derivativedSigmoid(m_neurons[i]));
		m_weights[i] -= learningRate * m_deltaNeurons[i] * sigmoid(m_neurons[i - 1]).transpose();
		m_biases[i] -= learningRate * m_deltaNeurons[i];
	}
}

void NeuralNetwork::randomizeWeightsBiases()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1, 1);
	for (int i = 0; i < m_layersCount; ++i)
	{
		for (int y = 0; y < m_weights[i].height(); ++y)
			for (int x = 0; x < m_weights[i].width(); ++x)
				m_weights[i](x, y) = dis(gen);

		for (int y = 0; y < m_biases[i].height(); ++y)
			for (int x = 0; x < m_biases[i].width(); ++x)
				m_biases[i](x, y) = dis(gen);
	}
}




void NeuralNetwork::infoLog()
{
	for (int i = 1; i < m_layersCount; ++i)
	{
		//std::cout << "Neurons: \n" << m_neurons[i] << std::endl;
		std::cout << "Weights: \n" << m_weights[i] << std::endl;
		std::cout << "Biases: \n" << m_biases[i] << std::endl;
		std::cout << "==============" << std::endl;
	}
	//std::cout << "Neurons: \n" << m_neurons[m_layersCount - 1] << std::endl;
}


const matf NeuralNetwork::getOutput()
{
	return sigmoid(m_neurons[m_layersCount - 1]);
}


//////////////////////


float NeuralNetwork::MSE(const matf& a, const matf& b)
{
	if(a.width() != b.width() ||
		a.height() != b.height())
		throw std::runtime_error("cannot calculate MSE. matrix sizes don\'t match");

	float out = 0;
	for (int i = 0; i < a.height(); ++i)
		for (int j = 0; j < a.width(); ++j)
			out += (a(j, i) - b(j, i)) * (a(j, i) - b(j, i));

	return out / 2.0f;
}



matf NeuralNetwork::sigmoid(const matf& mat)
{
	matf out(mat.width(), mat.height());
	for (int y = 0; y < mat.height(); ++y)
		for (int x = 0; x < mat.width(); ++x)
			out(x, y) = sigmoid(mat(x, y));
	return out;
}
matf NeuralNetwork::derivativedSigmoid(const matf& mat)
{
	matf out(mat.width(), mat.height());
	for (int y = 0; y < mat.height(); ++y)
		for (int x = 0; x < mat.width(); ++x)
			out(x, y) = derivativedSigmoid(mat(x, y));
	return out;
}


matf NeuralNetwork::applyFunction(const matf& mat, float(*func)(float))
{
	matf out(mat.width(), mat.height());
	for (int y = 0; y < mat.height(); ++y)
		for (int x = 0; x < mat.width(); ++x)
			out(x, y) = func(mat(x, y));
	return out;
}