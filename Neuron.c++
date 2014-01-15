#include <vector>
#include <iostream>
#include "Neuron.h"

Neuron::Neuron() : inputSynapses(0), outputSynapse(0) {}

bool Neuron::updateState()
{

    int sum = 0;

    if(inputSynapses.size() == 0)
    {
        return false;
    }
    for(auto& synapse: inputSynapses)
    {
        //std::cout << synapse.weight << "\n";
        sum += synapse.weight;
    }

    if(sum > THRESHOLD)
    {
        if(outputSynapse != 0) 
        {
            outputSynapse->value = FIRED_NEURON;
        }
        return true;
    }
    else
    {
        if(outputSynapse != 0) 
        {
            outputSynapse->value = 0;
        }
        return false;
    }

}
