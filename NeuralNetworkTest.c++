#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_test)
{
    
    BOOST_TEST_MESSAGE("Testing NeuralNetwork with empty network...");
    NeuralNetwork emptyNetwork;
    BOOST_CHECK(emptyNetwork.getConnections() == 0);
    BOOST_CHECK(emptyNetwork.inputNeurons == 0);
    BOOST_CHECK(emptyNetwork.outputNeurons == 0);

    std::vector<std::vector<int> > zero_weight = {{3, 2, 1}, {1, 1, 1}, {5, 5, 5}};

    for(size_t nTest = 0; nTest < zero_weight.size(); ++nTest)
    {
            
        std::stringstream ss;

        for(auto& nNeurons: zero_weight[nTest])
        {
            ss << nNeurons << ", ";        
        }
    
        BOOST_TEST_MESSAGE("Testing NeuralNetwork with: " << ss.str() << "with weight 0, input 0, 0, 0...");
        NeuralNetwork testNetwork(zero_weight[nTest], 0);
    
        int nConnections = 0;

        for(size_t layer = 0; layer < zero_weight[nTest].size(); ++layer)
        {
            if(layer == 0)
            {
                nConnections += zero_weight[nTest][layer];
            }

            if(layer == zero_weight[nTest].size()-1)
            {
                //nConnections += zero_weight[nTest][layer];
            }
            else
            {
                nConnections += zero_weight[nTest][layer+1] * zero_weight[nTest][layer];
            }
        }

        BOOST_CHECK(testNetwork.getConnections() == nConnections);
        BOOST_CHECK(testNetwork.inputNeurons->size() == zero_weight[nTest][0]);
        BOOST_CHECK(testNetwork.outputNeurons->size() == zero_weight[nTest].back());
        BOOST_CHECK(testNetwork.updateState() == 0);

    }

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 3, 2, 1 neurons weight 1000, input 0, 0, 0...");
    std::vector<int> normal_thousand = {3, 2, 1};
    NeuralNetwork testNetwork(normal_thousand, 1000);
    BOOST_CHECK(testNetwork.getConnections() == 11);
    BOOST_CHECK(testNetwork.inputNeurons->size() == 3);
    BOOST_CHECK(testNetwork.outputNeurons->size() == 1);
    BOOST_CHECK(testNetwork.updateState() == 0);

    BOOST_TEST_MESSAGE("Testing NeuralNetwork with 3, 2, 1 neurons weight 1000, input 1, 0, 0...");
    testNetwork.inputNeurons->front().inputSynapses.front().value = 1;
    BOOST_CHECK(testNetwork.updateState() == 3);

}
