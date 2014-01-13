#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_test)
{
    NeuralNetwork emptyNetwork;
    BOOST_CHECK(emptyNetwork.getConnections() == 0);
    BOOST_CHECK(emptyNetwork.inputNeurons == 0);
    BOOST_CHECK(emptyNetwork.outputNeurons == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 3, 2, 1 neurons...");
    std::vector<int> normal = {3, 2, 1};
    NeuralNetwork testNetwork(normal);
    BOOST_CHECK(testNetwork.getConnections() == 8);
    BOOST_CHECK(testNetwork.inputNeurons->size() == 3);
    BOOST_CHECK(testNetwork.outputNeurons->size() == 1);
    BOOST_CHECK(testNetwork.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 1, 1, 1 neurons...");
    std::vector<int> small = {1, 1, 1};
    NeuralNetwork testNetworkSmall(small);
    BOOST_CHECK(testNetworkSmall.getConnections() == 2);
    BOOST_CHECK(testNetworkSmall.inputNeurons->size() == 1);
    BOOST_CHECK(testNetworkSmall.outputNeurons->size() == 1);
    BOOST_CHECK(testNetworkSmall.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 5, 5, 5 neurons...");
    std::vector<int> big = {5, 5, 5};
    NeuralNetwork testNetworkBig(big);
    BOOST_CHECK(testNetworkBig.getConnections() == 50);
    BOOST_CHECK(testNetworkBig.inputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBig.outputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBig.updateState() == 0);

}
