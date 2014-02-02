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
                                                learningRate_(LEARNING_RATE) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network) : 
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), imageMap_(0),
                                            input_(0), output_(0), 
                                            nTrainings_(0),
                                            learningRate_(LEARNING_RATE) {}
        
NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input) :
                                            nn_(&network),
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(0), input_(input), 
                                            output_(0), nTrainings_(0),
                                            learningRate_(LEARNING_RATE)
{

    nn_->setInput(input);
    srand(time(NULL));

}


NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<float> input,
                                            std::vector<float> output) :
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(0), input_(input), 
                                            output_(output), nTrainings_(0),
                                            learningRate_(LEARNING_RATE)
{

    nn_->setInput(input);
    srand(time(NULL));

}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network,
                                            NnImageMap& imageMap) :
                                            nn_(&network), 
                                            type_(nn_->getNeuronType()), 
                                            imageMap_(&imageMap), input_(0), 
                                            output_(0), nTrainings_(0),
                                            learningRate_(LEARNING_RATE)
{

    srand(time(NULL));

}

// TODO: split into smaller parts
int NeuralNetworkTrainer::trainNetwork()
{

    float err = 0.0;
    float outErr = 0.0;

    // Select random training case from imageMap_ and set input of the network 
    // to match it
    size_t nTrainingCase = rand() % (*imageMap_).size();
    nn_->setInput((*imageMap_)[nTrainingCase].in());
    nn_->updateState();

    //BOOST_LOG_TRIVIAL(info) << "Random value for training case selection: " << nTrainingCase;
    //BOOST_LOG_TRIVIAL(info) << "Training case: " << (*imageMap_)[nTrainingCase].trainingCase;
    //BOOST_LOG_TRIVIAL(info) << "Desired output for training case: " << (*imageMap_)[nTrainingCase].getOutStr();

    std::vector<std::vector<float> > outputError(nn_->size());

    // Iterate over the layers in reverse order
    for(int nLayer = nn_->size()-1; nLayer > 0; --nLayer)
    {
        if(nLayer == nn_->size()-1)
        {
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                if(type_ == Neuron::Type::TYPE_LINEAR)
                {
                    outputError[nLayer].push_back(  ((*imageMap_)[nTrainingCase].out()[n] - 
                                                    (*nn_)[nLayer][n].out()));
            
                    updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer-1], n,
                                    outputError[nLayer]);
                }
                // TYPE_SIGMOID, default
                else
                {
                    // Funnily enough it doesn't seem to matter do I use this
                    // target - output or the same with the sigmoid derivative
                    // it seems to work fine anyway...
                    /*outErr =    ((*imageMap_)[nTrainingCase].out()[n] - 
                                (*nn_)[nLayer][n].out());*/
                    outErr =  (*nn_)[nLayer][n].out() * ((1.0 - 
                                    (*nn_)[nLayer][n].out()) * 
                                    ((*imageMap_)[nTrainingCase].out()[n] - 
                                    (*nn_)[nLayer][n].out()));
                    //BOOST_LOG_TRIVIAL(info) << "Desired neuron output value: " << (*imageMap_)[nTrainingCase].out()[n] << " error: " << outErr; 
                    outputError[nLayer].push_back(outErr);
                    /*if(outErr == 0)
                    {
                        break;
                    }*/
                    updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer-1], n,
                                    outputError[nLayer]);
                }
            }
        }
        else 
        {
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                for(size_t nPrev = 0; nPrev < (*nn_)[nLayer+1].size(); ++nPrev)
                {
                    if(type_ == Neuron::Type::TYPE_LINEAR)
                    {
                        err = (    outputError[nLayer+1][nPrev] 
                                    * (*nn_)[nLayer+1][nPrev][n].weight); 

                        //std::cout << "Error calculation for neuron " << n << ", previous neuron " << nPrev << " error: " << err << "\n";
                    }
                    // TYPE_SIGMOID
                    else
                    {
                        err =  (*nn_)[nLayer][n].out() * 
                                (1 - (*nn_)[nLayer][n].out()) * 
                                (outputError[nLayer+1][nPrev] 
                                * (*nn_)[nLayer+1][nPrev][n].weight); 
                    }
                }
                outputError[nLayer].push_back(err);
                err = 0;
            }
            for(size_t n = 0; n < (*nn_)[nLayer].size(); ++n)
            {
                updateWeights(  (*nn_)[nLayer], (*nn_)[nLayer-1], n,
                                outputError[nLayer]);
            }
        }
        /*if(outErr == 0)
        {
            break;
        }*/
    }

    int nUpdates = nn_->updateWeights();
    // Might not be needed
    nn_->updateState();
    ++nTrainings_;

    return nUpdates;

}

// TODO: check if it is worth converting this to inline
float NeuralNetworkTrainer::calculateWeight(    float weight, float err, 
                                                float out)
{

    return ((err * out));

}

void NeuralNetworkTrainer::updateWeights(   std::vector<Neuron>& layer, 
                                            std::vector<Neuron>& prevLayer,
                                            int nNeuron,
                                            std::vector<float> errors)
{
    
    float err = 0;

    for(size_t nPrev = 0; nPrev < prevLayer.size(); ++nPrev)
    {
        err = calculateWeight(    layer[nNeuron][nPrev].weight, 
                                        errors[nNeuron], 
                                        prevLayer[nPrev].out());
        //BOOST_LOG_TRIVIAL(info) << "Weight error: " << err;
        layer[nNeuron][nPrev].newWeight =   layer[nNeuron][nPrev].weight + 
                                            (learningRate_ * err);
    }

}

void NeuralNetworkTrainer::setLearningRate(float rate)
{

    this->learningRate_ = rate;

}
