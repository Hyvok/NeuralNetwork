#include <boost/test/unit_test.hpp>
#include "Synapse.h"

BOOST_AUTO_TEST_CASE(synapse_test)
{
    Synapse testSynapse;
    Synapse connectedTestSynapse(&testSynapse);

    BOOST_CHECK(testSynapse.weight == 0);
    BOOST_CHECK(testSynapse.value == 0);
    BOOST_CHECK(testSynapse.connectedTo == 0);

    BOOST_CHECK(connectedTestSynapse.connectedTo == &testSynapse);
    BOOST_CHECK(connectedTestSynapse.weight == 0);
    BOOST_CHECK(connectedTestSynapse.value == 0);

}
