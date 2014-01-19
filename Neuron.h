#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "InputSynapse.h"
#include "OutputSynapse.h"

// Implements a Neuron with a linear activation function
class Neuron
{

    public:

        Neuron();

        // Updates neuron state, returns the difference between old and new
        // output value
        float updateState();

        std::vector<InputSynapse> inputSynapses;
        OutputSynapse outputSynapse;

};

#endif
