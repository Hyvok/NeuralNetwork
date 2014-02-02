#include <vector>
#include <string>
#include <sstream>
#include <ios>
#include <boost/log/trivial.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include "NnImageMap.h"

using namespace boost::gil;

NnImageMap::NnImageMapData::NnImageMapData() :  trainingCase(""), input(0), 
                                                output(0) {}

NnImageMap::NnImageMapData::NnImageMapData( std::string trainingCase, 
                            std::vector<float> input, 
                            std::vector<float> output) : 
                            trainingCase(trainingCase), input(input), 
                            output(output) {}

NnImageMap::NnImageMap() :  mappedImages_(0), nMappedImages_(0), 
                            nLargestInput_(0) {}

NnImageMap::NnImageMap( std::vector<std::string> fileNames) : 
                        mappedImages_(0), nMappedImages_(0), nLargestInput_(0)
{

    // Currently hardcoded to BW images
    gray8_image_t img;
    point2<std::ptrdiff_t> dimensions;
    int nMaxPixels = 0;
    int currentPixels = 0;
    bool error = false;

    // Temp before mapping
    std::vector<float> input(0);
    std::vector<float> output(0);

    // TODO: support for other image types than PNG and detect type
    std::string type = "png";

    for(auto& image: fileNames)
    {
        try
        {
            dimensions = png_read_dimensions(image);

            // TODO: Figure out what this convert really does, does not seem to
            // work without converting...
            png_read_and_convert_image(image, img);
        }
        catch(std::ios_base::failure e)
        {
            BOOST_LOG_TRIVIAL(error) << "Error opening file " << image 
                                    << ", error: " << e.what();
            error = true;
        }

        currentPixels = dimensions.x * dimensions.y;

        if(!error)
        {
            // TODO: combine pixels for BW images
            // Using PixelInserter insert each pixel value to input
            for_each_pixel(const_view(img), PixelInserter(&input));
            mappedImages_.emplace_back(image, input, output);
            input.clear();
            ++nMappedImages_;
        }
            

        if(currentPixels > nMaxPixels)
        {
            nMaxPixels = currentPixels;
        }
        // Not sure if this can even happen
        if(currentPixels == 0 && !error)
        {
            BOOST_LOG_TRIVIAL(error) << "Error loading image " << image
                                    << ", image contains no pixels";
        }
        else if(!error)
        {
            BOOST_LOG_TRIVIAL(info) << "Loaded image " << image 
                                    << ", size: " << dimensions.x << "x" 
                                    << dimensions.y << ", type: " << type;
        }
        error = false;
    }

    nLargestInput_ = nMaxPixels;

    // Fix the sizes of the output vectors in mappedImages_ after we have loaded
    // all the images and we know the total amount of images
    mapOutputs();

    BOOST_LOG_TRIVIAL(info) << "Number of images loaded: " << nMappedImages_ 
                            << ", most amount of pixels: " << nMaxPixels;

    /*for(auto& image: mappedImages_)
    {
        int x = 0;
        std::cout << "Mapped image: " << image.trainingCase << ", input: \n";
        for(auto& inputValue: image.input)
        {
            std::cout << inputValue << "\t";
            ++x;
            if(x == 10)
            {
                std::cout << "\n";
                x = 0;
            }
        }
        std::cout << "Mapped image: " << image.trainingCase << ", output: \n";
        for(auto& outputValue: image.output)
        {
           std::cout << outputValue << " ";
        }
        std::cout << "\n";
    }*/

}

NnImageMap::NnImageMapData NnImageMap::operator[](size_t n)
{

    return mappedImages_[n];

}

void NnImageMap::mapOutputs()
{

    int nMappedImage = 0;

    for(auto& mappedImage: mappedImages_)
    {
        mappedImage.output.resize(nMappedImages_);
        mappedImage.output[nMappedImage] = 1;
        ++nMappedImage;
    }

}

size_t NnImageMap::size()
{

    return mappedImages_.size();

}

// TODO: fix these, do not work with empty mappedImages_!
size_t NnImageMap::inSize()
{

    size_t largestSize = 0;

    for(auto& image: mappedImages_)
    {
        if(image.in().size() > largestSize)
        {
            largestSize = image.in().size();
        }
    }

    return largestSize;

}

// TODO: fix these, do not work with empty mappedImages_!
size_t NnImageMap::outSize()
{

    return mappedImages_[0].out().size();

}

std::vector<float> NnImageMap::NnImageMapData::out()
{

    return output;

}

std::vector<float> NnImageMap::NnImageMapData::in()
{

    return input;

}

std::string NnImageMap::NnImageMapData::getOutStr()
{

    std::stringstream ss;

    for(auto& val: output)
    {
        ss << val << " ";
    }

    return ss.str();

}
