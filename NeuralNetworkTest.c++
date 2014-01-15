#include <iostream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_test)
{
    NeuralNetwork emptyNetwork;
    BOOST_CHECK(emptyNetwork.getConnections() == 0);
    BOOST_CHECK(emptyNetwork.inputNeurons == 0);
    BOOST_CHECK(emptyNetwork.outputNeurons == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 3, 2, 1 neurons weight 0...");
    std::vector<int> normal = {3, 2, 1};
    NeuralNetwork testNetwork(normal, 0);
    BOOST_CHECK(testNetwork.getConnections() == 11);
    BOOST_CHECK(testNetwork.inputNeurons->size() == 3);
    BOOST_CHECK(testNetwork.outputNeurons->size() == 1);
    BOOST_CHECK(testNetwork.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 1, 1, 1 neurons weight 0...");
    std::vector<int> small = {1, 1, 1};
    NeuralNetwork testNetworkSmall(small, 0);
    BOOST_CHECK(testNetworkSmall.getConnections() == 3);
    BOOST_CHECK(testNetworkSmall.inputNeurons->size() == 1);
    BOOST_CHECK(testNetworkSmall.outputNeurons->size() == 1);
    BOOST_CHECK(testNetworkSmall.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 5, 5, 5 neurons weight 0...");
    std::vector<int> big = {5, 5, 5};
    NeuralNetwork testNetworkBig(big, 0);
    BOOST_CHECK(testNetworkBig.getConnections() == 55);
    BOOST_CHECK(testNetworkBig.inputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBig.outputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBig.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 5, 5, 5 neurons weight 1000...");
    NeuralNetwork testNetworkBigTwo(big, 1000);
    BOOST_CHECK(testNetworkBigTwo.getConnections() == 55);
    BOOST_CHECK(testNetworkBigTwo.inputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBigTwo.outputNeurons->size() == 5);
    BOOST_CHECK(testNetworkBigTwo.updateState() == 10);

}
