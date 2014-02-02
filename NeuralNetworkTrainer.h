#ifndef NEURAL_NETWORK_TRAINER_H
#define NEURAL_NETWORK_TRAINER_H

#include <vector>
#include "NnImageMap.h"
#include "NeuralNetwork.h"
#include "Neuron.h"

// TODO: implement validation set support
class NeuralNetworkTrainer
{

    public:

        NeuralNetworkTrainer();

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network
        NeuralNetworkTrainer(NeuralNetwork& network, float learningRate);

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network with input
        NeuralNetworkTrainer(   NeuralNetwork& network, 
                                std::vector<float> input,
                                float learningRate);

        // Constructor for constructing a NeuralNetworkTrainer to train 
        // network with input and adjust networks weights based on
        // difference between input and desired output
        NeuralNetworkTrainer(   NeuralNetwork& network, 
                                std::vector<float> input,
                                std::vector<float> output, float learningRate);

        // Constructor for constructing a NeuralNetworkTrainer to train network
        // with input contained in imageMap
        NeuralNetworkTrainer(   NeuralNetwork& network, NnImageMap& imageMap, 
                                float learningRate);

        // Do one iteration of training the network using back-propagation
        // algorithm, returns number of weights updated
        int trainNetwork();

        // Set learning rate, defaults to LEARNING_RATE if not explicitly set
        // TODO: fix this to be specified in the constructor
        void setLearningRate(float rate);

    private:

        // Function for iterating over a layer and updating the weights for a
        // network with neurons with linear activation function
        void updateWeights( std::vector<Neuron>& layer, 
                            std::vector<Neuron>& prevLayer, int nNeuron, 
                            std::vector<float> errors);

        // Function for calculating the desired weight change for a network
        // with neurons with a linear activation function
        float calculateWeight(float weight, float err, float out);

        NeuralNetwork* nn_;
        Neuron::Type type_;
        NnImageMap* imageMap_;
        // TODO: Get rid of these
        std::vector<float> input_;
        std::vector<float> output_;

        int nTrainings_;

        float learningRate_;

};

#endif
