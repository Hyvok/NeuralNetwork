#include <boost/test/unit_test.hpp>
#include "InputSynapse.h"

BOOST_AUTO_TEST_CASE(input_synapse_test)
{

    InputSynapse testSynapse;

    BOOST_CHECK(testSynapse.weight == 0);
    BOOST_CHECK(testSynapse.value == 0);

}
