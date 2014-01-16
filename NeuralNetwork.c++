#include <iostream>
#include <boost/log/trivial.hpp>
#include "NeuralNetwork.h"
#include "Neuron.h"

NeuralNetwork::NeuralNetwork() : inputNeurons(0), outputNeurons(0), _nConnections(0), _nNeurons(0)
{

    inputNeurons = 0;
    outputNeurons = 0;

}

NeuralNetwork::NeuralNetwork(std::vector<int> nNeuronsPerLayer, int weight) : 
                                    neurons(0), _nConnections(0), _nNeurons(0)
{

    _nNeurons = createNeurons(nNeuronsPerLayer);
    _nConnections = connectNetwork(weight);

}

int NeuralNetwork::createNeurons(std::vector<int> nNeuronsPerLayer)
{

    int nNeurons = 0;

    neurons.resize(nNeuronsPerLayer.size());
                    
    int nLayer = 0;

    for(nLayer = 0; nLayer < nNeuronsPerLayer.size(); ++nLayer)
    {
        for(size_t j = 0; j < nNeuronsPerLayer[nLayer]; ++j)
        {
            neurons[nLayer].emplace_back(Neuron());
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Layers created: " << nLayer;
    BOOST_LOG_TRIVIAL(info) << "Neurons created: " << nNeuronsPerLayer.size();

    inputNeurons = &neurons[0];
    outputNeurons = &neurons[nNeuronsPerLayer.size()-1];
    BOOST_LOG_TRIVIAL(info) << "Input neurons created: " << this->inputNeurons->size();
    BOOST_LOG_TRIVIAL(info) << "Output neurons created: " << this->outputNeurons->size();

    return nNeurons;

}

int NeuralNetwork::connectNetwork(int weight)
{

    int nInputSynapses = 0;
    int nLayer = 0;

    std::vector<Neuron> prevLayer;

    for(auto& layer: neurons)
    {
        if(nLayer == 0) 
        {
            for(auto& neuron: layer)
            {
                neuron.inputSynapses.push_back(Synapse(weight));
                ++nInputSynapses;
            }
        }
        else
        {
            for(auto& neuron: prevLayer)
            {
                for(auto& connectedNeuron: layer)
                {
                    connectedNeuron.inputSynapses.push_back(Synapse(weight));
                    neuron.outputSynapse = &connectedNeuron.inputSynapses.back();
                    ++nInputSynapses;
                }
            }
        }
        prevLayer = layer;
        ++nLayer;
    }
    BOOST_LOG_TRIVIAL(info) << "Connections created: " << nInputSynapses;
    BOOST_LOG_TRIVIAL(info) << "Actual layers created: " << nLayer;
    return nInputSynapses;

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

    return _nConnections;

}

std::vector<std::vector<std::vector<int> > > NeuralNetwork::getState()
{

    std::vector<std::vector<std::vector<int> > > state(neurons.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

    for(auto& layer: neurons)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<std::vector<int > > lol;
        state.push_back(lol);

        for(auto& neuron: layer)
        {
            BOOST_LOG_TRIVIAL(info) << "Neuron: " << nNeuron;
            std::vector<int> kek;
            state.back().push_back(kek);
            for(auto& synapse: neuron.inputSynapses)
            {
                BOOST_LOG_TRIVIAL(info) << "Weight: " << synapse.weight;
                state.back().back().push_back(synapse.weight);
                ++nSynapse;
            }
            ++nNeuron;
        }
        ++nLayer;
    }

    return state;

}
