//
// Created by Ivan Shynkarenka on 26.01.2016.
//

#include "catch.hpp"

#include "time/timestamp.h"

using namespace CppCommon;

TEST_CASE("Nanoseconds timestamp accessor", "[CppCommon][Time]")
{
    REQUIRE(timestamp() > 0);

    uint64_t prev = 0;
    for (int i = 0; i < 1000; ++i)
    {
        uint64_t next = timestamp();
        REQUIRE(prev <= next);
        prev = next;
    }
}