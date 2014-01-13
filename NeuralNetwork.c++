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

NeuralNetwork::NeuralNetwork(std::vector<int> nNeuronsPerLayer) : neurons(0), connections(0)
{
    int nNeurons = 0;

    for(auto& n: nNeuronsPerLayer)
    {
        nNeurons += n;
    }

    neurons.resize(nNeurons);

    int nLayer = 0;

    for(nLayer = 0; nLayer < nNeuronsPerLayer.size(); ++nLayer)
    {
        for(size_t j = 0; j < nNeuronsPerLayer[nLayer]; ++j)
        {
            if(nLayer == 0)
            {
                neurons[nLayer].emplace_back(Neuron(1));
            }
            else
            {
                neurons[nLayer].emplace_back(Neuron(nNeuronsPerLayer[nLayer-1]));
            }
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Layers created: " << nNeuronsPerLayer.size();
    BOOST_LOG_TRIVIAL(info) << "Neurons created: " << nNeurons;

    inputNeurons = &neurons[0];
    outputNeurons = &neurons[nNeuronsPerLayer.size()-1];
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
