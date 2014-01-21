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
                                            std::vector<int> input) :
                                            network_(&network), input_(input),
                                            output_(0), nTrainings_(0) {}

NeuralNetworkTrainer::NeuralNetworkTrainer( NeuralNetwork& network, 
                                            std::vector<int> input,
                                            std::vector<int> output) :
                                            network_(&network), input_(input),
                                            output_(output), nTrainings_(0) {}

void NeuralNetworkTrainer::trainNetwork()
{

    std::vector<std::vector<float> > deltaOutput(network_->neurons_.size());
    float error = 0.0;

    // Iterate over the layers in reverse order
    for(size_t nLayer = network_->neurons_.size()-1; nLayer > 0; --nLayer)
    {
        if(nLayer == network_->neurons_.size()-1)
        {
            for(size_t nNeuron = 0; nNeuron < network_->neurons_[nLayer].size(); ++nNeuron)
            {
                deltaOutput[nLayer].push_back(((float)output_[nNeuron] - network_->neurons_[nLayer][nNeuron].outputSynapse.value));

                //std::cout << "Output error for neuron " << nNeuron << ": " << deltaOutput[nLayer][nNeuron] << "\n";

                    for(size_t nConnectingNeuron = 0; nConnectingNeuron < network_->neurons_[nLayer-1].size(); ++nConnectingNeuron)
                    {
                        error = network_->neurons_[nLayer][nNeuron].inputSynapses[nConnectingNeuron].weight + (deltaOutput[nLayer][nNeuron] * network_->neurons_[nLayer-1][nConnectingNeuron].outputSynapse.value);
                        //std::cout << "Updated weight for neuron " << nNeuron << ", synapse " << nConnectingNeuron << ": " << error << "\n";
                        network_->neurons_[nLayer][nNeuron].inputSynapses[nConnectingNeuron].newWeight += LEARNING_RATE * error;
                    }
            }
        }
        // Tämä ripulipaskakoodi ei toimi kuin kolmelle kerrokselle
        else if(nLayer != 0)
        {
            error = 0;
            for(size_t nNeuron = 0; nNeuron < network_->neurons_[nLayer].size(); ++nNeuron)
            {
                for(size_t nConnectingNeuron = 0; nConnectingNeuron < network_->neurons_[nLayer+1].size(); ++nConnectingNeuron)
                {
                    error += (deltaOutput[nLayer+1][nConnectingNeuron] * network_->neurons_[nLayer+1][nConnectingNeuron].inputSynapses[nNeuron].weight); 
                    deltaOutput[nLayer].push_back(error);
                    //std::cout << "Error for neuron " << nNeuron << ": " << error << "\n";
                    error = 0;
                }
            }
            for(size_t nNeuron = 0; nNeuron < network_->neurons_[nLayer].size(); ++nNeuron)
            {
                for(size_t nConnectingNeuron = 0; nConnectingNeuron < network_->neurons_[nLayer-1].size(); ++nConnectingNeuron)
                {
                    error = network_->neurons_[nLayer][nNeuron].inputSynapses[nConnectingNeuron].weight + (deltaOutput[nLayer][nNeuron] * network_->neurons_[nLayer-1][nConnectingNeuron].outputSynapse.value);
                    //std::cout << "Updated weight for neuron " << nNeuron << ", synapse " << nConnectingNeuron << ": " << error << "\n";
                    network_->neurons_[nLayer][nNeuron].inputSynapses[nConnectingNeuron].newWeight += LEARNING_RATE * error;
                }
            }
        }
        else
        {
            //std::cout << "Nothing to do with first layer yet\n";
        }
    }

    network_->updateWeights();
    network_->updateState();
    ++nTrainings_;

}
