#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <vector>
#include <iterator>
#include "Neuron.h"

#define MAX_RANDOM_WEIGHT 10

class NeuralNetwork
{

    friend class iterator;
    friend class NeuralNetworkTrainer;

    public:

        NeuralNetwork();

        // Default constructor, nNeuronsPerLayer specifies how many neurons
        // per layer there is and weight is the default weight for all the
        // synapses. If weight is 0 then the weights will be randomly chosen
        // between 0 and MAX_RANDOM_WEIGHT
        NeuralNetwork(std::vector<int> nNeuronsPerLayer, float weight);

        // Calculates the new state of the network,
        // returns the total amount of change in output synapses
        float updateState();

        // Updates the weights of the network to newWeight,
        // returns the amount of weights updated
        int updateWeights();

        // Returns number of connections in the network
        int getConnections();

        // Returns weights of the synapses in the network
        std::vector<std::vector<std::vector<float> > > getWeights();

        // Returns input values of the synapses in the network
        std::vector<std::vector<std::vector<float> > > getValues();

        // Returns states of the output synapses in the network
        std::vector<std::vector<float> > getState();

        // Give input to the network, returns true if it is accepted
        bool setInput(std::vector<float> input);

        // Get number of input neurons
        int getInputSize();

        // Get number of output neurons
        int getOutputSize();

        // Get output from the network
        std::vector<float> getOutput();

        // Iterators so you can iterate over all the neurons
        class iterator
        {

            public:

                typedef std::forward_iterator_tag iterator_category;
                typedef Neuron value_type;
                typedef Neuron& reference;
                typedef Neuron* pointer;
                //typedef int difference_type;
                iterator(   std::vector<Neuron>::iterator ptr, 
                            NeuralNetwork& owner);
                NeuralNetwork::iterator operator++();
                Neuron& operator*();
                bool operator!=(const NeuralNetwork::iterator& rhs);
                //pointer operator->();
                //bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                //bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }

            private:
                
                NeuralNetwork& owner_;
                std::vector<std::vector<Neuron> >::iterator layer_;
                std::vector<Neuron>::iterator neuron_;

        };

        iterator begin();
        iterator end();

    private:

        // Creates the neurons, returns number of neurons
        int createNeurons(std::vector<int> nNeuronsPerLayer);

        // Connects network and returns number of connections
        int connectNetwork(float weight);

        std::vector<std::vector<Neuron> > neurons_;
        std::vector<Neuron>* inputNeurons_;
        std::vector<Neuron>* outputNeurons_;

        std::vector<float> inputValues_;
        std::vector<float*> outputValues_;

        int nConnections_;
        int nNeurons_;

};

#endif
