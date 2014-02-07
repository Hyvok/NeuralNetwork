#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <iterator>
#include "Neuron.h"

#define MAX_RANDOM_WEIGHT 10

class NeuralNetwork
{

    friend class NeuralNetworkTrainer;

    public:

        // Constructs an empty network with no neurons
        NeuralNetwork();

        // Default constructor, nNeuronsPerLayer specifies how many neurons
        // per layer there is and weight is the default weight for all the
        // synapses. If weight is 0 then the weights will be randomly chosen
        // between -MAX_RANDOM_WEIGHT/2 and MAX_RANDOM_WEIGHT/2, neuron type
        // defaults to TYPE_SIGMOID
        NeuralNetwork(std::vector<unsigned int> nNeuronsPerLayer, float weight);

        // Constructor, nNeuronsPerLayer specifies how many neurons
        // per layer there is and weight is the default weight for all the
        // synapses. If weight is 0 then the weights will be randomly chosen
        // between -MAX_RANDOM_WEIGHT/2 and MAX_RANDOM_WEIGHT/2, type specifies
        // the type of neuron (TYPE_LINEAR or TYPE_SIGMOID)
        NeuralNetwork(  std::vector<unsigned int> nNeuronsPerLayer, 
                        float weight, Neuron::Type type);

        // Constructors without weight option, weight defaults to random values
        NeuralNetwork(std::vector<unsigned int> nNeuronsPerLayer);

        NeuralNetwork(  std::vector<unsigned int> nNeuronsPerLayer, 
                        Neuron::Type type);

        // Calculates the new state of the network,
        // returns the total amount of change in output synapses
        float updateState();

        // Updates the weights of the network to newWeight,
        // returns the amount of weights updated
        unsigned int updateWeights();

        // Returns number of connections in the network
        unsigned int getConnections() const;

        // Returns weights of the synapses in the network
        std::vector<std::vector<std::vector<float> > > getWeights() const;

        // Returns input values of the synapses in the network
        std::vector<std::vector<std::vector<float> > > getValues() const;

        // Returns states of the output synapses in the network
        std::vector<std::vector<float> > getState() const;

        // Give input to the network and updateState(),
        // returns true if it is accepted
        bool setInput(std::vector<float> input);

        // Get number of input neurons
        unsigned int getInputSize() const;

        // Get number of output neurons
        unsigned int getOutputSize() const;

        // Get output from the network
        std::vector<float> getOutput() const;

        // Operator [] for accessing the layers of the neural network
        std::vector<Neuron>& operator[](size_t nLayer);

        // Return how many layers the network has
        size_t size() const;

        // Return Type of the neurons in the network
        Neuron::Type getNeuronType() const;

        // Return last layer of the network
        std::vector<Neuron>& back();

    private:

        // Creates the neurons, returns number of neurons
        unsigned int createNeurons( std::vector<unsigned int> nNeuronsPerLayer, 
                                    Neuron::Type type);

        // Connects network and returns number of connections
        unsigned int connectNetwork(float weight);

        std::vector<std::vector<Neuron> > neurons_;
        std::vector<Neuron>* inputNeurons_;
        std::vector<Neuron>* outputNeurons_;

        std::vector<float> inputValues_;
        std::vector<float*> outputValues_;

        unsigned int nConnections_;
        unsigned int nNeurons_;
        Neuron::Type type_;

};

#endif
