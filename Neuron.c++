#include <vector>
#include <iostream>
#include <cmath>
#include "Neuron.h"

Neuron::Neuron() : inputSynapses(), outputSynapse(), type_(TYPE_SIGMOID) {}

Neuron::Neuron(Type type) : inputSynapses(), outputSynapse(), type_(type) {}

float Neuron::updateState()
{

    float sum = 0;
    float difference = 0;

    // neper
    float e = 2.71828182845904523536;

    if(inputSynapses.size() == 0)
    {
        return 0;
    }

    for(auto& synapse: inputSynapses)
    {
        sum += synapse.weight * (*synapse.value);
    }

    difference = sum - outputSynapse.value;

    if(type_ == TYPE_LINEAR)
    {
        outputSynapse.value = sum;
    }
    else
    {
        // Sigmoid function
        outputSynapse.value = (1 / ( 1 + pow(e, -sum)));
    }

    return difference;

}

unsigned int Neuron::updateWeights()
{

    unsigned int nWeights = 0;

    if(inputSynapses.size() == 0)
    {
        return 0;
    }

    for(auto& synapse: inputSynapses)
    {
        synapse.weight = synapse.newWeight;
        ++nWeights;
    }

    return nWeights;

}

Neuron::InputSynapse& Neuron::operator[](unsigned int nSynapse)
{

    return inputSynapses[nSynapse];

}

size_t Neuron::size()
{

    return inputSynapses.size();

}

float Neuron::out()
{

    return outputSynapse.value;

}
