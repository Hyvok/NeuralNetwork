#include "InputSynapse.h"

InputSynapse::InputSynapse() : newWeight(0), weight(0), value(0) {}

InputSynapse::InputSynapse(float weight) :  newWeight(1), weight(weight), 
                                            value(0) {}
