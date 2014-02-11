#include <iostream>
// rand()
#include <stdlib.h>
#include <time.h>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <omp.h>
#include "Config.h"
#include "NnImageMap.h"
#include "NnTrainer.h"
#include "NeuralNetwork.h"

NnTrainer::NnTrainer() :    nn_(0), imageMap_(0), input_(0), output_(0), 
                            nTrainings_(0), learningRate_(0) {}

NnTrainer::NnTrainer( NeuralNetwork& network, float learningRate) : 
                        nn_(&network), type_(nn_->getNeuronType()), 
                        imageMap_(0), input_(0), output_(0), nTrainings_(0),
                        learningRate_(learningRate) {}
        
NnTrainer::NnTrainer(   NeuralNetwork& network, std::vector<float> input,
                        float learningRate) :
                            nn_(&network), type_(nn_->getNeuronType()), 
                            imageMap_(0), input_(input), output_(0), 
                            nTrainings_(0), learningRate_(learningRate)
{

    nn_->setInput(input);
    srand(time(NULL));

}


NnTrainer::NnTrainer(   NeuralNetwork& network, std::vector<float> input,
                        std::vector<float> output, float learningRate) :
                            nn_(&network), type_(nn_->getNeuronType()), 
                            imageMap_(0), input_(input), 
                            output_(output), nTrainings_(0),
                            learningRate_(learningRate)
{

    nn_->setInput(input);
    srand(time(NULL));

}

NnTrainer::NnTrainer(   NeuralNetwork& network, NnImageMap& imageMap,
                        float learningRate) :
                            nn_(&network), type_(nn_->getNeuronType()), 
                            imageMap_(&imageMap), input_(0), 
                            output_(0), nTrainings_(0),
                            learningRate_(learningRate)
{

    srand(time(NULL));

}

// TODO: split into smaller parts
unsigned int NnTrainer::trainNetwork()
{

    // Select random training case from imageMap_ and set input of the network 
    // to match it
    size_t nTrainingCase = rand() % (*imageMap_).size();
    nn_->setInput((*imageMap_)[nTrainingCase].in());

    // Resize the error vector
    std::vector<std::vector<float> > outputError(nn_->layers());

    // Iterate over the layers in reverse order
    for(unsigned int nLayer = nn_->layers() - 1; nLayer > 0; --nLayer)
    {
        outputError[nLayer].resize((*nn_)[nLayer].size());

        // Iterate through neurons in the layer
        // Do this in multiple threads via OpenMP
        #ifdef MULTITHREAD
            #pragma omp parallel for ordered schedule(static)
        #endif
        for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
        {
            float err = 0.0;
            // If we are in the last (=output) layer
            if(nLayer == nn_->layers() - 1)
            {
                if(type_ == Neuron::Type::TYPE_LINEAR)
                {
                    err =   ((*imageMap_)[nTrainingCase].out()[n] - 
                            (*nn_)[nLayer][n].out());
                }
                // TYPE_SIGMOID, default
                else
                {
                    err =   (*nn_)[nLayer][n].out() * ((1.0 - 
                            (*nn_)[nLayer][n].out()) * 
                            ((*imageMap_)[nTrainingCase].out()[n] - 
                            (*nn_)[nLayer][n].out()));
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
            }
            outputError[nLayer][n] = err;
            updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer - 1], n,
                            outputError[nLayer]);
            err = 0;
        }
    }

    unsigned int nUpdates = nn_->swapWeights();
    ++nTrainings_;

    return nUpdates;

}

float NnTrainer::calculateWeight(   float const& weight, float const& err, 
                                    float const& out)
{

    return (err * out);

}

void NnTrainer::updateWeights(  std::vector<Neuron>& layer, 
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

void NnTrainer::setLearningRate(float rate)
{

    this->learningRate_ = rate;

}
