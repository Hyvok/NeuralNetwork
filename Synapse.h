#ifndef SYNAPSE_H
#define SYNAPSE_H

class Synapse
{

    public:

        Synapse();
        Synapse(Synapse*);

        int weight;
        int value;
        Synapse *connectedTo;

};

#endif
