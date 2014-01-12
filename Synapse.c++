#include "Synapse.h"

Synapse::Synapse()
{
    this->weight = 0;
    this->value = 0;
    this->connectedTo = 0;
}

Synapse::Synapse(Synapse *synapse)
{
    this->weight = 0;
    this->value = 0;
    this->connectedTo = synapse;
}
