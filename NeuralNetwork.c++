#include <iostream>
#include <boost/log/trivial.hpp>
#include "NeuralNetwork.h"
#include "Neuron.h"

// TODO: Remove hard-coded amount of layers
#define LAYERS 3

NeuralNetwork::NeuralNetwork() : inputNeurons(0), outputNeurons(0), connections(0) 
{
    inputNeurons = 0;
    outputNeurons = 0;
}

NeuralNetwork::NeuralNetwork(	int nInputNeurons, int nOutputNeurons, 
                                int nHiddenNeurons) : 
                                neurons(nInputNeurons+nOutputNeurons+nHiddenNeurons)
{

        int nNeurons = 0;

        for(size_t i = 0; i < LAYERS; ++i)
        {
            switch(i)
            {
                case 0:
                    for(size_t j = 0; j < nInputNeurons; ++j)
                    {
                        neurons[i].emplace_back(Neuron(1));
                        ++nNeurons;
                    }
                    break;
                case 1:
                    for(size_t j = 0; j < nHiddenNeurons; ++j)
                    {
                        neurons[i].emplace_back(Neuron(nInputNeurons));
                        ++nNeurons;
                    }
                    break;
                case 2:
                    for(size_t j = 0; j < nOutputNeurons; ++j)
                    {
                        neurons[i].emplace_back(Neuron(nHiddenNeurons));	
                        ++nNeurons;
                    }
                    break;
            }
        }
    BOOST_LOG_TRIVIAL(info) << "Layers created: " << LAYERS;
    BOOST_LOG_TRIVIAL(info) << "Neurons created: " << nNeurons;

    inputNeurons = &neurons[0];
    outputNeurons = &neurons[LAYERS-1];
    BOOST_LOG_TRIVIAL(info) << "Input neurons created: " << this->inputNeurons->size();
    BOOST_LOG_TRIVIAL(info) << "Output neurons created: " << this->outputNeurons->size();

    connections = connectNetwork();
}
int NeuralNetwork::connectNetwork()
{
    int nConnections = 0;
    int nLayer = 0;
    std::vector<Neuron> prevLayer;

    for(auto& layer: neurons)
    {
        if(nLayer == 0) 
        {
            prevLayer = layer;
        }
        else
        {
            for(auto& neuron: layer)
            {
                for(auto& connectedNeuron: prevLayer)
                {
                    neuron.inputSynapses.push_back(connectedNeuron.outputSynapse);
                    ++nConnections;
                }
            }
            prevLayer = layer;
        }
        ++nLayer;
    }
    BOOST_LOG_TRIVIAL(info) << "Connections created: " << nConnections;
    return nConnections;
}
int NeuralNetwork::updateState()
{
    int updates = 0;

    for(auto& layer: neurons)
    {
        for(auto& neuron: layer)
        {
            updates += neuron.updateState();
        }
    }

    return updates;
}
int NeuralNetwork::getConnections()
{
    return connections;
}
