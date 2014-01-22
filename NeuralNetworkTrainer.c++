#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "NeuralNetworkTrainer.h"
#include "NeuralNetwork.h"

NeuralNetworkTrainer::NeuralNetworkTrainer() :  nn_(0), input_(0),
                                                output_(0), nTrainings_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network) : 
                                            nn_(&network), input_(0),
                                            output_(0), nTrainings_(0) {}
        
NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input) :
                                            nn_(&network), input_(input),
                                            output_(0), nTrainings_(0) 
{

    nn_->setInput(input);

}


NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input,
                                            std::vector<float> output) :
                                            nn_(&network), input_(input),
                                            output_(output), nTrainings_(0) 
{

    nn_->setInput(input);

}


int NeuralNetworkTrainer::trainNetwork()
{

    float err = 0.0;

    std::vector<std::vector<float> > outputError(nn_->size());

    // Iterate over the layers in reverse order
    for(int nLayer = nn_->size()-1; nLayer > 0; --nLayer)
    {
        if(nLayer == nn_->size()-1)
        {
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                outputError[nLayer].push_back(  (output_[n] - 
                                                (*nn_)[nLayer][n].out()));
                //std::cout << "SKIGULI1\n";

                updateLinearWeights(    (*nn_)[nLayer], (*nn_)[nLayer-1], n,
                                        outputError[nLayer]);
            }
        }
        // TODO: does not work for more layers than 3
        else 
        {
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                for(size_t nPrev = 0; nPrev < (*nn_)[nLayer+1].size(); ++nPrev)
                {
                    err += (    outputError[nLayer+1][nPrev] 
                                * (*nn_)[nLayer+1][nPrev][n].weight); 

                    //std::cout << "Error calculation for neuron " << n << ", previous neuron " << nPrev << "\n";

                }
                outputError[nLayer].push_back(err);
                err = 0;
            }
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                updateLinearWeights(    (*nn_)[nLayer], (*nn_)[nLayer-1], n,
                                        outputError[nLayer]);
            }
        }
    }

    int nUpdates = nn_->updateWeights();
    nn_->updateState();
    ++nTrainings_;

    return nUpdates;

}

// TODO: check if it is worth converting this to inline
float NeuralNetworkTrainer::calculateLinearWeight(  float weight, float err, 
                                                    float out)
{

    return (weight + (err * out));

}

void NeuralNetworkTrainer::updateLinearWeights( std::vector<Neuron>& layer, 
                                                std::vector<Neuron>& prevLayer,
                                                int nNeuron,
                                                std::vector<float> errors)
{
    
    float err = 0;

    for(size_t nPrev = 0; nPrev < prevLayer.size(); ++nPrev)
    {
        err = calculateLinearWeight(    layer[nNeuron][nPrev].weight, 
                                        errors[nNeuron], 
                                        prevLayer[nPrev].out());

        layer[nNeuron][nPrev].newWeight += LEARNING_RATE * err;
    }

}
