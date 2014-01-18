#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "InputSynapse.h"
#include "OutputSynapse.h"

class Neuron
{

    public:

        Neuron();

        // Returns true if the neuron fires
        int updateState();

        std::vector<InputSynapse> inputSynapses;
        OutputSynapse outputSynapse;

};

#endif
