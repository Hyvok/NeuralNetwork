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

        // Returns the number of weights updated
        int updateState();

        std::vector<Synapse> inputSynapses;
        Synapse outputSynapse;

};

#endif
