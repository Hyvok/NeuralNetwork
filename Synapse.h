#ifndef SYNAPSE_H
#define SYNAPSE_H

class Synapse
{

    public:

        Synapse();

        // Constructor for creating a synapse with weight
        Synapse(int weight);
        Synapse(Synapse*);
        Synapse(Synapse*, int weight);

        int weight;
        int value;
        Synapse *connectedTo;

};

#endif
