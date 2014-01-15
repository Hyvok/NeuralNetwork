#include <boost/test/unit_test.hpp>
#include "Synapse.h"

BOOST_AUTO_TEST_CASE(synapse_test)
{
    Synapse testSynapse;

    BOOST_CHECK(testSynapse.weight == 0);
    BOOST_CHECK(testSynapse.value == 0);

}
