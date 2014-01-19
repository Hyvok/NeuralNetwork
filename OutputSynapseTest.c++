#include <boost/test/unit_test.hpp>
#include "OutputSynapse.h"

BOOST_AUTO_TEST_CASE(output_synapse_test)
{

    OutputSynapse testSynapse;

    BOOST_CHECK(testSynapse.value == 0.0);

}
