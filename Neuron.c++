#include <vector>
#include <iostream>
#include "Neuron.h"

Neuron::Neuron() : inputSynapses(), outputSynapse() {}

int Neuron::updateState()
{

    int sum = 0;
    int difference = 0;

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
