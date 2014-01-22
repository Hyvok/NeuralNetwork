#include <vector>
#include <iostream>
#include "Neuron.h"
#include "InputSynapse.h"

Neuron::Neuron() : inputSynapses(), outputSynapse() {}

float Neuron::updateState()
{

    float sum = 0;
    float difference = 0;

    if(inputSynapses.size() == 0)
    {
        return 0;
    }

    for(auto& synapse: inputSynapses)
    {
        sum += synapse.weight * (*synapse.value);
    }

    difference = sum - outputSynapse.value;
    outputSynapse.value = sum;

    return difference;

}

int Neuron::updateWeights()
{

    int nWeights = 0;

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

InputSynapse& Neuron::operator[](int nSynapse)
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
