#include <vector>
#include <string>
#include <sstream>
#include <ios>
#include <boost/log/trivial.hpp>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include "NnImageMap.h"


NnImageMap::NnImageMapData::NnImageMapData() :  trainingCase(""), input(0), 
                                                output(0) {}

NnImageMap::NnImageMapData::NnImageMapData( std::string trainingCase, 
                            std::vector<float> input, 
                            std::vector<float> output) : 
                            trainingCase(trainingCase), input(input), 
                            output(output) {}

NnImageMap::NnImageMap() :  mappedImages_(0), nMappedImages_(0), 
                            nInput_(0), nOutput_(0) {}

NnImageMap::NnImageMap( std::vector<std::string> fileNames) : 
                        mappedImages_(0), nMappedImages_(0), nInput_(0), 
                        nOutput_(0)
{
    
    using namespace boost::gil;

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
            BOOST_LOG_TRIVIAL(error) << "\tError opening file " << image 
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
        if(!error)
        {
            BOOST_LOG_TRIVIAL(info) << "\tLoaded image " << image 
                                    << ", size: " << dimensions.x << "x" 
                                    << dimensions.y << ", type: " << type;
        }
        error = false;
    }

    nInput_ = nMaxPixels;

    // Fix the sizes of the output vectors in mappedImages_ after we have loaded
    // all the images and we know the total amount of images
    mapOutputs();

    BOOST_LOG_TRIVIAL(info) << "\tNumber of images loaded: " << nMappedImages_ 
                            << ", most amount of pixels: " << nMaxPixels;

}

NnImageMap::NnImageMapData& NnImageMap::operator[](size_t n)
{

    return mappedImages_[n];

}

void NnImageMap::mapOutputs()
{

    for(auto& mappedImage: mappedImages_)
    {
        mappedImage.output.resize(nMappedImages_);
        mappedImage.output[nOutput_] = 1;
        ++nOutput_;
    }

}

size_t NnImageMap::size() const
{

    return mappedImages_.size();

}

// TODO: fix these, do not work with empty mappedImages_!
size_t NnImageMap::inSize() const
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
size_t NnImageMap::outSize() const
{

    return nOutput_;

}

std::vector<float> NnImageMap::NnImageMapData::out() const
{

    return output;

}

std::vector<float> NnImageMap::NnImageMapData::in() const
{

    return input;

}

std::string NnImageMap::NnImageMapData::getOutStr() const
{

    std::stringstream ss;

    for(auto& val: output)
    {
        ss << val;
    }

    return ss.str();

}
