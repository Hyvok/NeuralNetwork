#include <boost/test/unit_test.hpp>
#include "Neuron.h"

BOOST_AUTO_TEST_CASE(neuron_test)
{

    Neuron testNeuronOne;
    BOOST_CHECK(testNeuronOne.inputSynapses.size() == 0);
    BOOST_CHECK(testNeuronOne.updateState() == false);

    Neuron testNeuronTwo(0);
    BOOST_CHECK(testNeuronTwo.inputSynapses.size() == 0);
    BOOST_CHECK(testNeuronTwo.updateState() == false);

    Neuron testNeuronThree(1);
    testNeuronThree.inputSynapses.front().weight = THRESHOLD+1;
    BOOST_CHECK(testNeuronThree.updateState() == true);
    testNeuronThree.inputSynapses.front().weight = THRESHOLD-1;
    BOOST_CHECK(testNeuronThree.updateState() == false);

}
