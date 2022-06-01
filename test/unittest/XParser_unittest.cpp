
#include <sstream>

#include "gtest/gtest.h"

using namespace std;

TEST( XParser, BasicTest )
{
    EXPECT_STREQ( "GOT LAZY", "GOT LAZY" );
}
