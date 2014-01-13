#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "Synapse.h"

#define FIRED_NEURON 1
#define THRESHOLD 1000

class Neuron
{

    public:

        Neuron();
        Neuron(int nInputSynapses);
        Neuron(int nInputSynapses, int weight);

        // Returns true if the neuron fires
        bool updateState();

        std::vector<Synapse> inputSynapses;
        Synapse outputSynapse;

};

#endif
