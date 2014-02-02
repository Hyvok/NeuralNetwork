#include <iostream>
#include <sstream>
#include <vector>
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
        ("network,n", po::value<std::vector<int> >()->multitoken(), 
            "number of hidden neurons per layer, "
            "input and output layers sizes are based on input data)")
        ("activation,a", po::value<std::string>(), 
            "activation function of the neurons")
        ("training,t", po::value<std::string>(), "training algorithm")
        ("iterations,i", po::value<int>(), "number of iterations of training")
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
    
    std::vector<int> nNeurons(0);

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
        BOOST_LOG_TRIVIAL(info) << "Activation function of neurons set to: "
                                << vm.count("activation");
    }
    else
    {
        // TODO: implement
        BOOST_LOG_TRIVIAL(info) << "Activation function defaulting to "
                                << QUOTEMACRO(DEFAULT_ACTIVATION_FUNCTION);
    }
    if(vm.count("iterations"))
    {
        BOOST_LOG_TRIVIAL(info) << "Number of training iterations set to: "
                                << vm["iterations"].as<int>();
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Number of training iterations defaulting "  
                                << "to " << QUOTEMACRO(DEFAULT_ITERATIONS);
    }
    if(vm.count("training"))
    {
        // TODO: implement
        BOOST_LOG_TRIVIAL(info) << "Training algorithm set to: "
                                << vm["training"].as<std::string>();
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "Training algorithm defaulting to "
                                << QUOTEMACRO(BACKPROP);
    }
    if(vm.count("learning_rate"))
    {
        BOOST_LOG_TRIVIAL(info) << "Learning rate set to "
                                << vm["learning_rate"].as<float>();
    }
    // TODO: network option is too greedy, implement validator
    if(vm.count("network")) 
    {
        nNeurons = vm["network"].as<std::vector<int> >();

        std::stringstream ss;
        
        ss << "Number of hidden neurons was set to ";

        for(size_t n = 0; n < vm["network"].as<std::vector<int> >().size(); ++n)
        {
            ss << vm["network"].as<std::vector<int> >()[n] << " ";
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
            ss << "Input file: ";
            ss << vm["input-file"].as<std::vector<std::string> >()[n];
            BOOST_LOG_TRIVIAL(info) << ss.str();
            ss.str("");
        }
    }

    NnImageMap imageMap(vm["input-file"].as<std::vector<std::string> >());

    // Default to input neurons divided by two for the amount of hidden neurons
    if(nNeurons.size() == 0)
    {
        nNeurons.resize(1);
        nNeurons[0] = imageMap.inSize() / 2;
    }
    nNeurons.insert(nNeurons.begin(), imageMap.inSize());
    nNeurons.push_back(imageMap.outSize());

    // Put the architecture in a stringstream for logging
    std::stringstream architecture;
    for(auto& layer: nNeurons)
    {
        architecture << layer;

        // No space after the last one
        if(&layer == &(nNeurons.back())) {}
        else
        {
            architecture << " ";
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Architecture of the network: " 
                            << architecture.str();

    NeuralNetwork network(nNeurons);

    NeuralNetworkTrainer trainer(   network, imageMap, 
                                    vm["learning_rate"].as<float>());

    // Train network
    for(size_t i = 0; i < vm["iterations"].as<int>(); ++i)
    {
        trainer.trainNetwork();
    }

    // After training print out the network outputs with all the inputs
    for(size_t n = 0; n < imageMap.size(); ++n)
    {
        std::cout   << "Network output with input "
                    << imageMap[n].trainingCase << ", desired output: " 
                    << imageMap[n].getOutStr();

        network.setInput(imageMap[n].input);
        std::vector<float> out = network.getOutput();

        std::cout << ", output: ";

        for(size_t i = 0; i < out.size(); ++i)
        {
            std::cout << out[i] << " ";
        }
        std::cout << "\n";

    }

    return 0;

}
