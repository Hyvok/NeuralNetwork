#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "../NnTrainer.h"
#include "../NeuralNetwork.h"

// TODO: make better tests
BOOST_AUTO_TEST_CASE(neural_network_trainer_test)
{

    BOOST_TEST_MESSAGE("Testing NnTrainer with empty network...");
    NeuralNetwork emptyNetwork;
    NnTrainer emptyTrainer(emptyNetwork);

    //std::vector<int> normal = {3, 2, 1};
    std::vector<int> normal = {4, 3, 2, 1};
    NeuralNetwork testNetwork(normal, 0);
    //NnTrainer testTrainer(testNetwork, {1, 0, 0}, {1});
    NnTrainer testTrainer(testNetwork, {1, 0, 0, 0}, {1});

    BOOST_TEST_MESSAGE("NnTrainer trainNetwork()...");
    BOOST_CHECK(testTrainer.trainNetwork() == 20);

    int nUpdates = 0;

    for(size_t i = 0; i < 500; ++i)
    {
        nUpdates += testTrainer.trainNetwork();
        //testNetwork.getWeights();
        if(testNetwork.getOutput()[0] > 0.9 && testNetwork.getOutput()[0] < 1.1)
        {
            BOOST_TEST_MESSAGE( "Target output reached in " << i 
                                << " rounds with " << nUpdates 
                                << " weights updated, " << " output value: " 
                                << testNetwork.getOutput()[0]);
            break;
        }
        if(i == 500-1)
        {
            BOOST_ERROR("Output not converging in 500 rounds...");
            testNetwork.getWeights();
            break;
        }
    }

}
