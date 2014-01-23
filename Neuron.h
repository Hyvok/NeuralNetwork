#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include "InputSynapse.h"
#include "OutputSynapse.h"

// Implements a Neuron with a linear activation function
class Neuron
{

    public:

        enum Type { TYPE_LINEAR, TYPE_SIGMOID };

        // Construct a neuron, defaults to TYPE_SIGMOID
        Neuron();

        // Construct Neuron of Type
        Neuron(Type type);

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

    private:

        Type type_;

};

#endif
