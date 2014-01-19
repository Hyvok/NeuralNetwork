#include <iostream>
#include <boost/log/trivial.hpp>
#include "NeuralNetwork.h"

NeuralNetworkTrainer::NeuralNetworkTrainer() :  network_(0), intput_(0),
                                                output_(0), nTrainings_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork network) : 
                                            network_(network), intput_(0),
                                            output_(0), nTrainings(0) {}
        
NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork network, 
                                            std::vector<int> input) :
                                            network_(network), input_(input),
                                            output_(0), nTrainings_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork network, 
                                            std::vector<int> input,
                                            std::vector<int> output) :
                                            network_(network), input_(input),
                                            output_(output), nTrainings_(0) {}

NeuralNetworkTrainer::trainNetwork()
{

    int error = 0;

}
