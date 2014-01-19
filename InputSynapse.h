#ifndef INPUT_SYNAPSE_H
#define INPUT_SYNAPSE_H

class InputSynapse
{

    public:

        InputSynapse();

        // Constructor for creating an InputSynapse with weight
        InputSynapse(float weight);

        float weight;
        float* value;

};

#endif
