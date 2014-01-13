#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include "Neuron.h"

class NeuralNetwork
{

    public:

        NeuralNetwork();
        NeuralNetwork(std::vector<int> nNeuronsPerLayer);

        // Calculates the new state of the network, 
        // returns the number of weights updated
        int updateState();

        // Returns number of connections in the network
        int getConnections();

        std::vector<Neuron>* inputNeurons;
        std::vector<Neuron>* outputNeurons;

    private:

        // Creates the NeuralNetwork, returns number of neurons
        int createNetwork(std::vector<int> nNeuronsPerLayer);

        // Connects network and returns number of connections between neurons
        int connectNetwork();
        std::vector<std::vector<Neuron> > neurons;

        int _nConnections;
        int _nNeurons;

};

#endif
