#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_test)
{
    NeuralNetwork emptyNetwork;
    BOOST_CHECK(emptyNetwork.getConnections() == 0);
    //BOOST_CHECK(emptyNetwork.inputNeurons->size() == 0);
    //BOOST_CHECK(emptyNetwork.outputNeurons->size() == 0);

    NeuralNetwork testNetwork(3, 1, 2);
    BOOST_CHECK(testNetwork.getConnections() == 8);
    BOOST_CHECK(testNetwork.inputNeurons->size() == 3);
    BOOST_CHECK(testNetwork.outputNeurons->size() == 1);

    NeuralNetwork testNetworkSmall(1, 1, 1);
    BOOST_CHECK(testNetworkSmall.getConnections() == 2);
    BOOST_CHECK(testNetworkSmall.inputNeurons->size() == 1);
    BOOST_CHECK(testNetworkSmall.outputNeurons->size() == 1);

    NeuralNetwork testNetworkBig(5, 5, 5);
    BOOST_CHECK(testNetworkBig.getConnections() == 50);
    BOOST_CHECK(testNetworkBig.inputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBig.outputNeurons->size() == 5);

}
