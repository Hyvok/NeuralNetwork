#ifndef NEURAL_NETWORK_TRAINER_H
#define NEURAL_NETWORK_TRAINER_H

#include <vector>
#include "NeuralNetwork.h"
#include "Neuron.h"

#define LEARNING_RATE 0.1

class NeuralNetworkTrainer
{

    public:

        NeuralNetworkTrainer();

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network
        NeuralNetworkTrainer(NeuralNetwork& network);

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network with input
        NeuralNetworkTrainer(NeuralNetwork& network, std::vector<float> input);

        // Constructor for constructing a NeuralNetworkTrainer to train 
        // network with input and adjust networks weights based on
        // difference between input and desired output
        NeuralNetworkTrainer(   NeuralNetwork& network, std::vector<float> input,
                                std::vector<float> output);

        // Do one iteration of training the network using back-propagation
        // algorithm, returns number of weights updated
        int trainNetwork();

    private:

        // Function for iterating over a layer and updating the weights for a
        // network with neurons with linear activation function
        void updateWeights( std::vector<Neuron>& layer, 
                            std::vector<Neuron>& prevLayer, int nNeuron, 
                            std::vector<float> errors);

        // Function for calculating the desired weight change for a network
        // with neurons with a linear activation function
        float calculateLinearWeight(float weight, float err, float out);

        NeuralNetwork* nn_;
        Neuron::Type type_;
        std::vector<float> input_;
        std::vector<float> output_;

        int nTrainings_;

};

#endif
