#include <iostream>
#include <boost/log/trivial.hpp>
#include "NeuralNetwork.h"
#include "Neuron.h"

NeuralNetwork::NeuralNetwork() : _inputNeurons(0), _outputNeurons(0), _outputSynapses(0), _nConnections(0), _nNeurons(0) 
{

    _inputNeurons = 0;
    _outputNeurons = 0;

}

NeuralNetwork::NeuralNetwork(std::vector<int> nNeuronsPerLayer, int weight) : 
                                    _neurons(0), _outputSynapses(0), _nConnections(0), _nNeurons(0)

{

    _nNeurons = createNeurons(nNeuronsPerLayer);
    _nConnections = connectNetwork(weight);

}

int NeuralNetwork::createNeurons(std::vector<int> nNeuronsPerLayer)
{

    int nNeurons = 0;

    _neurons.resize(nNeuronsPerLayer.size());
                    
    int nLayer = 0;

    for(nLayer = 0; nLayer < nNeuronsPerLayer.size(); ++nLayer)
    {
        for(size_t j = 0; j < nNeuronsPerLayer[nLayer]; ++j)
        {
            _neurons[nLayer].emplace_back();
            ++nNeurons;
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Layers created: " << nLayer;
    BOOST_LOG_TRIVIAL(info) << "Neurons created: " << nNeurons;

    _inputNeurons = &_neurons[0];
    _outputNeurons = &_neurons[nNeuronsPerLayer.size()-1];
    BOOST_LOG_TRIVIAL(info) << "Input neurons created: " << this->_inputNeurons->size();
    BOOST_LOG_TRIVIAL(info) << "Output neurons created: " << this->_outputNeurons->size();

    return nNeurons;

}

int NeuralNetwork::connectNetwork(int weight)
{

    int nInputSynapses = 0;
    int nLayer = 0;

    std::vector<Neuron>* prevLayer;

    for(auto& layer: _neurons)
    {
        if(nLayer == 0) 
        {
            // Create input synapses which are not connected to anything
            for(auto& neuron: layer)
            {
                neuron.inputSynapses.push_back(Synapse(weight));
                ++nInputSynapses;
            }
        }
        else
        {
            for(auto& neuron: layer)
            {
                for(auto& prevNeuron: *prevLayer)
                {
                    neuron.inputSynapses.push_back(Synapse(weight));
                    ++nInputSynapses;
                }
            }
        }
        prevLayer = &layer;
        ++nLayer;
    }

    int nSynapse = 0;
    nLayer = 0;

    _outputSynapses.resize(_outputNeurons->size());

    for(auto& layer: _neurons)
    {
        if(nLayer == 0) {}
        else
        {
            for(auto& neuron: *prevLayer)
            {
                for(auto& connectedNeuron: layer)
                {
                    // Connect first neuron from prevLayer to inputSynapse 0 etc.
                    neuron.outputSynapse = &connectedNeuron.inputSynapses[nSynapse];
                }
                ++nSynapse;
            }
        }  
        nSynapse = 0;
        prevLayer = &layer;
        ++nLayer;
    }

    int nNeuron = 0;

    for(auto& neuron: *_outputNeurons)
    {
        neuron.outputSynapse = &_outputSynapses[nNeuron];
        ++nNeuron;
    }

    BOOST_LOG_TRIVIAL(info) << "Connections created: " << nInputSynapses;
    BOOST_LOG_TRIVIAL(info) << "Actual layers created: " << nLayer;
    return nInputSynapses;

}

int NeuralNetwork::updateState()
{

    int updates = 0;

    for(auto& layer: _neurons)
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

std::vector<std::vector<std::vector<int> > > NeuralNetwork::getWeights()
{

    std::vector<std::vector<std::vector<int> > > state(_neurons.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

    for(auto& layer: _neurons)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<std::vector<int > > layer_temp;
        state.push_back(layer_temp);

        for(auto& neuron: layer)
        {
            BOOST_LOG_TRIVIAL(info) << "\tNeuron: " << nNeuron;
            std::vector<int> neuron_temp;
            state.back().push_back(neuron_temp);
            for(auto& synapse: neuron.inputSynapses)
            {
                BOOST_LOG_TRIVIAL(info) << "\t\tWeight: " << synapse.weight;
                state.back().back().push_back(synapse.weight);
                ++nSynapse;
            }
            ++nNeuron;
        }
        ++nLayer;
    }

    return state;

}

std::vector<std::vector<int> > NeuralNetwork::getState()
{

    std::vector<std::vector<int> > state(_neurons.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

    for(auto& layer: _neurons)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<int> layer_temp;
        state.push_back(layer_temp);

        for(auto& neuron: layer)
        {
            if(neuron.outputSynapse != nullptr)
            {
                BOOST_LOG_TRIVIAL(info) << "\t\tState: " << neuron.outputSynapse->value;
                state.back().push_back(neuron.outputSynapse->value);
            }
            ++nSynapse;
            ++nNeuron;
        }
        ++nLayer;
    }

    return state;

}

bool NeuralNetwork::setInput(std::vector<int> input)
{

    if(input.size() != _inputNeurons->size())
    {
        return false;
    }

    int nNeuron = 0;

    for(auto& neuron: *_inputNeurons)
    {
        neuron.inputSynapses.front().value = input[nNeuron];
        ++nNeuron;
    }

    return true;

}

std::vector<int> NeuralNetwork::getOutput()
{

    std::vector<int> result(_outputSynapses.size());
    int nSynapse = 0;

    for(auto& synapse: _outputSynapses)
    {
        result[nSynapse] = synapse.value;
        ++nSynapse;
    }

    return result;

}

int NeuralNetwork::getInputSize()
{
    
    if(_inputNeurons == nullptr)
    {
        return 0;
    }
    else
    {
        return _inputNeurons->size();
    }

}

int NeuralNetwork::getOutputSize()
{
    
    if(_outputNeurons == nullptr)
    {
        return 0;
    }
    else
    {
        return _outputNeurons->size();
    }

}
