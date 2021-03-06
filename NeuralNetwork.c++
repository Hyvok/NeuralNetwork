#include <iostream>
#include <sstream>
#include <iterator>
#include <boost/log/trivial.hpp>
// rand()
#include <stdlib.h>
#include <time.h>
#include "NeuralNetwork.h"
#include "Neuron.h"

NeuralNetwork::NeuralNetwork() :    inputNeurons_(0), outputNeurons_(0),
                                    inputValues_(0), outputValues_(0),
                                    nConnections_(0), nNeurons_(0) {}

NeuralNetwork::NeuralNetwork(   std::vector<unsigned int> nNeuronsPerLayer, 
                                float weight) :
                                neurons_(0), inputValues_(0),
                                outputValues_(0), nConnections_(0),
                                nNeurons_(0), type_(Neuron::Type::TYPE_SIGMOID)
{

    // Create neurons
    nNeurons_ = createNeurons(nNeuronsPerLayer, type_);

    // Create the synapses and connect the pointers between outputSynapses
    // and inputSynapses
    nConnections_ = connectNetwork(weight);

}

NeuralNetwork::NeuralNetwork(   std::vector<unsigned int> nNeuronsPerLayer, 
                                float weight, Neuron::Type type) :
                                neurons_(0), inputValues_(0),
                                outputValues_(0), nConnections_(0),
                                nNeurons_(0), type_(type)
{

    // Create neurons
    nNeurons_ = createNeurons(nNeuronsPerLayer, type_);

    // Create the synapses and connect the pointers between outputSynapses
    // and inputSynapses
    nConnections_ = connectNetwork(weight);

}

NeuralNetwork::NeuralNetwork(std::vector<unsigned int> nNeuronsPerLayer) :
                                neurons_(0), inputValues_(0),
                                outputValues_(0), nConnections_(0),
                                nNeurons_(0), type_(Neuron::Type::TYPE_SIGMOID)
{

    // Create neurons
    nNeurons_ = createNeurons(nNeuronsPerLayer, type_);

    // Create the synapses and connect the pointers between outputSynapses
    // and inputSynapses
    nConnections_ = connectNetwork(0);

}

NeuralNetwork::NeuralNetwork(   std::vector<unsigned int> nNeuronsPerLayer, 
                                Neuron::Type type) :
                                neurons_(0), inputValues_(0),
                                outputValues_(0), nConnections_(0),
                                nNeurons_(0), type_(type)
{

    // Create neurons
    nNeurons_ = createNeurons(nNeuronsPerLayer, type_);

    // Create the synapses and connect the pointers between outputSynapses
    // and inputSynapses
    nConnections_ = connectNetwork(0);

}

unsigned int NeuralNetwork::createNeurons(  std::vector<unsigned int> 
                                            nNeuronsPerLayer, 
                                            Neuron::Type type)
{
    
    BOOST_LOG_TRIVIAL(info) << "Creating new NeuralNetwork";

    // Put the architecture in a stringstream for logging
    std::stringstream architecture;
    for(auto& layer: nNeuronsPerLayer)
    {
        architecture << layer << " ";
    }
    BOOST_LOG_TRIVIAL(info) << "\tArchitecture of the network: " 
                            << architecture.str();
    unsigned int nNeurons = 0;

    neurons_.resize(nNeuronsPerLayer.size());

    unsigned int nLayer = 0;

    for(nLayer = 0; nLayer < nNeuronsPerLayer.size(); ++nLayer)
    {
        for(size_t j = 0; j < nNeuronsPerLayer[nLayer]; ++j)
        {
            // Create neuron
            neurons_[nLayer].emplace_back(type);
            ++nNeurons;
        }
    }

    BOOST_LOG_TRIVIAL(info) << "\tLayers created: " << nLayer;
    BOOST_LOG_TRIVIAL(info) << "\tNeurons created: " << nNeurons;

    // Connect inputNeurons_ pointer to the correct layer
    inputNeurons_ = &neurons_[0];
    // Connect outputNeurons_ pointer to the correct layer
    outputNeurons_ = &neurons_[nNeuronsPerLayer.size()-1];

    BOOST_LOG_TRIVIAL(info) << "\tInput neurons created: "
                            << this->inputNeurons_->size();
    BOOST_LOG_TRIVIAL(info) << "\tOutput neurons created: "
                            << this->outputNeurons_->size();

    return nNeurons;

}

