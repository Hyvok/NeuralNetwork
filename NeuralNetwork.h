#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include "Neuron.h"

class NeuralNetwork
{

    public:

        NeuralNetwork();
        NeuralNetwork(	int nInputNeurons, int nOutputNeurons, 
                        int nHiddenNeurons);

        // Calculates the new state of the network, 
        // returns the number of weights updated
        int updateState();

        std::vector<Neuron*> inputNeurons;
        std::vector<Neuron*> outputNeurons;

    private:
        // Connects network and returns number of connections between neurons
        int connectNetwork();
        std::vector<std::vector<Neuron> > neurons;

};

#endif
