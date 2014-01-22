#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "NeuralNetworkTrainer.h"
#include "NeuralNetwork.h"

NeuralNetworkTrainer::NeuralNetworkTrainer() :  network_(0), input_(0),
                                                output_(0), nTrainings_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network) : 
                                            network_(&network), input_(0),
                                            output_(0), nTrainings_(0) {}
        
NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input) :
                                            network_(&network), input_(input),
                                            output_(0), nTrainings_(0) 
{

    network_->setInput(input);

}


NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input,
                                            std::vector<float> output) :
                                            network_(&network), input_(input),
                                            output_(output), nTrainings_(0) 
{

    network_->setInput(input);

}


int NeuralNetworkTrainer::trainNetwork()
{

    std::vector<std::vector<float> > deltaOutput(network_->size());
    float error = 0.0;

    // Iterate over the layers in reverse order
    for(size_t nLayer = network_->size()-1; nLayer > 0; --nLayer)
    {
        if(nLayer == network_->size()-1)
        {
            for(size_t nNeuron = 0; nNeuron < (*network_)[nLayer].size(); ++nNeuron)
            {
                deltaOutput[nLayer].push_back(((float)output_[nNeuron] - (*network_)[nLayer][nNeuron].out()));

                //std::cout << "Output error for neuron " << nNeuron << ": " << deltaOutput[nLayer][nNeuron] << "\n";

                    for(size_t nConnectingNeuron = 0; nConnectingNeuron < (*network_)[nLayer-1].size(); ++nConnectingNeuron)
                    {
                        error = (*network_)[nLayer][nNeuron][nConnectingNeuron].weight + (deltaOutput[nLayer][nNeuron] * (*network_)[nLayer-1][nConnectingNeuron].out());
                        //std::cout << "Updated weight for neuron " << nNeuron << ", synapse " << nConnectingNeuron << ": " << error << "\n";
                        (*network_)[nLayer][nNeuron][nConnectingNeuron].newWeight += LEARNING_RATE * error;
                    }
            }
        }
        // Tämä ripulipaskakoodi ei toimi kuin kolmelle kerrokselle
        else if(nLayer != 0)
        {
            error = 0;
            for(size_t nNeuron = 0; nNeuron < (*network_)[nLayer].size(); ++nNeuron)
            {
                for(size_t nConnectingNeuron = 0; nConnectingNeuron < (*network_)[nLayer+1].size(); ++nConnectingNeuron)
                {
                    error += (deltaOutput[nLayer+1][nConnectingNeuron] * (*network_)[nLayer+1][nConnectingNeuron][nNeuron].weight); 
                    deltaOutput[nLayer].push_back(error);
                    //std::cout << "Error for neuron " << nNeuron << ": " << error << "\n";
                    error = 0;
                }
            }
            for(size_t nNeuron = 0; nNeuron < (*network_)[nLayer].size(); ++nNeuron)
            {
                for(size_t nConnectingNeuron = 0; nConnectingNeuron < (*network_)[nLayer-1].size(); ++nConnectingNeuron)
                {
                    error = (*network_)[nLayer][nNeuron][nConnectingNeuron].weight + (deltaOutput[nLayer][nNeuron] * (*network_)[nLayer-1][nConnectingNeuron].out());
                    //std::cout << "Updated weight for neuron " << nNeuron << ", synapse " << nConnectingNeuron << ": " << error << "\n";
                    (*network_)[nLayer][nNeuron][nConnectingNeuron].newWeight += LEARNING_RATE * error;
                }
            }
        }
        else
        {
            //std::cout << "Nothing to do with first layer yet\n";
        }
    }

    int nUpdates = network_->updateWeights();
    network_->updateState();
    ++nTrainings_;

    
    return nUpdates;

}
