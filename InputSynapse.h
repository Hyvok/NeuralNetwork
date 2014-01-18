#ifndef INPUT_SYNAPSE_H
#define INPUT_SYNAPSE_H

class InputSynapse
{

    public:

        InputSynapse();

        // Constructor for creating a synapse with weight
        InputSynapse(int weight);

        int weight;
        int* value;

};

#endif
