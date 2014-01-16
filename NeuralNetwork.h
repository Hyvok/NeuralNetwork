#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include "Neuron.h"

class NeuralNetwork
{

    public:

        NeuralNetwork();

        // Default constructor, nNeuronsPerLayer specifies how many neurons
        // per layer there is and weight is the default weight for all the 
        // synapses
        NeuralNetwork(std::vector<int> nNeuronsPerLayer, int weight);

        // Calculates the new state of the network, 
        // returns the number of neurons that have fired
        int updateState();

        // Returns number of connections in the network
        int getConnections();

        // Returns weights of the synapses in the network
        std::vector<std::vector<std::vector<int> > > getState();

        std::vector<Neuron>* inputNeurons;
        std::vector<Neuron>* outputNeurons;

    private:

        // Creates the neurons, returns number of neurons
        int createNeurons(std::vector<int> nNeuronsPerLayer);

        // Connects network and returns number of connections
        int connectNetwork(int weight);
        std::vector<std::vector<Neuron> > neurons;

        int _nConnections;
        int _nNeurons;

};

#endif
