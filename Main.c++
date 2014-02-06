#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/log/trivial.hpp>
#include "NeuralNetworkTrainer.h"
#include "NeuralNetwork.h"
#include "NnImageMap.h"
#include "Config.h" 

namespace po = boost::program_options;

// TODO: use "more" standard exit codes
// TODO: implement stopping criteria for training
// TODO: implement configuration file
// TODO: implement multiple logging levels
// TODO: implement verbose options, related to ^
// TODO: implement validators for options
int main(int argc, char *argv[])
{

    po::options_description generic("Command-line options");
    generic.add_options()
        ("help,h", "help message")
        ("version,v", "print version");

    po::options_description config("Options in config file and command-line");
    config.add_options()
        ("network,n", po::value<std::vector<unsigned int> >()->multitoken(), 
            "number of hidden neurons per layer, "
            "input and output layers sizes are based on input data)")
        ("activation,a", po::value<std::string>(), 
            "activation function of the neurons")
        ("training,t", po::value<std::string>(), "training algorithm")
        ("iterations,i", po::value<unsigned int>(), "number of iterations of training")
        ("input-file", po::value<std::vector<std::string> >(), "input file for training")
        ("learning_rate", po::value<float>()->default_value(DEFAULT_LEARNING_RATE), 
            "learning rate for back-propagation");

    // Map all the filenames that come after the options to name input-file
    po::positional_options_description pd;
    pd.add("input-file", -1);

    po::options_description cmdLineOptions;
    cmdLineOptions.add(generic).add(config);
        
    po::options_description configFileOptions;
    configFileOptions.add(config);
        
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(cmdLineOptions).positional(pd).run(), vm);
    po::notify(vm);
    
    std::vector<unsigned int> nNeurons(0);

    BOOST_LOG_TRIVIAL(info) << "Configuration:";

    if(vm.count("help")) 
    {
        std::cout << cmdLineOptions << "\n";
        return 1;
    }
    if(vm.count("version"))
    {
        std::cout << "NeuralNetwork version: " << VERSION << "\n";
    }
    if(vm.count("activation"))
    {
        // TODO: implement
        BOOST_LOG_TRIVIAL(info) << "\tActivation function of neurons set to: "
                                << vm.count("activation");
    }
    else
    {
        // TODO: implement
        BOOST_LOG_TRIVIAL(info) << "\tActivation function defaulting to "
                                << QUOTEMACRO(DEFAULT_ACTIVATION_FUNCTION);
    }
    if(vm.count("iterations"))
    {
        BOOST_LOG_TRIVIAL(info) << "\tNumber of training iterations set to: "
                                << vm["iterations"].as<unsigned int>();
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "\tNumber of training iterations defaulting "  
                                << "to " << QUOTEMACRO(DEFAULT_ITERATIONS);
    }
    if(vm.count("training"))
    {
        // TODO: implement
        BOOST_LOG_TRIVIAL(info) << "\tTraining algorithm set to: "
                                << vm["training"].as<std::string>();
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "\tTraining algorithm defaulting to "
                                << QUOTEMACRO(BACKPROP);
    }
    if(vm.count("learning_rate"))
    {
        BOOST_LOG_TRIVIAL(info) << "\tLearning rate set to "
                                << vm["learning_rate"].as<float>();
    }
    // TODO: network option is too greedy, implement validator
    if(vm.count("network")) 
    {
        nNeurons = vm["network"].as<std::vector<unsigned int> >();

        std::stringstream ss;
        
        ss << "\tNumber of hidden neurons was set to ";

        for(size_t n = 0; n < vm["network"].as<std::vector<unsigned int> >().size(); ++n)
        {
            ss << vm["network"].as<std::vector<unsigned int> >()[n] << " ";
        }

        BOOST_LOG_TRIVIAL(info) << ss.str();
    }
    else 
    {
        BOOST_LOG_TRIVIAL(info) << "Number of hidden neurons not set, " 
                                << "defaulting to number of input neurons "
                                << "divided by two";
    }
    if(vm.count("input-file"))
    {
        std::stringstream ss;


        for(size_t n = 0; n < vm["input-file"].as<std::vector<std::string> >().size(); ++n)
        {
            ss << "\tInput file: ";
            ss << vm["input-file"].as<std::vector<std::string> >()[n];
            BOOST_LOG_TRIVIAL(info) << ss.str();
            ss.str("");
        }
    }

    BOOST_LOG_TRIVIAL(info) << "Loading images...";
    NnImageMap imageMap(vm["input-file"].as<std::vector<std::string> >());

    // Default to input neurons divided by two for the amount of hidden neurons
    if(nNeurons.size() == 0)
    {
        nNeurons.resize(1);
        nNeurons[0] = imageMap.inSize() / 2;
    }
    nNeurons.insert(nNeurons.begin(), imageMap.inSize());
    nNeurons.push_back(imageMap.outSize());

    NeuralNetwork network(nNeurons);

    NeuralNetworkTrainer trainer(   network, imageMap, 
                                    vm["learning_rate"].as<float>());

    // Train network
    BOOST_LOG_TRIVIAL(info) << "Starting network training...";
    auto trainingStart = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    size_t nWeights = 0;
    // Disable cursor
    system("setterm -cursor off");
    for(size_t i = 0; i < vm["iterations"].as<unsigned int>() + 1; ++i)
    {
        // Print info every 100 iterations, otherwise printing will suck a lot
        // of CPU
        if(i % 100 == 0)
        {
            std::cout.flush();
            std::cout   << "\rIteration " << i << "/" 
                        << vm["iterations"].as<unsigned int>();
        }
        nWeights += trainer.trainNetwork();
    }
    auto trainingStop = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << std::endl;
    // Enable cursor
    system("setterm -cursor on");

    // Statistics of the training
    float trainingTime = (trainingStop - trainingStart) / 1000.0;

    BOOST_LOG_TRIVIAL(info) << "Network training finished...";

    BOOST_LOG_TRIVIAL(info) << "Performance statistics:";
    BOOST_LOG_TRIVIAL(info) << "\tRunning " << vm["iterations"].as<unsigned int>() 
                            << " iterations took " << trainingTime
                            << " seconds which is " 
                            << (vm["iterations"].as<unsigned int>() / trainingTime)
                            << " iterations/s";
    BOOST_LOG_TRIVIAL(info) << "\tUpdated " << nWeights 
                            << " weights at a rate of "
                            << (nWeights / trainingTime) << " weights/s";

    // After training print out the network outputs with all the inputs
    // TODO: gather and print statistics of the training
    BOOST_LOG_TRIVIAL(info) << "Final test results:";
    for(size_t n = 0; n < imageMap.size(); ++n)
    {
        BOOST_LOG_TRIVIAL(info) << "\tInput "
                                << imageMap[n].trainingCase 
                                << ", desired output: " 
                                << imageMap[n].getOutStr();

        network.setInput(imageMap[n].input);
        std::vector<float> out = network.getOutput();


        for(size_t i = 0; i < out.size(); ++i)
        {
            BOOST_LOG_TRIVIAL(info) << "\t\tOutput neuron " << i << " output: \t"
                                    << out[i];
        }

    }


    return 0;

}