unsigned int NeuralNetwork::connectNetwork(float weight)
{

    unsigned int nInputSynapses = 0;
    unsigned int nLayer = 0;
    float weight_ = weight;

    if(weight == 0)
    {
        srand(time(NULL));
    }

    std::vector<Neuron>* prevLayer;
    
    for(auto& layer: neurons_)
    {
        if(nLayer == 0)
        {
            // Create input synapses to the first layer with weight 1
            // TODO: make sure the input layer weights are supposed to be 1...
            for(auto& neuron: layer)
            {
                neuron.inputSynapses.emplace_back(1.0);
                ++nInputSynapses;
            }
        }
        else
        {
            for(auto& neuron: layer)
            {
                // Create rest of the synapses, same amount of input synapses
                // as there are neurons in the previous layer
                for(size_t n = 0; n < (*prevLayer).size(); ++n)
                {
                    if(weight == 0)
                    {
                        // Random weights have to be symmetrical around 0, if
                        // they are all negative or positive the networks values
                        // can get maxed out before it has time to converge!
                        weight_ = (rand() % MAX_RANDOM_WEIGHT - 
                                            (MAX_RANDOM_WEIGHT/2)) / 5.0;
                    }
                    neuron.inputSynapses.emplace_back();
                    neuron.inputSynapses.back().weight = weight_;
                    ++nInputSynapses;
                }
            }
        }
        prevLayer = &layer;
        ++nLayer;
    }

    unsigned int nSynapse = 0;
    nLayer = 0;

    for(auto& layer: neurons_)
    {
        if(nLayer == 0) {}
        else
        {
            for(auto& neuron: *prevLayer)
            {
                // Connect the inputSynapses value pointer to the correct
                // outputSynapse in the previous layer
                for(auto& connectedNeuron: layer)
                {
                    connectedNeuron.inputSynapses[nSynapse].value =
                                                &neuron.outputSynapse.value;
                }
                ++nSynapse;
            }
        }
        nSynapse = 0;
        prevLayer = &layer;
        ++nLayer;
    }

    unsigned int nNeuron = 0;

    // Resize inputValues_ vector and connect the pointers to the input neurons
    inputValues_.resize(inputNeurons_->size());

    for(auto& neuron: *inputNeurons_)
    {
        neuron.inputSynapses.front().value = &inputValues_[nNeuron];
        ++nNeuron;
    }

    nNeuron = 0;

    // Resize outputValues_ vector and connect the pointers to the output
    // neurons
    outputValues_.resize(outputNeurons_->size());

    for(auto& neuron: *outputNeurons_)
    {
        outputValues_[nNeuron] = &neuron.outputSynapse.value;
        ++nNeuron;
    }

    BOOST_LOG_TRIVIAL(info) << "\tConnections created: " << nInputSynapses;
    return nInputSynapses;

}

float NeuralNetwork::updateState()
{

    float change = 0;

    for(auto& layer: neurons_)
    {
        for(auto& neuron: layer)
        {
           change += neuron.updateState();
        }
    }

    return change;

}

unsigned int NeuralNetwork::swapWeights()
{

    unsigned int nWeights = 0;
    size_t nLayer = 0;

    for(auto& layer: neurons_)
    {
        // Don't update input neuron weights, they stay at 1
        if(nLayer != 0)
        {
            for(auto& neuron: layer)
            {
                nWeights += neuron.updateWeights();
            }
        }
        ++nLayer;
    }

    return nWeights;

}

