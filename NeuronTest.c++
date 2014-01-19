#include <boost/test/unit_test.hpp>
#include "Neuron.h"

BOOST_AUTO_TEST_CASE(neuron_test)
{

    Neuron testNeuronOne;
    BOOST_CHECK(testNeuronOne.inputSynapses.size() == 0);
    BOOST_CHECK(testNeuronOne.outputSynapse.value == 0.0);
    BOOST_CHECK(testNeuronOne.updateState() == 0.0);

}
