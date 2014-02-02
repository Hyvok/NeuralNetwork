#ifndef NN_IMAGE_MAP_H
#define NN_IMAGE_MAP_H

#include <vector>
#include <string>
#include <boost/gil/gil_all.hpp>

// Class for mapping training image files to suitable format that is then
// given to NeuralNetworkTrainer

// TODO: currently only supports "linear" output mapping where one of the 
// outputs corresponds to one training case
// TODO: currently only supports one training case per "item to be learned"
// TODO: implement iterators for NnImageMap
// TODO: fails if given a folder instead of an image
class NnImageMap
{

    public:

        NnImageMap();

        // Load image specified by filename
        NnImageMap(std::vector<std::string> fileNames);

        // Return number of training cases in this NnImageMap
        size_t size();

        // Return LARGEST input vector size
        size_t inSize();

        // Return output vector size
        size_t outSize();

        // Data container class for the data
        class NnImageMapData
        {

            public:
                
                NnImageMapData();
    
                NnImageMapData( std::string trainingCase, std::vector<float> input, std::vector<float>);

                // Return output vector
                std::vector<float> out();

                // Return the input vector
                std::vector<float> in();

                // Name for the training case, currently filename
                std::string trainingCase;

                // Input neuron values
                std::vector<float> input;

                // Desired output neuron values
                std::vector<float> output;

                // Return desired output neuron values as a std::string
                std::string getOutStr();

        };

        NnImageMap::NnImageMapData operator[](size_t n);

    private:

        struct PixelInserter
        {
            
            PixelInserter(std::vector<float>* input) : input_(input) {}

            void operator()(boost::gil::gray8_pixel_t p) const 
            {
                input_->push_back(p[0]);

                // Needed for color
                //input_->push_back(boost::gil::at_c<1>(p));
                //input_->push_back(boost::gil::at_c<2>(p));
            }

            std::vector<float>* input_;

        };

        // Fixes the output vectors after loading all the images
        void mapOutputs();

        std::vector<NnImageMap::NnImageMapData> mappedImages_;
        int nMappedImages_;
        int nLargestInput_;

};

#endif
