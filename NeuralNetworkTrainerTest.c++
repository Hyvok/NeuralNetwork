#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "NeuralNetwork.h"

BOOST_AUTO_TEST_CASE(neural_network_trainer_test)
{

    BOOST_TEST_MESSAGE("Testing NeuralNetworkTrainer with empty network...");
    NeuralNetwork emptyNetwork;
    NeuralNetworkTrainer emptyTrainer(emptyNetwork);

}
