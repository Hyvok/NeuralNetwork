#include <iostream>
#include "NeuralNetwork.h"
#include "Neuron.h"

NeuralNetwork::NeuralNetwork() {}

NeuralNetwork::NeuralNetwork(	int nInputNeurons, int nOutputNeurons, 
                                int nHiddenNeurons) : 
                                neurons(nInputNeurons+nOutputNeurons
                                +nHiddenNeurons) {

        int nNeurons = 0;

        for(size_t i = 3; i > 0; --i)
        {
            switch(i)
            {
                case 3:
                    for(size_t j = 0; j < nOutputNeurons; ++j)
                    {
                        neurons[i].push_back(Neuron(nHiddenNeurons));
                        ++nNeurons;
                    }
                    break;
                case 2:
                    for(size_t j = 0; j < nHiddenNeurons; ++j)
                    {
                        neurons[i].push_back(Neuron(nInputNeurons));
                        ++nNeurons;
                    }
                    break;
                case 1:
                    for(size_t j = 0; j < nInputNeurons; ++j)
                    {
                        neurons[i].push_back(Neuron(1));	
                        ++nNeurons;
                    }
                    break;
            }
        }
        connectNetwork();
        std::cout << "Number of neurons created: " << nNeurons << "\n";
    }
int NeuralNetwork::connectNetwork()
{
    int nConnections = 0;
    for(std::vector<std::vector<Neuron> >::iterator iter = neurons.end(); iter != (neurons.begin()+1); --iter)
    {
        for(std::vector<Neuron>::iterator inner_iter = (*iter).begin(); inner_iter != (*iter).end(); ++inner_iter)
        {
            for(size_t i = 0; i < (*inner_iter).inputSynapses.size(); ++i)
            {
                (*inner_iter).inputSynapses[i] = (*(inner_iter-1)).outputSynapse;
                ++nConnections;
            }
        }
    }
    return nConnections;
}
int NeuralNetwork::updateState()
{
    int updates = 0;

    for(std::vector<std::vector<Neuron> >::iterator iter = neurons.begin();
            iter != neurons.end(); ++iter)
    {
        for(std::vector<Neuron>::iterator inner_iter = (*iter).begin();
                inner_iter != (*iter).end(); ++inner_iter)
        {

            updates += (*inner_iter).updateState();
        }
    }

    return updates;
}
