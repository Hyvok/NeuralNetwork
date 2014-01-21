#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetworkTrainer.h"
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_trainer_test)
{

    BOOST_TEST_MESSAGE("Testing NeuralNetworkTrainer with empty network...");
    NeuralNetwork emptyNetwork;
    NeuralNetworkTrainer emptyTrainer(emptyNetwork);

    std::vector<int> normal = {3, 2, 1};
    NeuralNetwork testNetwork(normal, 0);
    NeuralNetworkTrainer testTrainer(testNetwork, {1, 0, 0}, {1});

    BOOST_TEST_MESSAGE("NeuralNetworkTrainer trainNetwork()...");
    testNetwork.setInput({1, 0, 0});
    //testNetwork.getWeights();
    //testNetwork.getState();

    for(size_t i = 0; i < 500; ++i)
    {
        BOOST_TEST_MESSAGE("Training round " << i << "...");
        testTrainer.trainNetwork();
        //testNetwork.getWeights();
        //testNetwork.getState();
        if(testNetwork.getOutput()[0] > 0.9 && testNetwork.getOutput()[0] < 1.1)
        {
            BOOST_TEST_MESSAGE("Output value: " << testNetwork.getOutput()[0]);
            break;
        }
    }
    /*
    testTrainer.trainNetwork();
    testNetwork.updateState();
    testNetwork.getWeights();
    testTrainer.trainNetwork();
    testNetwork.updateState();
    testNetwork.getWeights();
    testNetwork.getState();*/

}
