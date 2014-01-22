#include <iostream>
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

NeuralNetwork::NeuralNetwork(   std::vector<int> nNeuronsPerLayer, 
                                float weight) :
                                neurons_(0), inputValues_(0),
                                outputValues_(0), nConnections_(0),
                                nNeurons_(0)
{

    // Create neurons
    nNeurons_ = createNeurons(nNeuronsPerLayer);

    // Create the synapses and connect the pointers between outputSynapses
    // and inputSynapses
    nConnections_ = connectNetwork(weight);

}

int NeuralNetwork::createNeurons(std::vector<int> nNeuronsPerLayer)
{

    int nNeurons = 0;

    neurons_.resize(nNeuronsPerLayer.size());

    int nLayer = 0;

    for(nLayer = 0; nLayer < nNeuronsPerLayer.size(); ++nLayer)
    {
        for(size_t j = 0; j < nNeuronsPerLayer[nLayer]; ++j)
        {
            // Create neuron
            neurons_[nLayer].emplace_back();
            ++nNeurons;
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Layers created: " << nLayer;
    BOOST_LOG_TRIVIAL(info) << "Neurons created: " << nNeurons;

    // Connect inputNeurons_ pointer to the correct layer
    inputNeurons_ = &neurons_[0];
    // Connect outputNeurons_ pointer to the correct layer
    outputNeurons_ = &neurons_[nNeuronsPerLayer.size()-1];

    BOOST_LOG_TRIVIAL(info) << "Input neurons created: "
                            << this->inputNeurons_->size();
    BOOST_LOG_TRIVIAL(info) << "Output neurons created: "
                            << this->outputNeurons_->size();

    return nNeurons;

}

int NeuralNetwork::connectNetwork(float weight)
{

    int nInputSynapses = 0;
    int nLayer = 0;
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
                neuron.inputSynapses.push_back(InputSynapse(1.0));
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
                        weight_ = rand() % MAX_RANDOM_WEIGHT;
                    }
                    neuron.inputSynapses.push_back(InputSynapse(weight_));
                    ++nInputSynapses;
                }
            }
        }
        prevLayer = &layer;
        ++nLayer;
    }

    int nSynapse = 0;
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

    int nNeuron = 0;

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

    BOOST_LOG_TRIVIAL(info) << "Connections created: " << nInputSynapses;
    BOOST_LOG_TRIVIAL(info) << "Actual layers created: " << nLayer;
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

int NeuralNetwork::updateWeights()
{

    int nWeights = 0;
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

int NeuralNetwork::getConnections()
{

    return nConnections_;

}

// TODO: combine getter functions with templates
std::vector<std::vector<std::vector<float> > > NeuralNetwork::getWeights()
{

    std::vector<std::vector<std::vector<float> > > state(neurons_.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

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

std::vector<std::vector<std::vector<float> > > NeuralNetwork::getValues()
{

    std::vector<std::vector<std::vector<float> > > values(neurons_.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

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

std::vector<std::vector<float> > NeuralNetwork::getState()
{

    std::vector<std::vector<float> > state(neurons_.size());

    int nLayer = 0;
    int nNeuron = 0;
    int nSynapse = 0;

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
        return false;
    }
    else
    {
        inputValues_ = input;
        return true;
    }

}

std::vector<float> NeuralNetwork::getOutput()
{

    std::vector<float> output(outputValues_.size());

    for(size_t i = 0; i < outputValues_.size(); ++i)
    {
        output[i] = (*outputValues_[i]);
    }

    return output;

}

int NeuralNetwork::getInputSize()
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

int NeuralNetwork::getOutputSize()
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

size_t NeuralNetwork::size()
{

    return neurons_.size();

}
