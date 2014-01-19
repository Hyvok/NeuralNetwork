#ifndef NEURAL_NETWORK_TRAINER_H
#define NEURAL_NETWORK_TRAINER_H

#include <vector>
#include "NeuralNetwork.h"

#define LEARNING_RATE 0.01

class NeuralNetworkTrainer
{

    public:

        NeuralNetworkTrainer();

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network
        NeuralNetworkTrainer(NeuralNetwork network);

        // Constructor for constructing a NeuralNetworkTrainer to train
        // network with input
        NeuralNetworkTrainer(NeuralNetwork network, std::vector<int> input);

        // Constructor for constructing a NeuralNetworkTrainer to train 
        // network with input and adjust networks weights based on
        // difference between input and desired output
        NeuralNetworkTrainer(   NeuralNetwork network, std::vector<int> input,
                                std::vector<int> output);

        // Do one iteration of training the network
        trainNetwork();

    private:

        NeuralNetwork network_;
        int (*scoreResult_)(std::vector<int> input, std::vector<int> output);
        std::vector<int> input_;
        std::vector<int> output_;

        int nTrainings_;

};

#endif
