#include "NeuralNetwork.h"
#include <cstring>

NeuralNetwork::NeuralNetwork(std::initializer_list<uint> layers)
{
	m_layersCount = layers.size();
	m_layers = new uint[m_layersCount];

	std::copy(layers.begin(), layers.end(), m_layers);

	//m_neurons = new matf[m_layersCount];
	//m_weights = new matf[m_layersCount - 1];
	//m_biases = new matf[m_layersCount - 1];

	for (int i = 0; i < m_layersCount - 1; ++i)
	{
		//m_neurons[i] = matf(1, m_layers[i]);
		//m_weights[i] = matf(m_layers[i], m_layers[i + 1]);
		//m_biases[i] = matf(1, m_layers[i+1]);
	}



	for (int i = 0; i < m_layersCount; ++i)
	{
		//std::cout << m_neurons[i].width() << " " << m_neurons[i].height() << std::endl;
	}
}

NeuralNetwork::~NeuralNetwork()
{
	//delete[] m_neurons;
	//delete[] m_weights;
	//delete[] m_biases;
}


//void NeuralNetwork::setInput(const matf& input)
//{
//	// TODO: input data copy
//	//m_weights[0];
//}
//void NeuralNetwork::setExpectedOutput(const matf& expectedOutput)
//{
//	// TODO: expectedOutput data copy
//	//m_matrixLayers[m_layersCount - 2];
//}