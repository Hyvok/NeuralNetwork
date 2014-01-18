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
        // returns the total amount of change in output synapses
        int updateState();

        // Returns number of connections in the network
        int getConnections();

        // Returns weights of the synapses in the network
        std::vector<std::vector<std::vector<int> > > getWeights();

        // Returns input values of the synapses in the network
        std::vector<std::vector<std::vector<int> > > getValues();

        // Returns states of the output synapses in the network
        std::vector<std::vector<int> > getState();

        // Give input to the network, returns true if it is accepted
        bool setInput(std::vector<int> input);

        // Get number of input neurons
        int getInputSize();

        // Get number of output neurons
        int getOutputSize();

        // Get output from the network
        std::vector<int> getOutput();


    private:

        // Creates the neurons, returns number of neurons
        int createNeurons(std::vector<int> nNeuronsPerLayer);

        // Connects network and returns number of connections
        int connectNetwork(int weight);

        std::vector<std::vector<Neuron> > neurons_;
        std::vector<Neuron>* inputNeurons_;
        std::vector<Neuron>* outputNeurons_;
    
        std::vector<int> inputValues_;
        std::vector<int*> outputValues_;

        int nConnections_;
        int nNeurons_;

};

#endif
