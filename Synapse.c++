#include "Synapse.h"

Synapse::Synapse() : weight(0), value(0), connectedTo(0) {}

Synapse::Synapse(Synapse* synapse) : weight(0), value(0), connectedTo(synapse){}
