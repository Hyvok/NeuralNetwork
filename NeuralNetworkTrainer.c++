#include <iostream>
// rand()
#include <stdlib.h>
#include <time.h>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "NnImageMap.h"
#include "NeuralNetworkTrainer.h"
#include "NeuralNetwork.h"

NeuralNetworkTrainer::NeuralNetworkTrainer() :  nn_(0), imageMap_(0), input_(0),
                                                output_(0), nTrainings_(0), 
                                                learningRate_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network,
                                            float learningRate) : 
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), imageMap_(0),
                                            input_(0), output_(0), 
                                            nTrainings_(0),
                                            learningRate_(learningRate) {}
        
NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input,
                                            float learningRate) :
                                            nn_(&network),
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(0), input_(input), 
                                            output_(0), nTrainings_(0),
                                            learningRate_(learningRate)
{

    nn_->setInput(input);
    srand(time(NULL));

}


NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input,
                                            std::vector<float> output,
                                            float learningRate) :
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(0), input_(input), 
                                            output_(output), nTrainings_(0),
                                            learningRate_(learningRate)
{

    nn_->setInput(input);
    srand(time(NULL));

}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network,
                                            NnImageMap& imageMap,
                                            float learningRate) :
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(&imageMap), input_(0), 
                                            output_(0), nTrainings_(0),
                                            learningRate_(learningRate)
{

    srand(time(NULL));

}

// TODO: split into smaller parts
unsigned int NeuralNetworkTrainer::trainNetwork()
{

    float err = 0.0;
    float outErr = 0.0;

    // Select random training case from imageMap_ and set input of the network 
    // to match it
    size_t nTrainingCase = rand() % (*imageMap_).size();
    nn_->setInput((*imageMap_)[nTrainingCase].in());

    std::vector<std::vector<float> > outputError(nn_->layers());

    // Iterate over the layers in reverse order
    for(unsigned int nLayer = nn_->layers()-1; nLayer > 0; --nLayer)
    {
        // Iterate through neurons in the layer
        for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
        {
            // If we are in the last (=output) layer
            if(nLayer == nn_->layers() - 1)
            {
                if(type_ == Neuron::Type::TYPE_LINEAR)
                {
                    outErr =    ((*imageMap_)[nTrainingCase].out()[n] - 
                                (*nn_)[nLayer][n].out());
                    outputError[nLayer].push_back(outErr);

                    updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer - 1], n,
                            outputError[nLayer]);
                }
                // TYPE_SIGMOID, default
                else
                {
                    outErr =    (*nn_)[nLayer][n].out() * ((1.0 - 
                                (*nn_)[nLayer][n].out()) * 
                                ((*imageMap_)[nTrainingCase].out()[n] - 
                                (*nn_)[nLayer][n].out()));
                    outputError[nLayer].push_back(outErr);

                    updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer - 1], n,
                                    outputError[nLayer]);
                }
            }
            else
            {
                // For the other layers we need to go through all the neurons
                // in the previous layer for the error calculation
                for(size_t nPrev = 0; nPrev < (*nn_)[nLayer+ 1].size(); ++nPrev)
                {
                    if(type_ == Neuron::Type::TYPE_LINEAR)
                    {
                        err += (    outputError[nLayer + 1][nPrev] 
                                    * (*nn_)[nLayer + 1][nPrev][n].weight); 

                    }
                    // TYPE_SIGMOID
                    else
                    {
                        err +=  (*nn_)[nLayer][n].out() * 
                                (1 - (*nn_)[nLayer][n].out()) * 
                                (outputError[nLayer + 1][nPrev] 
                                * (*nn_)[nLayer + 1][nPrev][n].weight); 
                    }
                }
                outputError[nLayer].push_back(err);
                err = 0;
                updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer - 1], n,
                                outputError[nLayer]);
            }
        }
    }

    unsigned int nUpdates = nn_->updateWeights();
    ++nTrainings_;

    return nUpdates;

}

// TODO: check if it is worth converting this to inline
float NeuralNetworkTrainer::calculateWeight(    float const& weight, 
                                                float const& err, 
                                                float const& out)
{

    return (err * out);

}

void NeuralNetworkTrainer::updateWeights(   std::vector<Neuron>& layer, 
                                            std::vector<Neuron> const& prevLayer,
                                            unsigned int const& nNeuron,
                                            std::vector<float> const& errors)
{
    
    float err = 0;

    for(size_t nPrev = 0; nPrev < prevLayer.size(); ++nPrev)
    {
        err = calculateWeight(    layer[nNeuron][nPrev].weight, 
                                        errors[nNeuron], 
                                        prevLayer[nPrev].out());

        layer[nNeuron][nPrev].newWeight =   layer[nNeuron][nPrev].weight + 
                                            (learningRate_ * err);
    }

}

void NeuralNetworkTrainer::setLearningRate(float rate)
{

    this->learningRate_ = rate;

}
