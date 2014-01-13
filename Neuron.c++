#include <vector>
#include "Neuron.h"

Neuron::Neuron() : inputSynapses(0), outputSynapse() {}

Neuron::Neuron(int nInputSynapses) : inputSynapses(nInputSynapses), outputSynapse() {}

Neuron::Neuron(int nInputSynapses, int weight) : inputSynapses(nInputSynapses, weight), outputSynapse() {}

bool Neuron::updateState()
{

    int sum = 0;

    for(auto& synapse: inputSynapses)
    {
        sum += synapse.weight;
    }

    if(sum > THRESHOLD)
    {
        outputSynapse.value = FIRED_NEURON;
        return true;
    }
    else
    {
        outputSynapse.value = 0;
        return false;
    }

}
