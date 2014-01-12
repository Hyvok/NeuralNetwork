#include <vector>
#include "Neuron.h"

Neuron::Neuron() : inputSynapses(0) {}

Neuron::Neuron(int nInputSynapses) :
    inputSynapses(nInputSynapses)
{}

int Neuron::updateState()
{
    int sum = 0;
    int updates = 0;

    for(std::vector<Synapse>::iterator iter = inputSynapses.begin(); iter != inputSynapses.end(); ++iter)
    {
        sum += (*iter).weight;
    }

    if(sum > THRESHOLD)
    {
        outputSynapse.value = FIRED_NEURON;
        ++updates;
    }
    else
    {
        outputSynapse.value = 0;
        ++updates;
    }
    return updates;
}