unsigned int NeuralNetwork::getConnections() const
{

    return nConnections_;

}

// TODO: combine getter functions with templates
std::vector<std::vector<std::vector<float> > > NeuralNetwork::getWeights() const
{

    std::vector<std::vector<std::vector<float> > > state(neurons_.size());

    unsigned int nLayer = 0;
    unsigned int nNeuron = 0;
    unsigned int nSynapse = 0;

    for(auto& layer: neurons_)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<std::vector<float > > layer_temp;
        state.push_back(layer_temp);

        for(auto& neuron: layer)
        {
            BOOST_LOG_TRIVIAL(info) << "\tNeuron: " << nNeuron;

            std::vector<float> neuron_temp;
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

std::vector<std::vector<std::vector<float> > > NeuralNetwork::getValues() const
{

    std::vector<std::vector<std::vector<float> > > values(neurons_.size());

    unsigned int nLayer = 0;
    unsigned int nNeuron = 0;
    unsigned int nSynapse = 0;

    for(auto& layer: neurons_)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<std::vector<float > > layer_temp;
        values.push_back(layer_temp);

        for(auto& neuron: layer)
        {
            BOOST_LOG_TRIVIAL(info) << "\tNeuron: " << nNeuron;
            std::vector<float> neuron_temp;
            values.back().push_back(neuron_temp);
            for(auto& synapse: neuron.inputSynapses)
            {
                BOOST_LOG_TRIVIAL(info) << "\t\tValue: " << (*synapse.value);
                values.back().back().push_back(*synapse.value);
                ++nSynapse;
            }
            ++nNeuron;
        }
        ++nLayer;
    }

    return values;

}

std::vector<std::vector<float> > NeuralNetwork::getState() const
{

    std::vector<std::vector<float> > state(neurons_.size());

    unsigned int nLayer = 0;
    unsigned int nNeuron = 0;
    unsigned int nSynapse = 0;

    for(auto& layer: neurons_)
    {
        BOOST_LOG_TRIVIAL(info) << "Layer: " << nLayer;

        std::vector<float> layer_temp;
        state.push_back(layer_temp);

        for(auto& neuron: layer)
        {
            BOOST_LOG_TRIVIAL(info) << "\t\tState: "
                                    << neuron.outputSynapse.value;
            state.back().push_back(neuron.outputSynapse.value);
            ++nSynapse;
            ++nNeuron;
        }
        ++nLayer;
    }

    return state;

}

bool NeuralNetwork::setInput(std::vector<float> input)
{

    if(input.size() != inputNeurons_->size())
    {
        BOOST_LOG_TRIVIAL(error)    << "setInput(): input does not match "
                                    << "network input size!";
        return false;
    }
    else
    {
        inputValues_ = input;
        updateState();
        return true;
    }

}

std::vector<float> NeuralNetwork::getOutput() const
{

    std::vector<float> output(outputValues_.size());

    for(size_t i = 0; i < outputValues_.size(); ++i)
    {
        output[i] = (*outputValues_[i]);
    }

    return output;

}

unsigned int NeuralNetwork::getInputSize() const
{

    if(inputNeurons_ == nullptr)
    {
        return 0;
    }
    else
    {
        return inputNeurons_->size();
    }

}

unsigned int NeuralNetwork::getOutputSize() const
{

    if(outputNeurons_ == nullptr)
    {
        return 0;
    }
    else
    {
        return outputNeurons_->size();
    }

}

std::vector<Neuron>& NeuralNetwork::operator[](size_t nLayer)
{

    return neurons_[nLayer];

}

size_t NeuralNetwork::size() const
{

    return nNeurons_;

}

size_t NeuralNetwork::layers() const
{

    return neurons_.size();

}

Neuron::Type NeuralNetwork::getNeuronType() const
{

    return type_;

}

std::vector<Neuron>& NeuralNetwork::back()
{

    return neurons_.back();

}
