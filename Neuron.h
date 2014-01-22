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

        // Updates all the inputSynapses weights to newWeight
        int updateWeights();

        // Operator for accessing the synapses
        InputSynapse& operator[](int nSynapse);

        // Get the size (=number) of inputSynapses
        size_t size();

        // Return value of the output synapse
        float out();

        std::vector<InputSynapse> inputSynapses;
        OutputSynapse outputSynapse;

};

#endif
