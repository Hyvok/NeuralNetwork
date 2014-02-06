#ifndef NEURON_H
#define NEURON_H

#include <vector>

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
        unsigned int updateWeights();

        // Get the size (=number) of inputSynapses
        size_t size();

        // Return value of the output synapse
        float out();

        class InputSynapse 
        {

            public:

                InputSynapse() : weight(0), newWeight(1), value(nullptr) {}
                InputSynapse(int weight) :  weight(weight), newWeight(1), 
                                            value(nullptr) {}

                float weight;
                float newWeight;
                float* value;

        };

        class OutputSynapse
        {

            public:

                OutputSynapse() : value(0) {}

                float value;

        };

        // Operator for accessing the synapses
        InputSynapse& operator[](unsigned int nSynapse);

        std::vector<InputSynapse> inputSynapses;
        OutputSynapse outputSynapse;

    private:

        Type type_;

};

#endif
