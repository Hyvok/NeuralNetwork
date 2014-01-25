#include <vector>
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include "NnImageMapper.h"

using namespace boost::gil;

NnImageMapper::NnImageMapper() : {}

NnImageMapper::NnImageMapper(std::string &fileName) : 
{

    dimensions = png_read_dimensions(fileName);

}
